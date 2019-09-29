#ifndef _OutputStream_H_
#define _OutputStream_H_
#include "mbed.h"
#include "../Debug.h"
#include "../Config.h"

class OutputStream
{
public:
  OutputStream();

  // Verschickt einen Byte
  bool sendData(const uint8_t &data);

  // Verschickt ein Array aus Daten. Cool oder?
  bool sendDataArray(const uint8_t arr[], const size_t &length);

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
