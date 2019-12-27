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
    // The Interrupt Service Routine is taking care of the Response
    // After the timer expired we need to doublecheck the Response-Signal. ISR could kicked in during this condition
    if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_ON && !FastGPIO::Pin<OUT_RESPONSE>::isInputHigh())
    {
#ifdef DEBUG
      Serial.println("WAIT_FOR_RESPONSE_ON " + String(this->time_waiting()) + " " + String(FastGPIO::Pin<OUT_RESPONSE>::isInputHigh()) + String(static_cast<int>(state)));
#endif
      timeout();
    }
    break;

  case SenderState::WAIT_FOR_RESPONSE_OFF:
    // The Interrupt Service Routine is taking care of the Response
    // After the timer expired we need to doublecheck the Response-Signal. ISR could kicked in during this condition
    if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_OFF && FastGPIO::Pin<OUT_RESPONSE>::isInputHigh())
    {
      timeout();
#ifdef DEBUG
      Serial.println("WAIT_FOR_RESPONSE_OFF " + String(this->time_waiting()));
#endif
    }
    break;

  case SenderState::WAIT_FOR_DATA:
    if (!this->buffer.is_empty())
    {
      this->send_next_bit();
    }
    else if (this->time_waiting() > SENDER_WAIT_FOR_DATA_IN_BUFFER)
    {
      this->state = SenderState::IDLE;
    }

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
    // Maybe we are sending faster then receiving. Waiting short time for next data in buffer
    this->state = SenderState::WAIT_FOR_DATA;
    this->last_action = micros();
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