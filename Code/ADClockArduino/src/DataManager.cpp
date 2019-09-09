#include "DataManager.h"

DataManager::DataManager(InputStream &in, OutputStream &out, Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2) : in(in), out(out), motor1(motor1), motor2(motor2), calibration1(calibration1), calibration2(calibration2)
{
}

void DataManager::delayAndCheck(size_t ms)
{
  ms *= 10;
  size_t count;
  for (count = 0; count < ms; count++)
  {
    checkForData(); // Einen digital Pin abfragen ~ 4 ms
    delayMicroseconds(96);
  }
}

void DataManager::checkForData()
{
  if (in.hasData())
  {
    reciveData();
    // Serial.println("Data reading complete");
  }
}

void DataManager::reciveData()
{
  // Serial.println("Recieving data");
  byte command = in.readData();
  switch (command)
  {
  case 0x01: // Init
    out.sendData(command);
    calibrate();
    break;

  case 0x02: // Image
    readMyClockImage();
    if (out.sendData(command))
    {
      pipeIncommingData();
    }
    break;

  case 0x03: // Speedtest
    while (in.hasData())
    {
      in.readData();
    }
    break;

  default:
    Serial.println("Unbekannter Command: " + String(command));
    break;
  }
  // Serial.println("Recieving data complete");
}

void DataManager::pipeIncommingData()
{
  // Kleinen Trick angewandt: Durch das Senden des Commands ist beim Pipe bereits genug Zeit vergangen.
  // Eigentlich müsste man zu Beginn ggf. noch auf die Daten warten.

  while (in.hasData())
  {
    if (!out.sendData(in.readData()))
      break; // Empfänger hat nicht mehr gelesen. & Tschüss
    // Auch hier wieder, zuerst wird gelesen dann gesendet. Durch diesen zeitlichen Versatz liegen die nöchsten Daten garantiert wieder an.
  }
  // Serial.println("Pipe complete");
}

void DataManager::readMyClockImage()
{
  uint8_t input[8];
  // 8 Byte lesen (4 je Motor)
  // -- Position  (16 Bit)
  // -- Delay     (8 Bit)
  // -- Direction (1 Bit)
  // -- Speed     (7 Bit)
  for (int i = 0; i < 8; i++)
  {
    if (!in.waitForData())
    {
      Serial.println("Clock image unvollständig.");
      return; // Keine Daten? Blöd gelaufen
    }
    input[i] = in.readData();
  }

  // Werte einlesen
  uint16_t position_1 = ((input[0] << 4) + ((input[1] >> 4) & 0x0F));
  uint16_t waitSteps_1 = ((input[1] & 0x0F) << 8) + input[2];
  int8_t delay_1 = input[3] & 0xFE;
  bool direction_1 = input[3] & 0x01;

  uint16_t position_2 = ((input[4] << 4) + ((input[5] >> 4) & 0x0F));
  uint16_t waitSteps_2 = ((input[5] & 0x0F) << 8) + input[6];
  int8_t delay_2 = input[7] & 0xFE;
  bool direction_2 = input[7] & 0x01;
  Serial.println("Clock image " + String(input[0]) + " " + String(input[1]) + " " + String(input[2]) + " " + String(input[3]) + " " + String(input[4]) + " " + String(input[5]) + " " + String(input[6]) + " " + String(input[7]) +
                 " und target: " + String(position_1) + " " + String(position_2) + " waitSteps: " + String(waitSteps_1) + " " + String(waitSteps_2) + " stepDelay: " + String(delay_1) + " " + String(delay_2) + " direction: " + String(direction_1) + " " + String(direction_2));

  // // Aktuell für Testzwecke um immer das gleiche Image senden zu können
  // motor1.reset_position();
  // motor2.reset_position();

  motor1.set_target_pos(position_1);
  motor1.set_step_delay(delay_1);
  motor1.set_wait_steps(waitSteps_1);
  motor1.set_direction(direction_1);

  motor2.set_target_pos(position_2);
  motor2.set_step_delay(delay_2);
  motor2.set_wait_steps(waitSteps_2);
  motor2.set_direction(direction_2);
  // Serial.println("New Clock image set.");
}

void DataManager::calibrate()
{
  calibration1.start_calibration();
  calibration2.start_calibration();
  bool motor1Calibrated = false;
  bool motor2Calibrated = false;
  do
  {
    motor1Calibrated = calibration1.calibrate();
    motor2Calibrated = calibration2.calibrate();
    delay(10);
  } while (!motor1Calibrated || !motor2Calibrated);
}