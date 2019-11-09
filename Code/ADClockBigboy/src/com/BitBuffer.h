#ifndef _BIT_BUFFER_H_
#define _BIT_BUFFER_H_

#include "mbed.h"

#define BUFFER_SIZE 1024

class BitBuffer
{

public:
  BitBuffer();

  bool is_full();
  bool is_empty();
  size_t size();
  size_t free_space();

  void enqueue(bool b);
  bool dequeue();
  bool front();

  void clear();

private:
  bool buffer[BUFFER_SIZE];
  size_t head = 0;
  size_t tail = 0;
};

#endif