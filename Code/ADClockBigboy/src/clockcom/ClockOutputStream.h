#ifndef _ClockOutputStream_H_
#define _ClockOutputStream_H_
#include "mbed.h"
#include "../Debug.h"
#include "../Config.h"
class ClockOutputStream
{
public:
  ClockOutputStream();

  // Verschickt einen Byte
  bool sendByte(const uint8_t data);

  // Verschickt ein Array aus Bytes. Cool oder?
  bool sendByteArray(const uint8_t arr[], const size_t &length);

private:
  // Verschickt einen Bit
  void sendDataBit(bool bit);
  // Prüft ob Daten erfolgreich eingelesen wurden.
  bool checkDataReadingComplete();

  DigitalOut pin_data;
  DigitalOut pin_clock;
  DigitalIn pin_response;
};

#endif
