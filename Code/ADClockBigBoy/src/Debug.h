
#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "mbed.h"

namespace Debug
{
extern Serial serial;

static inline void print(const char *c)
{
  serial.printf(c);
}

static inline void println(const char *c)
{
  serial.printf("%s\n", c);
}

static inline void printf(const char *c, ...)
{
  serial.printf(c);
}
}; // namespace Debug

#endif