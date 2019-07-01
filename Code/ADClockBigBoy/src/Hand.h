
#ifndef _HAND_H_
#define _HAND_H_
#include "mbed.h"
#include "MbedJSONValue.h"
#include <cstdlib>
using namespace std;

#define MIN_STEP_DELAY 4000
#define STEPS_FOR_CIRCLE 1705
#define STEPS_PER_DEGREE STEPS_FOR_CIRCLE / 360.

#define RIGHT true
#define LEFT false

class Hand
{
public:
  void init();

  void setNextPositionSteps(size_t steps);
  void setNextPositionDegree(size_t degree);
  void setNextDelayBetweenSteps(size_t delay);
  void setNextWaitSteps(size_t steps);
  void setDirection(bool direction);

  void setSimultaneouslyMove(size_t steps);

  void move();

  bool hasPendingMove();

  long getCalculateMoveTime();

  u_int8_t *nextStepAsImage();

  size_t getPendingSteps();

  MbedJSONValue asJson();
  void update(MbedJSONValue &json);

private:
  size_t position;

  size_t next_position;

  size_t next_waitSteps;

  size_t next_stepDelay;

  bool next_direction;
};
#endif
