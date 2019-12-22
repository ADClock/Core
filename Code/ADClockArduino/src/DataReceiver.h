#ifndef _DATA_RECEIVER_H_
#define _DATA_RECEIVER_H_

#include "BitBuffer.h"
#include "Config.h"
#include "FastGPIO.h"

enum class ReceiverState
{
  // Waiting for first bit arive
  IDLE,
  // waiting for sender turning clock off
  WAIT_FOR_CLOCK_OFF,
  // waiting for next bit arive
  WAIT_FOR_NEXT_BIT,
  COMPLETE,
  // failed. woops.
  FAILED
};

class DataReceiver
{
public:
  DataReceiver(BitBuffer &buffer) : buffer(buffer)
  {
    last_action = micros();
    pinMode(IN_RESPONSE, OUTPUT);
    digitalWrite(IN_RESPONSE, LOW);
  };

  void tick();
  void confirm();

  bool receiving(); // true -> currently receiving
  bool failed();    // true -> last transmission failed
  bool complete();  // true -> no more data recived

  void reset();

  // Returns the time since last action
  unsigned long time_waiting();

  // IDLE -> WAIT_FOR_CLOCK_OFF
  void read_next_bit();
  // WAIT_FOR_CLOCK_OFF -> WAIT_FOR_NEXT_BIT
  void receive_clock_off();

private:
  // Change State from x -> FAILED
  void timeout();

  ReceiverState state = ReceiverState::IDLE;
  unsigned long last_action;
  BitBuffer &buffer;
};

#endif