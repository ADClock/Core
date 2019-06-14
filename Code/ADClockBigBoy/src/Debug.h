
#ifndef _DEBUG_H_
#define _DEBUG_H_
#include "mbed.h"

namespace Debug
{
Serial serial(SERIAL_TX, SERIAL_RX, 9600);

static inline void print(const char *c)
{
  serial.printf(c);
}

static inline void println(const char *c)
{
  serial.printf("%s\n", c);
}
} // namespace Debug

#endif