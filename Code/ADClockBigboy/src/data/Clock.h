
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

  u_int8_t *serialize();

  MbedJSONValue asJson();
  void update(MbedJSONValue &json);
  void update(Clock &clock);

  bool equals(Clock &clock);

  Hand minute;
  Hand hour;

private:
};
#endif
