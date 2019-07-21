
#ifndef _CLOCK_H_
#define _CLOCK_H_
#include "Arduino.h"
#include "Hand.h"
#include <cstdlib>
using namespace std;

class Clock
{
public:
  void init();

  const Hand &getMinuteHand();
  const Hand &getHourHand();

  JSONValue asJson();
  void update(JSONValue &json);

  Hand minute;
  Hand hour;

private:
};
#endif
