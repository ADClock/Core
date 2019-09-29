#include "Clock.h"

u_int8_t *Clock::serialize()
{
  static u_int8_t image[8];
  u_int8_t *hour = this->hour.serialize();

  for (int i = 0; i < 4; i++)
  {
    // Stepper 1: Hour
    image[i] = hour[i];
  }

  u_int8_t *minute = this->minute.serialize();

  for (int i = 0; i < 4; i++)
  {
    // Stepper 2: Minute
    image[4 + i] = minute[i];
  }

  return image;
}

void Clock::init()
{
  this->hour.init();
  this->minute.init();
}

MbedJSONValue Clock::asJson()
{
  MbedJSONValue v;
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