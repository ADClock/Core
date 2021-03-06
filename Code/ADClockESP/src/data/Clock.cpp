#include "Clock.h"

void Clock::init()
{
  this->hour.init();
  this->minute.init();
}

JsonDocument Clock::asJson()
{
  DynamicJsonDocument v(1024);
  v["h"] = hour.asJson();
  v["m"] = minute.asJson();
  return v;
}

void Clock::update(Clock &clock)
{
  this->hour.update(clock.hour);
  this->minute.update(clock.minute);
}

bool Clock::equals(Clock &clock)
{
  return this->hour.equals(clock.hour) && this->minute.equals(clock.minute);
}