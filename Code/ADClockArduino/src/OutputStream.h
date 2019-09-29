
#ifndef _OUTPUTSTREAM_H_
#define _OUTPUTSTREAM_H_

#include "Arduino.h"
#include "Config.h"
#include "FastGPIO.h"

class OutputStream
{
public:
  OutputStream();

  // Verschickt einen Byte
  bool sendData(byte data) const;

  // Verschickt ein Array aus Daten. Cool oder?
  bool sendDataArray(byte arr[], size_t length) const;

private:
  // Verschickt einen Bit
  void sendDataBit(bool bit) const;
  // Prüft ob Daten erfolgreich eingelesen wurden.
  bool checkDataReadingComplete() const;
};

#endif
