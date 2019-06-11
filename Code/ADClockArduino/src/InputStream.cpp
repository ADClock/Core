#include "InputStream.h"

#define IN_RESPONSE A3
#define IN_DATA A4
#define IN_CLOCK A5

InputStream::InputStream()
{
  pinMode(IN_RESPONSE, OUTPUT);
  digitalWrite(IN_RESPONSE, LOW);
}

bool InputStream::hasData()
{
  return FastGPIO::Pin<IN_CLOCK>::isInputHigh();
}

byte InputStream::readData()
{
  uint8_t value = 0;
  uint8_t i;

  for (i = 0; i < 8; ++i)
  {
    if (!hasData() && !waitForData())
    { // Falls Bit unvollständig gesendet
      Serial.println("readData: Byte unvollständig.");
      return 0x00;
    }

    value |= readDataBit() << i;
    _hasData = false; // Daten wurden gelesen

    // Mitteilen, dass Bit gelesen wurde
    if (!sendDataReadingComplete())
    {
      Serial.println("readData: Clock nicht zurückgesetzt.");
      // Der Sender hat die Clock nicht ausgeschaltet.
      return 0x00;
    }
  }

  return value;
}

int InputStream::readDataBit()
{
  return FastGPIO::Pin<IN_DATA>::isInputHigh();
}

// Response pin aktivieren, warten bis clock aus ist, Response pin deaktivieren
bool InputStream::sendDataReadingComplete()
{
  FastGPIO::Pin<IN_RESPONSE>::setOutputValueHigh();

  size_t delayTimer = 0;
  while (hasData())
  {
    if (delayTimer > 10000) // Innerhalb 10000µs = 10ms keine Response
    {
      // digitalWrite(responsePin, LOW);
      FastGPIO::Pin<IN_RESPONSE>::setOutputValueLow();
      Serial.println("sendDataReadingComplete: Clock immer noch an.");
      return false; // Langsam hätte die Clock aus sein müssen -> Der Sendet hat die Geduld verloren und nicht gelaubt, dass ich den Bit noch lese.
    }
    delayTimer++;
    delayMicroseconds(1);
  }

  FastGPIO::Pin<IN_RESPONSE>::setOutputValueLow();

  return true;
}

// Wartet darauf das ein Signal an der Clock anliegt.
bool InputStream::waitForData()
{
  size_t delayTimer = 0;
  while (!hasData())
  {
    if (delayTimer > 10000) // Innerhalb 10000µs = 10ms keine Response
    {
      Serial.println("waitForData: Keine Daten gekommen.");
      return false; // Langsam hätte die Clock aus sein müssen -> Da kommt nichts mehr...
    }
    delayTimer++;
    delayMicroseconds(1);
  }

  return true;
}