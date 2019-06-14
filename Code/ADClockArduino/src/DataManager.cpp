#include "DataManager.h"

DataManager::DataManager(InputStream &in, OutputStream &out, Motor &motor1, Motor &motor2) : in(in), out(out), motor1(motor1), motor2(motor2)
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
    motor1.start_calibraton();
    motor2.start_calibraton();
    break;

  case 0x02: // Image
    readMyClockImage();
    if (out.sendData(command))
    {
      pipeIncommingData();
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
      // Serial.println("Clock image unvollständig.");
      return; // Keine Daten? Blöd gelaufen
    }
    input[i] = in.readData();
  }

  // Werte einlesen
  uint16_t position_1 = (input[0] << 8) + input[1];
  uint8_t delay_1 = input[2];
  int8_t speed_1 = input[3];

  uint16_t position_2 = (input[4] << 8) + input[5];
  uint8_t delay_2 = input[6];
  int8_t speed_2 = input[7];
  // Serial.println("Clock image " + String(input[0]) + String(input[1]) + String(input[2]) + String(input[3]) + String(input[4]) + String(input[5]) + String(input[6]) + String(input[7]) +
  //                " und target: " + String(position_1) + " " + String(position_2));

  // Aktuell für Testzwecke um immer das gleiche Image senden zu können
  motor1.reset_position();
  motor2.reset_position();

  motor1.set_target_pos(position_1);
  motor2.set_target_pos(position_2);
  // Serial.println("New Clock image set.");
}