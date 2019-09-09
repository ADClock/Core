#include "OutputStream.h"

#define OUT_RESPONSE D3
#define OUT_DATA D4
#define OUT_CLOCK D5

OutputStream::OutputStream() : pin_data(DigitalOut(OUT_DATA)), pin_clock(DigitalOut(OUT_CLOCK)), pin_response(DigitalIn(OUT_RESPONSE))
{
  pin_clock = 0;
}

bool OutputStream::sendData(const uint8_t &data)
{
  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    sendDataBit(!!(data & (1 << i)));

    if (!checkDataReadingComplete())
    {
      Debug::serial.printf("Bit %d konnte nicht gesendet werden", i);
      return false; // Bit wurde nicht gelesen -> Übertragung fehlerhaft
    }
  }
  return true;
}

bool OutputStream::sendDataArray(const uint8_t arr[], const size_t &length)
{
  for (size_t i = 0; i < length; i++)
  {
    // Debug::serial.printf("%d ", arr[i]);
    if (!sendData(arr[i]))
      return false;
  }
  // Debug::println("");
  return true;
}

void OutputStream::sendDataBit(bool bit)
{
  pin_data = bit;
  pin_clock = 1;
}

bool OutputStream::checkDataReadingComplete()
{
  // Warten das Response aktiviert wird. (Der Empfänger das Bit gelesen hat)
  size_t delayTimer = 0;

  // TODO
  while (!pin_response.read())
  {
    if (delayTimer > 1000000) // Innerhalb 100000 µs = 100ms keine Response - das kann aber nicht ganz stimmen
    {
      // TODO
      pin_clock = 0;
      Debug::println("Keine Response erhalten.");
      return false; // Langsam hätte die Response an sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
  }

  // TODO
  pin_clock = 0;

  // Warten, dass der Empfänger die Response wieder deaktivert.
  delayTimer = 0;
  // TODO
  while (pin_response.read())
  {
    if (delayTimer > 100) // Innerhalb 100µs = 0,1ms keine Response
    {
      Debug::println("Response immer noch aktiv.");
      return false; // Langsam hätte die Response wieder aus sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
  }

  return true;
}
