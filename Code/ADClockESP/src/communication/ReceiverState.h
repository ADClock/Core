#ifndef _RECEIVER_STATE_H_
#define _RECEIVER_STATE_H_

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

#endif