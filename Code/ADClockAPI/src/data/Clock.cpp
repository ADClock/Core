#include "Clock.h"

void Clock::init()
{
  this->hour.init();
  this->minute.init();
}

const Hand &Clock::getMinuteHand()
{
  return this->minute;
}

const Hand &Clock::getHourHand()
{
  return this->hour;
}

JSONValue Clock::asJson()
{
  JSONValue v;
  v["hour"] = hour.asJson();
  v["minute"] = minute.asJson();
  return v;
}