#include "ApiReceiver.h"

void ApiReceiver::tick()
{
  switch (this->state)
  {
  case ReceiverState::IDLE:
    if (clock)
    {
      this->read_next_bit();
    }
    break;

  case ReceiverState::WAIT_FOR_CLOCK_OFF:
    if (!clock)
    {
      this->recive_clock_off();
    }
    else if (this->time_waiting() > 100000) // TODO Konstante
    {
      Debug::serial.printf("Clock stil on after %d", this->time_waiting());
      timeout();
    }
    break;

  case ReceiverState::WAIT_FOR_NEXT_BIT:
    if (clock)
    {
      this->read_next_bit();
    }
    else if (this->time_waiting() > 100000) // TODO Konstante
    {
      this->state = ReceiverState::COMPLETE;
    }
    break;

  default:
    break;
  }
}

bool ApiReceiver::recieving()
{
  return this->state != ReceiverState::IDLE;
}

bool ApiReceiver::failed()
{
  return this->state == ReceiverState::FAILED;
}

bool ApiReceiver::complete()
{
  return this->state == ReceiverState::COMPLETE;
}

void ApiReceiver::reset()
{
  response = 0;
  this->state = ReceiverState::IDLE;
  this->buffer.clear();
  this->last_action = us_ticker_read();
}

void ApiReceiver::read_next_bit()
{
  this->buffer.enqueue(data);
  response = 1;
  this->state = ReceiverState::WAIT_FOR_CLOCK_OFF;
  this->last_action = us_ticker_read();
}

void ApiReceiver::recive_clock_off()
{
  response = 0;
  this->state = ReceiverState::WAIT_FOR_NEXT_BIT;
  this->last_action = us_ticker_read();
}

void ApiReceiver::timeout()
{
  this->state = ReceiverState::FAILED;
  Debug::serial.printf("Failed receiving after %d bits.", this->buffer.size());
}

unsigned long ApiReceiver::time_waiting()
{
  auto current_time = us_ticker_read();
  if (current_time < last_action)
  {
    return current_time + 0; // TODO Hier fehlt noch die Zeit von MAX_VALUE - last_action
  }
  else
  {
    return current_time - last_action;
  }
}

// sends response for reviced bit
void ApiReceiver::confirm()
{
  while (this->state == ReceiverState::WAIT_FOR_CLOCK_OFF)
    tick();
}