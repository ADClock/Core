
#ifndef _HAND_H_
#define _HAND_H_
#include "mbed.h"
#include "../MbedJSONValue.h"
#include "../Config.h"
#include <cstdlib>
using namespace std;

class Hand
{
public:
  Hand() { init(); }

  void init();

  void setPositionSteps(size_t steps);
  void setPositionDegree(size_t degree);
  void setDelayBetweenSteps(size_t delay);
  void setWaitSteps(size_t steps);
  void setDirection(bool direction);

  void setSimultaneouslyMove(size_t steps);

  u_int8_t *serialize();

  size_t getPosition();
  size_t getWaitSteps();
  size_t getStepDelay();
  bool getDirection();

  MbedJSONValue asJson();
  void update(MbedJSONValue &json);
  void update(Hand &hand);

  bool equals(Hand &hand);

private:
  size_t position;

  size_t waitSteps;

  size_t stepDelay;

  bool direction;
};
#endif
