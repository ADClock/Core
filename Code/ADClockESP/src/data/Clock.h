
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

  JSONValue asJson();
  void update(JSONValue &json);
  void update(Clock &clock);

  bool equals(Clock &clock);

  Hand minute;
  Hand hour;

private:
};
#endif
