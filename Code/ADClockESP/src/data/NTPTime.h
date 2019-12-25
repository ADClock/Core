#ifndef _NTPTIME_H_
#define _NTPTIME_H_

#include "Arduino.h"
#include "time.h"

class NTPTime
{
public:
  NTPTime(){};
  void load_time();
  bool is_time_set();
  size_t get_hour();
  size_t get_minute();

private:
  bool is_configured;
  unsigned long last_update;
  struct tm timedata;
};

#endif