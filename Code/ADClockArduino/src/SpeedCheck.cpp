#ifdef DEBUG
#include "SpeedCheck.h"

void SpeedCheck::start()
{
  this->_start = systemtime();
}

void SpeedCheck::stop()
{
  this->_end = systemtime();
  Serial.println(String(name) + " Call took " + String(duration()) + " µs");
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
  return micros();
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
    Serial.println("Complete Test took " + String(_sum) + " µs with " + String(pushes) + " pushes");
    return;
  }
  Serial.println("Test took " + String(duration()) + " µs");
}

void SpeedCheck::printBandwidth(size_t &bytes)
{
  Serial.println("Bandwith " + String(bandwidth(bytes)) + " µs");
}

#endif