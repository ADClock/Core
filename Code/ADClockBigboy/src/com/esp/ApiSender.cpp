#include "ApiSender.h"

void ApiSender::tick()
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
    if (this->response.read())
    {
      this->recive_response_on();
    }
    else if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_ON)
    {
      timeout();
      Debug::println("WAIT_FOR_RESPONSE_ON");
    }
    break;

  case SenderState::WAIT_FOR_RESPONSE_OFF:
    if (!this->response.read())
    {
      this->recive_response_off();
    }
    else if (this->time_waiting() > SENDER_TIMEOUT_RESPONSE_OFF)
    {
      timeout();
      Debug::println("WAIT_FOR_RESPONSE_OFF");
    }
    break;

  default:
    break;
  }
}

bool ApiSender::sending()
{
  return this->state != SenderState::IDLE;
}

bool ApiSender::failed()
{
  return this->state == SenderState::FAILED;
}

void ApiSender::reset()
{
  this->clock = 0;
  this->state = SenderState::IDLE;
  this->buffer.clear();
  this->last_action = us_ticker_read();
}

void ApiSender::send_next_bit()
{
  this->data = buffer.dequeue();
  this->clock = 1;
  this->state = SenderState::WAIT_FOR_RESPONSE_ON;
  this->last_action = us_ticker_read();
  // Debug::println("Sending bit..");
}

void ApiSender::recive_response_on()
{
  this->clock = 0;
  this->state = SenderState::WAIT_FOR_RESPONSE_OFF;
  this->last_action = us_ticker_read();
}

void ApiSender::recive_response_off()
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

void ApiSender::timeout()
{
  this->state = SenderState::FAILED;
  Debug::println("Failed.");
}

u_int32_t ApiSender::time_waiting()
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