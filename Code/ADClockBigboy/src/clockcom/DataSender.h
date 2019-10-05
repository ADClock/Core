#ifndef _DATA_SENDER_H_
#define _DATA_SENDER_H_

#include "BitBuffer.h"
#include "../Config.h"
#include "../Debug.h"
#include "../fastio/FastIO.h"

enum class SenderState
{
  // ready for sending data
  IDLE,
  // waiting for reciver reading bit
  WAIT_FOR_RESPONSE_ON,
  // waiting for reciver confirms successfull communication
  WAIT_FOR_RESPONSE_OFF,
  // Waiting for reset...
  FAILED
};

class DataSender
{
public:
  DataSender(BitBuffer &buffer) : buffer(buffer) { last_action = us_ticker_read(); };

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
  FastOut<SENDER_CLOCK> clock;
  FastOut<SENDER_DATA> data;
  FastIn<SENDER_RESPONSE> response;
};

#endif