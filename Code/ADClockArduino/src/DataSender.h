#ifndef _DATA_SENDER_H_
#define _DATA_SENDER_H_

#include "BitBuffer.h"
#include "Config.h"
#include "FastGPIO.h"

enum class SenderState
{
  // ready for sending data
  IDLE,
  // waiting for receiver reading bit
  WAIT_FOR_RESPONSE_ON,
  // waiting for receiver confirms successfull communication
  WAIT_FOR_RESPONSE_OFF,
  // If it sends faster then filling the buffer.. (could only be short time of period)
  WAIT_FOR_DATA,
  // Waiting for reset...
  FAILED
};

class DataSender
{
public:
  DataSender(BitBuffer &buffer) : buffer(buffer)
  {
    last_action = micros();
    pinMode(OUT_DATA, OUTPUT);
    pinMode(OUT_CLOCK, OUTPUT);
    digitalWrite(OUT_CLOCK, LOW);
  };

  void tick();

  bool sending(); // true -> currently sending
  bool failed();  // true -> last transmission failed
  void reset();

  // WAIT_FOR_RESPONSE_ON -> WAIT_FOR_RESPONSE_OFF
  void receive_response_on();
  // WAIT_FOR_RESPONSE_OFF -> IDLE or sendNextBit();
  void receive_response_off();
  // Returns the time since last action
  unsigned long time_waiting();

private:
  // IDLE -> WAIT_FOR_RESPONSE_ON
  void send_next_bit();
  // x -> FAILED
  void timeout();

  SenderState state = SenderState::IDLE;
  unsigned long last_action;
  BitBuffer &buffer;
};

#endif