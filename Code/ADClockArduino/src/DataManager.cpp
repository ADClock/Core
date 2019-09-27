#include "DataManager.h"

DataManager::DataManager(InputStream &in, OutputStream &out, MotorManager &motormanager) : in(in), out(out), moma(moma)
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
    this->moma.calibrate();
    break;

  case 0x02: // Image
    readMyClockImage();
    if (out.sendData(command))
    {
      pipeIncommingData();
    }
    else
    {
      Serial.println("no arduino listening");
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

  DataStruct motordata[2];
  motordata[0] = deserialze(&input[0]);
  motordata[1] = deserialze(&input[4]);

  this->moma.setMotorData(motordata);
}

DataStruct DataManager::deserialze(uint8_t *stream)
{
  DataStruct data;
  data.position = ((stream[0] << 4) + ((stream[1] >> 4) & 0x0F));
  data.waitSteps = ((stream[1] & 0x0F) << 8) + stream[2];
  data.delay = stream[3] & 0xFE;
  data.direction = stream[3] & 0x01;
  return data;
}
