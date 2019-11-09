#ifndef _API_SENDER_H_
#define _API_SENDER_H_

#include "../BitBuffer.h"
#include "../../Config.h"
#include "../../Debug.h"
#include "../../fastio/FastIO.h"
#include "../SenderState.h"

class ApiSender
{
public:
  ApiSender(BitBuffer &buffer) : buffer(buffer) { last_action = us_ticker_read(); };

  void tick();

  bool sending(); // true -> currently sending
  bool failed();  // true -> last transmission failed
  void reset();

  // Returns the time since last action
  uint32_t time_waiting();

private:
  // IDLE -> WAIT_FOR_RESPONSE_ON
  void send_next_bit();
  // WAIT_FOR_RESPONSE_ON -> WAIT_FOR_RESPONSE_OFF
  void recive_response_on();
  // WAIT_FOR_RESPONSE_OFF -> IDLE or sendNextBit();
  void recive_response_off();
  // x -> FAILED
  void timeout();

  SenderState state = SenderState::IDLE;
  uint32_t last_action;
  BitBuffer &buffer;

  // Pins
  FastOut<API_SENDER_CLOCK> clock;
  FastOut<API_SENDER_DATA> data;
  FastIn<API_SENDER_RESPONSE> response;
};

#endif