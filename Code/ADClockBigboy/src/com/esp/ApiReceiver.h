#ifndef _API_RECEIVER_H_
#define _API_RECEIVER_H_

#include "../BitBuffer.h"
#include "../../Config.h"
#include "../../Debug.h"
#include "../../fastio/FastIO.h"
#include "../ReceiverState.h"

class ApiReceiver
{
public:
  ApiReceiver(BitBuffer &buffer) : buffer(buffer)
  {
    last_action = us_ticker_read();
  };

  void tick();
  void confirm();

  bool recieving(); // true -> currently recieving
  bool failed();    // true -> last transmission failed
  bool complete();  // true -> no more data recived
  void reset();

  // Returns the time since last action
  unsigned long time_waiting();

private:
  // IDLE -> WAIT_FOR_CLOCK_OFF
  void read_next_bit();
  // WAIT_FOR_CLOCK_OFF -> WAIT_FOR_NEXT_BIT
  void recive_clock_off();
  // x -> FAILED
  void timeout();

  ReceiverState state = ReceiverState::IDLE;
  unsigned long last_action;
  BitBuffer &buffer;

  // Pins
  FastIn<API_RECEIVER_CLOCK> clock;
  FastIn<API_RECEIVER_DATA> data;
  FastOut<API_RECEIVER_RESPONSE> response;
};

#endif