#include "NTPTime.h"

void NTPTime::load_time()
{
  if (!getLocalTime(&this->timedata))
  {
    this->last_update = 0;
  }
  else
  {
    this->last_update = micros();
    Serial.println("Current time loaded. Its " + String(get_hour()) + ":" + String(get_minute()));
  }
}

bool NTPTime::is_time_set()
{
  return this->last_update + 1000 * 1000 * 10 > micros();
}

size_t NTPTime::get_hour()
{
  return this->timedata.tm_hour;
}
size_t NTPTime::get_minute()
{
  return this->timedata.tm_min;
}