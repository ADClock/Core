
#ifndef _OUTPUTSTREAM_H_
#define _OUTPUTSTREAM_H_
#include "Arduino.h"
class OutputStream
{
public:
  OutputStream(uint8_t clockPin, uint8_t dataPin, uint8_t responsePin);

  // Verschickt einen Byte
  bool sendData(byte data);

  // Verschickt ein Array aus Daten. Cool oder?
  void sendDataArray(byte *arr);

private:
  // Verschickt einen Bit
  void sendDataBit(bool bit);
  // Prüft ob Daten erfolgreich eingelesen wurden.
  bool checkDataReadingComplete();
  uint8_t clockPin;
  uint8_t dataPin;
  uint8_t responsePin;
};

#endif
