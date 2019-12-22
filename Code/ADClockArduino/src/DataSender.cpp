#include "DataSender.h"

void DataSender::tick()
{
  switch (this->state)
  {
  case SenderState::IDLE:
    if (!this->buffer.is_empty())
    {
      this->send_next_bit();
    }
    break;

  case SenderState::WAIT_FOR_RESPONSE_ON:
    if (FastGPIO::Pin<OUT_RESPONSE>::isInputHigh())
    {
      this->receive_response_on();
    }
    else if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_ON)
    {
      timeout();
#ifdef DEBUG
      Serial.println("WAIT_FOR_RESPONSE_ON");
#endif
    }
    break;

  case SenderState::WAIT_FOR_RESPONSE_OFF:
    if (!FastGPIO::Pin<OUT_RESPONSE>::isInputHigh())
    {
      this->receive_response_off();
    }
    else if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_OFF)
    {
      timeout();
#ifdef DEBUG
      Serial.println("WAIT_FOR_RESPONSE_OFF");
#endif
    }
    break;

  default:
    break;
  }
}

bool DataSender::sending()
{
  return this->state != SenderState::IDLE;
}

bool DataSender::failed()
{
  return this->state == SenderState::FAILED;
}

void DataSender::reset()
{
  FastGPIO::Pin<OUT_CLOCK>::setOutputValueLow();
  this->state = SenderState::IDLE;
#ifdef DEBUG
  Serial.println("Resetting sender remaining bits = " + String(buffer.size()));
#endif
  this->buffer.clear();
  this->last_action = micros();
}

void DataSender::send_next_bit()
{
  FastGPIO::Pin<OUT_DATA>::setOutputValue(buffer.dequeue());
  FastGPIO::Pin<OUT_CLOCK>::setOutputValueHigh();
  this->state = SenderState::WAIT_FOR_RESPONSE_ON;
  this->last_action = micros();
}

void DataSender::receive_response_on()
{
  FastGPIO::Pin<OUT_CLOCK>::setOutputValueLow();
  this->state = SenderState::WAIT_FOR_RESPONSE_OFF;
  this->last_action = micros();
}

void DataSender::receive_response_off()
{
  if (!this->buffer.is_empty())
  {
    send_next_bit();
  }
  else
  {
    this->state = SenderState::IDLE;
  }
}

void DataSender::timeout()
{
  this->state = SenderState::FAILED;
}

unsigned long DataSender::time_waiting()
{
  return (unsigned long)(micros() - last_action);
}