#include "ClockOutputStream.h"

ClockOutputStream::ClockOutputStream()
{
  pin_clock = 0;
}

bool ClockOutputStream::sendByte(const uint8_t &data)
{
  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    sendDataBit(!!(data & (1 << i)));

    if (!checkDataReadingComplete())
    {
#ifdef DEBUG
      Debug::serial.printf("Bit %d konnte nicht gesendet werden", i);
#endif
      return false; // Bit wurde nicht gelesen -> Übertragung fehlerhaft
    }
  }
  return true;
}

bool ClockOutputStream::sendByteArray(const uint8_t arr[], const size_t &length)
{
  for (size_t i = 0; i < length; i++)
  {
    // Debug::serial.printf("%d ", arr[i]);
    if (!sendByte(arr[i]))
      return false;
  }
  // Debug::println("");
  return true;
}

void ClockOutputStream::sendDataBit(bool bit)
{
  pin_data = bit;
  pin_clock = 1;
}

bool ClockOutputStream::checkDataReadingComplete()
{
  // Warten das Response aktiviert wird. (Der Empfänger das Bit gelesen hat)
  unsigned long delayTimer = 0;

  // TODO
  while (!pin_response.read())
  {
    if (delayTimer > 10000) // Innerhalb 100000 µs = 100ms keine Response - das kann aber nicht ganz stimmen
    {
      pin_clock = 0;
#ifdef DEBUG
      Debug::println("ClockOut >> No response recived from arduino.");
#endif
      return false; // Langsam hätte die Response an sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
  }

  pin_clock = 0;

  // Warten, dass der Empfänger die Response wieder deaktivert.
  delayTimer = 0;
  while (pin_response.read())
  {
    if (delayTimer > 50000) // Innerhalb 100000 µs = 100ms keine Response
    {
#ifdef DEBUG
      Debug::println("ClockOut >> Response still active. (Arduino to slow?)");
#endif
      return false; // Langsam hätte die Response wieder aus sein müssen -> Der Empfänger ist mir zu langsam, mit dem Rede ich nicht mehr. Nagut.. Vielleicht gleich nochmal.
    }
    delayTimer++;
  }

  return true;
}
