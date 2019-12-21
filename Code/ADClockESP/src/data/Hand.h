
#ifndef _HAND_H_
#define _HAND_H_
#include "Arduino.h"
#include "ArduinoJson.h"
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

  uint8_t *serialize();

  size_t getPosition();
  size_t getWaitSteps();
  size_t getStepDelay();
  bool getDirection();

  JsonDocument asJson();
  void update(JsonDocument &json);
  void update(Hand &hand);

  bool equals(Hand &hand);

private:
  size_t position;

  size_t waitSteps;

  size_t stepDelay;

  bool direction;
};
#endif
