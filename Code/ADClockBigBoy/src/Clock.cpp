#include "Clock.h"

bool Clock::hasPendingMove()
{
  return this->hour.hasPendingMove() || this->minute.hasPendingMove();
}

void Clock::move()
{
  this->hour.move();
  this->minute.move();
}

u_int8_t *Clock::nextStepAsImage()
{
  static u_int8_t image[8];
  u_int8_t *hour = this->hour.nextStepAsImage();

  for (int i = 0; i < 4; i++)
  {
    // Stepper 1: Hour
    image[i] = hour[i];
  }

  u_int8_t *minute = this->minute.nextStepAsImage();

  for (int i = 0; i < 4; i++)
  {
    // Stepper 1: Hour
    image[4 + i] = minute[i];
  }

  return image;
}

long Clock::getCalculateMoveTime()
{
  return max(this->hour.getCalculateMoveTime(), this->minute.getCalculateMoveTime());
}

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

MbedJSONValue Clock::asJson()
{
  MbedJSONValue v;
  v["hour"] = hour.asJson();
  v["minute"] = minute.asJson();
  return v;
}