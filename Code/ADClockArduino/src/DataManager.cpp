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
    checkForData();
    delayMicroseconds(100);
  }
}

void DataManager::checkForData()
{
  if (in.hasData())
  {
    reciveData();
    Serial.println("Data reading complete");
  }
}

void DataManager::reciveData()
{
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
    out.sendData(command);
    pipeIncommingData();
    break;

  default:
    Serial.println("Unbekannter Command: " + String(command));
    break;
  }
}

void DataManager::pipeIncommingData()
{
  // Kleinen Trick angewandt: Durch das Senden des Commands ist beim Pipe bereits genug Zeit vergangen.
  // Eigentlich müsste man zu Beginn ggf. noch auf die Daten warten.

  while (in.hasData())
  {
    out.sendData(in.readData());
    // Auch hier wieder, zuerst wird gelesen dann gesendet. Durch diesen zeitlichen Versatz liegen die nöchsten Daten garantiert wieder an.
  }
}

void DataManager::readMyClockImage()
{
  uint8_t input[8];
  // 8 Byte lesen (4 je Motor)
  // -- Position  (16 Bit)
  // -- Delay     (8 Bit)
  // -- Speed     (7 Bit)
  // -- Direction (1 Bit)
  for (int i = 0; i < 8; i++)
  {
    if (!in.hasData())
    {
      // Keine Daten? Blöd. Vielleicht in einer 10 µs
      delayMicroseconds(10);
      if (!in.hasData())
        return; // Keine Daten? Blöd gelaufen
    }
    input[i] = in.readData();
  }

  // Motor bewegen
  uint16_t foo = *input;
  Serial.println("Clock image " + String(input[0]) + String(input[1]) + String(input[2]) + String(input[3]) + String(input[4]) + String(input[5]) + String(input[6]) + String(input[7]) +
                 " und target: " + String(foo));
  motor2.set_target_pos(foo);
}