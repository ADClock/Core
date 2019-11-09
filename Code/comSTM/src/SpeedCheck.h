#ifndef _SPEEDCHECK_H_
#define _SPEEDCHECK_H_

#include "mbed.h"
#include "Debug.h"

class SpeedCheck
{

public:
  SpeedCheck(char *name) : name(name){};

  void start();
  void stop();
  void pushback();

  void printResult();
  void printBandwidth(size_t &bytes);

private:
  unsigned long bandwidth(size_t &bytes);
  unsigned long duration();

  unsigned long systemtime();

  unsigned long _start;
  unsigned long _end;
  unsigned long _sum;
  char *name;
  size_t pushes;
};

#endif