#include "OutputStream.h"

OutputStream::OutputStream(uint8_t clockPin, uint8_t dataPin, uint8_t responsePin)
{
  this->clockPin = clockPin;
  this->dataPin = dataPin;
  this->responsePin = responsePin;

  digitalWrite(clockPin, LOW);
}

bool OutputStream::sendData(byte data)
{
  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    sendDataBit(!!(data & (1 << i)));

    if (!checkDataReadingComplete())
      return false; // Bit wurde nicht gelesen -> Übertragung fehlerhaft
  }
  return true;
}

void OutputStream::sendDataBit(bool bit)
{
  digitalWrite(dataPin, bit);
  digitalWrite(clockPin, HIGH);
}

bool OutputStream::checkDataReadingComplete()
{
  // Warten das Response aktiviert wird. (Der Empfänger das Bit gelesen hat)
  size_t delayTimer = 0;
  while (!digitalRead(responsePin))
  {
    if (delayTimer > 100000) // Innerhalb 100000 µs = 100ms keine Response
    {
      digitalWrite(clockPin, LOW); // Clock deaktivieren
      Serial.println("Keine Response erhalten.");
      return false; // Langsam hätte die Response an sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
    delayMicroseconds(1);
  }

  digitalWrite(clockPin, LOW);

  // Warten, dass der Empfänger die Response wieder deaktivert.
  delayTimer = 0;
  while (digitalRead(responsePin))
  {
    if (delayTimer > 10000) // Innerhalb 10000µs = 10ms keine Response
    {
      Serial.println("Response immer noch aktiv.");
      return false; // Langsam hätte die Response wieder aus sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
    delayMicroseconds(1);
  }

  return true;
}
