
#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "Arduino.h"

#define MIN_STEP_DELAY 4
#define STEPS_FOR_CIRCLE 2050

class Clock
{
public:
  void setNextMinutePosition(size_t degree);
  void setNextHourPosition(size_t degree);

  void move();

  bool hasPendingMove();

  long getCalculateMoveTime();

  byte *nextStepAsImage();

private:
  size_t minutePosition;
  size_t hourPosition;

  size_t next_minutePosition;
  size_t next_hourPosition;
};
#endif
