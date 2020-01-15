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
    if (digitalRead(OUT_RESPONSE))
    {
      this->receive_response_on();
    }
    else if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_ON)
    {
      timeout();
      Serial.println("WAIT_FOR_RESPONSE_ON");
    }
    break;

  case SenderState::WAIT_FOR_RESPONSE_OFF:
    if (!digitalRead(OUT_RESPONSE))
    {
      this->receive_response_off();
    }
    else if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_OFF)
    {
      timeout();
      Serial.println("WAIT_FOR_RESPONSE_OFF");
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
  digitalWrite(OUT_CLOCK, 0);
  this->state = SenderState::IDLE;
  Serial.println("Resetting sender remaining bits = " + String(buffer.size()));
  this->buffer.clear();
  this->last_action = micros();
}

void DataSender::send_next_bit()
{
  digitalWrite(OUT_DATA, buffer.dequeue());
  digitalWrite(OUT_CLOCK, 1);
  this->state = SenderState::WAIT_FOR_RESPONSE_ON;
  this->last_action = micros();
  // Serial.println("Sending bit..");
}

void DataSender::receive_response_on()
{
  digitalWrite(OUT_CLOCK, 0);
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
  Serial.println("Failed sending.");
}

unsigned long DataSender::time_waiting()
{
  return (unsigned long)(micros() - last_action);
}