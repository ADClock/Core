#ifndef _ClockOutputStream_H_
#define _ClockOutputStream_H_
#include "mbed.h"
#include "../Debug.h"
#include "../Config.h"
#include "../fastio/FastIO.h"
class ClockOutputStream
{
public:
  ClockOutputStream();

  // Verschickt einen Byte
  bool sendByte(const uint8_t &data);

  // Verschickt ein Array aus Bytes. Cool oder?
  bool sendByteArray(uint8_t arr[], size_t &length);

  bool sendSerializedHand(uint8_t arr[]);

  void printResult();

private:
  // Verschickt einen Bit
  void sendDataBit(bool bit);
  // Prüft ob Daten erfolgreich eingelesen wurden.
  bool checkDataReadingComplete();

  FastOut<OUT_DATA> pin_data;
  FastOut<OUT_CLOCK> pin_clock;
  FastIn<OUT_RESPONSE> pin_response;
};

#endif
