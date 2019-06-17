
#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "mbed.h"
#include "Hand.h"
#include <cstdlib>
using namespace std;

class Clock
{
public:
  void init();

  const Hand &getMinuteHand();
  const Hand &getHourHand();

  void move();

  bool hasPendingMove();

  long getCalculateMoveTime();

  u_int8_t *nextStepAsImage();

  Hand minute;
  Hand hour;

private:
};
#endif
