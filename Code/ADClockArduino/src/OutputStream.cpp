#include "OutputStream.h"

#define OUT_RESPONSE 3
#define OUT_DATA 4
#define OUT_CLOCK 5

OutputStream::OutputStream()
{
  pinMode(OUT_DATA, OUTPUT);
  pinMode(OUT_CLOCK, OUTPUT);
  digitalWrite(OUT_CLOCK, LOW);
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
  FastGPIO::Pin<OUT_DATA>::setOutputValue(bit);
  FastGPIO::Pin<OUT_CLOCK>::setOutputHigh();
}

bool OutputStream::checkDataReadingComplete()
{
  // Warten das Response aktiviert wird. (Der Empfänger das Bit gelesen hat)
  size_t delayTimer = 0;

  while (!FastGPIO::Pin<OUT_RESPONSE>::isInputHigh())
  {
    if (delayTimer > 10000) // Innerhalb 100000 µs = 100ms keine Response - das kann aber nicht ganz stimmen
    {
      FastGPIO::Pin<OUT_CLOCK>::setOutputValueLow();
      Serial.println("Keine Response erhalten.");
      return false; // Langsam hätte die Response an sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
    delayMicroseconds(1);
  }

  FastGPIO::Pin<OUT_CLOCK>::setOutputValueLow();

  // Warten, dass der Empfänger die Response wieder deaktivert.
  delayTimer = 0;
  while (FastGPIO::Pin<OUT_RESPONSE>::isInputHigh())
  {
    if (delayTimer > 100) // Innerhalb 100µs = 0,1ms keine Response
    {
      Serial.println("Response immer noch aktiv.");
      return false; // Langsam hätte die Response wieder aus sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
    // delayMicroseconds(1);
  }

  return true;
}
