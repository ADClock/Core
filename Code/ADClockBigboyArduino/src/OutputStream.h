
#ifndef _OUTPUTSTREAM_H_
#define _OUTPUTSTREAM_H_
#include "Arduino.h"
#include "FastGPIO.h"
class OutputStream
{
public:
  OutputStream();

  // Verschickt einen Byte
  bool sendData(const byte &data);

  // Verschickt ein Array aus Daten. Cool oder?
  bool sendDataArray(const byte arr[], const size_t &length);

private:
  // Verschickt einen Bit
  void sendDataBit(bool bit);
  // Prüft ob Daten erfolgreich eingelesen wurden.
  bool checkDataReadingComplete();
};

#endif
