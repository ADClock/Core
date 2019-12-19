#include "DataReceiver.h"

void DataReceiver::tick()
{
  switch (this->state)
  {
  case ReceiverState::IDLE:
    if (FastGPIO::Pin<IN_CLOCK>::isInputHigh())
    {
      this->read_next_bit();
    }
    break;

  case ReceiverState::WAIT_FOR_CLOCK_OFF:
    if (!FastGPIO::Pin<IN_CLOCK>::isInputHigh())
    {
      // Der interrupt sollte sich hierdrum kümmern
      //this->recive_clock_off();
    }
    else if (this->time_waiting() > RECEIVER_TIMEOUT_CLOCK_OFF)
    {
      Serial.println("Clock stil on after " + String(this->time_waiting()));
      timeout();
    }
    break;

  case ReceiverState::WAIT_FOR_NEXT_BIT:
    if (FastGPIO::Pin<IN_CLOCK>::isInputHigh())
    {
      // Der interrupt sollte sich hierdrum kümmern
      //  this->read_next_bit();
    }
    else if (this->time_waiting() > RECEIVER_TIMEOUT_NEXT_DATA)
    {
      this->state = ReceiverState::COMPLETE;
    }
    break;

  default:
    break;
  }
}

bool DataReceiver::recieving()
{
  return this->state != ReceiverState::IDLE;
}

bool DataReceiver::failed()
{
  return this->state == ReceiverState::FAILED;
}

bool DataReceiver::complete()
{
  return this->state == ReceiverState::COMPLETE;
}

void DataReceiver::reset()
{
  FastGPIO::Pin<IN_RESPONSE>::setOutputValueLow();
  this->state = ReceiverState::IDLE;
  this->buffer.clear();
  this->last_action = micros();
}

void DataReceiver::read_next_bit()
{
  this->buffer.enqueue(FastGPIO::Pin<IN_DATA>::isInputHigh());
  FastGPIO::Pin<IN_RESPONSE>::setOutputValueHigh();
  this->state = ReceiverState::WAIT_FOR_CLOCK_OFF;
  this->last_action = micros();
}

void DataReceiver::recive_clock_off()
{
  FastGPIO::Pin<IN_RESPONSE>::setOutputValueLow();
  this->state = ReceiverState::WAIT_FOR_NEXT_BIT;
  this->last_action = micros();
}

void DataReceiver::timeout()
{
  this->state = ReceiverState::FAILED;
  Serial.println("Failed receiving after " + String(this->buffer.size()) + " bits.");
}

unsigned long DataReceiver::time_waiting()
{
  auto current_time = micros();
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
void DataReceiver::confirm()
{
  while (this->state == ReceiverState::WAIT_FOR_CLOCK_OFF)
    tick();
}