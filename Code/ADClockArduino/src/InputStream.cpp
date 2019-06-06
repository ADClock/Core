#include "InputStream.h"

InputStream::InputStream(uint8_t clockPin, uint8_t dataPin, uint8_t responsePin)
{
  this->clockPin = clockPin;
  this->dataPin = dataPin;
  this->responsePin = responsePin;

  digitalWrite(responsePin, LOW);
}

bool InputStream::hasData()
{
  return digitalRead(clockPin);
}

byte InputStream::readData()
{
  uint8_t value = 0;
  uint8_t i;

  for (i = 0; i < 8; ++i)
  {
    if (!hasData() && !waitForClock())
    { // Falls Bit unvollständig gesendet
      Serial.println("Bit unvollständig.");
      return 0x00;
    }

    value |= readDataBit() << i;

    // Mitteilen, dass Bit gelesen wurde
    if (!sendDataReadingComplete())
    {
      Serial.println("Clock nicht zurückgesetzt.");
      // Der Sender hat die Clock nicht ausgeschaltet.
      return 0x00;
    }
  }

  return value;
}

int InputStream::readDataBit()
{
  return digitalRead(dataPin);
}

// Response pin aktivieren, warten bis clock aus ist, Response pin deaktivieren
bool InputStream::sendDataReadingComplete()
{
  digitalWrite(responsePin, HIGH);

  size_t delayTimer = 0;
  while (hasData())
  {
    if (delayTimer > 10000) // Innerhalb 10000µs = 10ms keine Response
    {
      digitalWrite(responsePin, LOW);
      return false; // Langsam hätte die Clock aus sein müssen -> Der Sendet hat die Geduld verloren und nicht gelaubt, dass ich den Bit noch lese.
    }
    delayTimer++;
    delayMicroseconds(1);
  }

  digitalWrite(responsePin, LOW);

  return true;
}

// Wartet darauf das ein Signal an der Clock anliegt.
bool InputStream::waitForClock()
{
  size_t delayTimer = 0;
  while (!hasData())
  {
    if (delayTimer > 10000) // Innerhalb 10000µs = 10ms keine Response
    {
      return false; // Langsam hätte die Clock aus sein müssen -> Da kommt nichts mehr...
    }
    delayTimer++;
    delayMicroseconds(1);
  }

  return true;
}