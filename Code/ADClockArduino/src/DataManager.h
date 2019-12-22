
#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_
#include "Arduino.h"
#include "BitBuffer.h"
#include "DataSender.h"
#include "DataReceiver.h"
#include "MotorManager.h"

enum class CommandState
{
  IDLE,
  READING_COMMAND,
  READING_IMAGE,
  PIPEING,
  TESTING
};

class DataManager
{
public:
  DataManager(DataSender &sender, BitBuffer &out, DataReceiver &receiver, BitBuffer &in, MotorManager &moma);

  void tick();
  void process_incoming_data();
  void finish_transmission();
  void send_command(uint8_t command);
  void send_byte(uint8_t byte);

private:
  uint8_t read_byte();        // reads one byte from input buffer
  void set_current_command(); // Sets the current command based on input data

  void read_my_data();

  MotorData deserialize(uint8_t stream[4]);

  DataSender &sender;
  BitBuffer &out;
  DataReceiver &receiver;
  BitBuffer &in;
  MotorManager &moma;

  CommandState state = CommandState::IDLE;
  uint8_t current_command;
};
#endif
