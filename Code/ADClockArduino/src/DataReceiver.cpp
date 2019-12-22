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
    // The Interrupt Service Routine is taking care of the Clock and reads next Bit
    // After the timer expired we need to doublecheck the CLOCK-Signal. ISR could kicked in during this condition
    if (this->time_waiting() > RECEIVER_TIMEOUT_CLOCK_OFF && FastGPIO::Pin<IN_CLOCK>::isInputHigh())
    {
      timeout();
    }
    break;

  case ReceiverState::WAIT_FOR_NEXT_BIT:
    // The Interrupt Service Routine is taking care of the Clock and reads next Bit
    // After the timer expired we need to doublecheck the CLOCK-Signal. ISR could kicked in during this condition
    if (this->time_waiting() > RECEIVER_TIMEOUT_NEXT_DATA && !FastGPIO::Pin<IN_CLOCK>::isInputHigh())
    {
      this->state = ReceiverState::COMPLETE;
    }
    break;

  default:
    break;
  }
}

bool DataReceiver::receiving()
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

void DataReceiver::receive_clock_off()
{
  FastGPIO::Pin<IN_RESPONSE>::setOutputValueLow();
  this->state = ReceiverState::WAIT_FOR_NEXT_BIT;
  this->last_action = micros();
}

void DataReceiver::timeout()
{
  this->state = ReceiverState::FAILED;
#if DEBUG
  Serial.println("Failed receiving after " + String(this->buffer.size()) + " bits.");
#endif
}

unsigned long DataReceiver::time_waiting()
{
  return (unsigned long)(micros() - last_action);
}

// sends response for reviced bit
void DataReceiver::confirm()
{
  while (this->state == ReceiverState::WAIT_FOR_CLOCK_OFF)
    tick();
}