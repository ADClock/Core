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
  this->hourPosition = degree;
}

void Clock::setNextMinutePosition(size_t degree)
{
  this->minutePosition = degree;
}

byte *Clock::nextStepAsImage()
{
  byte image[8];
  // TODO Werte setzen
  return image;
}