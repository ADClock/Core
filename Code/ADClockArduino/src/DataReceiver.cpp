#include "DataReceiver.h"

void DataReceiver::tick()
{
  unsigned long waiting = this->time_waiting();
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
    if (waiting > 4000000)
      break; // ISR triggert while getting time_waiting
    if (waiting > RECEIVER_TIMEOUT_CLOCK_OFF && FastGPIO::Pin<IN_CLOCK>::isInputHigh() && state == ReceiverState::WAIT_FOR_CLOCK_OFF)
    {
      timeout();
      Serial.println("WAIT_FOR_CLOCK_OFF");
    }
    break;

  case ReceiverState::WAIT_FOR_NEXT_BIT:
    // The Interrupt Service Routine is taking care of the Clock and reads next Bit
    // After the timer expired we need to doublecheck the CLOCK-Signal. ISR could kicked in during this condition
    if (waiting > 4000000)
      break; // ISR triggert while getting time_waiting
    if (waiting > RECEIVER_TIMEOUT_NEXT_DATA && !FastGPIO::Pin<IN_CLOCK>::isInputHigh())
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
#ifdef DEBUG
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