#include "Arduino.h"
#include "Motor.h"

#ifndef _MYDATACOM_H_
#define _MYDATACOM_H_

#define IN_RESPONSE 2
#define IN_DATA 3
#define IN_CLOCK 4

#define OUT_RESPONSE 5
#define OUT_DATA 6
#define OUT_CLOCK 7

class MyDataCom
{

public:
  MyDataCom(const Motor &motor2);

  void checkForData();

  void sendData(uint8_t data);

private:
  Motor motor2;

  void recieveData();

  void recieveClockImage();

  void shiftInShiftOut();

  bool waitForSignalChange(uint8_t pin, int signal);
  int readBit();

  uint8_t shiftIn(uint8_t bitOrder);

  void shiftOut(uint8_t bitOrder, uint8_t val);

  /**
 * Versendet einen Bit, setzt anschließend die Clock und wartet darauf, dass das Bit gelesen wurde.
 * */
  void sendBit(int bit);
};

#endif
