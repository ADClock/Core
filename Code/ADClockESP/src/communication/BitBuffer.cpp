#include "BitBuffer.h"

BitBuffer::BitBuffer()
{
}

bool BitBuffer::is_full()
{
  return tail == (head - 1) % BUFFER_SIZE;
}
bool BitBuffer::is_empty()
{
  return tail == head;
}

void BitBuffer::clear()
{
  tail = head;
}

// TODO Das klappt irgendwie nicht glaube ich
size_t BitBuffer::size()
{
  if (tail >= head)
    return tail - head;
  return BUFFER_SIZE - head - tail;
}

size_t BitBuffer::free_space()
{
  return BUFFER_SIZE - size();
}

bool BitBuffer::front()
{
  return buffer[head];
}

void BitBuffer::enqueue(bool item)
{
  buffer[tail] = item;
  tail = (tail + 1) % BUFFER_SIZE;
}

bool BitBuffer::dequeue()
{
  auto bit = buffer[head];
  head = (head + 1) % BUFFER_SIZE;
  return bit;
}
