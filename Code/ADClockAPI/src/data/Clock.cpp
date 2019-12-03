#include "Clock.h"

void Clock::init()
{
  this->hour.init();
  this->minute.init();
}

JSONValue Clock::asJson()
{
  JSONValue v;
  v["hour"] = hour.asJson();
  v["minute"] = minute.asJson();
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