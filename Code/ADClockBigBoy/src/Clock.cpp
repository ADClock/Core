#include "Clock.h"

bool Clock::hasPendingMove()
{
  return this->hourPosition != this->next_hourPosition || this->minutePosition != this->next_minutePosition;
}

void Clock::move()
{
  this->hourPosition = this->next_hourPosition;
  this->minutePosition = this->next_minutePosition;
}

void Clock::setNextHourPosition(size_t degree)
{
  this->next_hourPosition = degree;
}

void Clock::setNextMinutePosition(size_t degree)
{
  this->next_minutePosition = degree;
}

u_int8_t *Clock::nextStepAsImage()
{
  static u_int8_t image[8];
  // Stepper 1: Hour
  image[0] = (next_hourPosition >> 8) & 0xFF;
  image[1] = next_hourPosition & 0xFF;

  // Stepper 2: Minute
  image[4] = (next_minutePosition >> 8) & 0xFF;
  image[5] = (next_minutePosition & 0xFF);

  return image;
}

long Clock::getCalculateMoveTime()
{
  return max(abs(static_cast<long>(next_hourPosition - hourPosition)) * MIN_STEP_DELAY,
             abs(static_cast<long>(next_minutePosition - minutePosition)) * MIN_STEP_DELAY);
}

void Clock::init()
{
  this->hourPosition = 0;
  this->next_hourPosition = 0;
  this->minutePosition = 0;
  this->next_minutePosition = 0;
}