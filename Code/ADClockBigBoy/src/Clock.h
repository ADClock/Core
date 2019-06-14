
#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "mbed.h"
#include <cstdlib>
using namespace std;

#define MIN_STEP_DELAY 4
#define STEPS_FOR_CIRCLE 2050

class Clock
{
public:
  void init();
  void setNextMinutePosition(size_t degree);
  void setNextHourPosition(size_t degree);

  void move();

  bool hasPendingMove();

  long getCalculateMoveTime();

  u_int8_t *nextStepAsImage();

private:
  size_t minutePosition;
  size_t hourPosition;

  size_t next_minutePosition;
  size_t next_hourPosition;
};
#endif
