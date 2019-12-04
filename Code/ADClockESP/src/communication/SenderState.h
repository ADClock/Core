#ifndef _SENDER_STATE_H_
#define _SENDER_STATE_H_

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

#endif