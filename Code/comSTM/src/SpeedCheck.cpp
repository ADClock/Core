#include "SpeedCheck.h"

void SpeedCheck::start()
{
  this->_start = systemtime();
}

void SpeedCheck::stop()
{
  this->_end = systemtime();
  Debug::serial.printf("%s Call took %u µs\n", name, duration());
}

void SpeedCheck::pushback()
{
  this->pushes++;
  this->_sum += duration();
}

unsigned long SpeedCheck::duration()
{
  return this->_end - this->_start;
}

unsigned long SpeedCheck::systemtime()
{
  return us_ticker_read();
}

unsigned long SpeedCheck::bandwidth(size_t &bytes)
{
  // TODO Bandbreite in bytes pro s ausrechnen
  return 1;
}

void SpeedCheck::printResult()
{
  if (this->_sum != 0)
  {
    Debug::serial.printf("Complete Test took %u µs with %u pushes\n", _sum, pushes);
    return;
  }
  Debug::serial.printf("Test took %u µs\n", duration());
}

void SpeedCheck::printBandwidth(size_t &bytes)
{
  Debug::serial.printf("Bandwith %u µs\n", bandwidth(bytes));
}