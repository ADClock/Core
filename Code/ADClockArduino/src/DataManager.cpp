#include "DataManager.h"

DataManager::DataManager(DataSender &sender, BitBuffer &out, DataReceiver &receiver, BitBuffer &in, MotorManager &moma) : sender(sender), out(out), receiver(receiver), in(in), moma(moma)
{
}

void DataManager::tick()
{
  this->receiver.tick();
  this->sender.tick();

  this->process_incoming_data();

  if (this->receiver.complete())
  {
    this->process_incoming_data();
#ifndef IS_LAST_CLOCK
    this->state = CommandState::WAIT_FOR_DATA_SENDING_DONE;
#else
    this->state = CommandState::IDLE;
#endif
    this->receiver.reset();
  }

  if (this->receiver.failed())
  {
#ifndef IS_LAST_CLOCK
    this->state = CommandState::WAIT_FOR_DATA_SENDING_DONE;
#else
    this->state = CommandState::IDLE;
#endif
    this->receiver.reset();
  }
}

void DataManager::process_incoming_data()
{
  switch (state)
  {
  case CommandState::IDLE:
    if (!this->in.is_empty())
    {
      this->state = CommandState::READING_COMMAND;
    }
    /*else
    {
      // Wenn kein Command ankommt können wir ja auch alles senden
      if (!this->out.is_empty())
      {
        this->sender.tick();

        while (this->sender.sending() && !this->sender.failed())
          this->sender.tick();
      }
    }*/
    break;

  case CommandState::READING_COMMAND:
    if (this->in.size() >= 8)
    {
      set_current_command();
    }
    break;

  case CommandState::READING_IMAGE:
    if (this->in.size() >= 8 * 8)
    {
      this->state = CommandState::PIPEING;
      read_my_data();
    }
    break;

  case CommandState::WAIT_FOR_DATA_SENDING_DONE:
    if (this->sender.failed())
      this->sender.reset();
    if (!this->sender.sending())
    {
      this->state = CommandState::IDLE;
      // Serial.println("Command complete");
    }

  case CommandState::PIPEING:
    while (!this->in.is_empty())
    {
#ifndef IS_LAST_CLOCK
      this->out.enqueue(this->in.dequeue());
#else
      this->in.clear();
#endif
    }
  case CommandState::TESTING:
    in.clear(); // Dafür sorgen, dass Buffer nicht überläuft

  default:
    break;
  }
}

void DataManager::set_current_command()
{
  auto command = read_byte();
  this->receiver.confirm();

#ifdef DEBUG
// Serial.println("Processing command.. cmd = " + String(command));
#endif
  switch (command)
  {
  case 0x01:
    send_command(0x01);
    this->state = CommandState::IDLE;
    finish_transmission();
    moma.calibrate();
    break;

  case 0x02:
    send_command(0x02);
    this->state = CommandState::READING_IMAGE;
    break;

  case 0x03:
    this->state = CommandState::TESTING;
    break;

  default:
    Serial.println("Invalid command");
    // received invalid command
    this->receiver.reset();
    this->state = CommandState::IDLE;
    break;
  }
}

void DataManager::send_command(uint8_t command)
{
  while (sender.sending() || sender.time_waiting() < DELAY_BETWEEN_COMMANDS)
  {
    this->sender.tick();
    if (sender.failed())
      sender.reset();

    // Also Tick receiver (otherwise there could happen a timeout)
    this->receiver.tick();
  }
  send_byte(command);
}

void DataManager::send_byte(uint8_t byte)
{
#ifdef IS_LAST_CLOCK
  return;
#endif
  for (uint8_t i = 0; i < 8; i++)
    this->out.enqueue(!!(byte & (1 << i)));
}

uint8_t DataManager::read_byte()
{
  uint8_t byte = 0;
  for (uint8_t i = 0; i < 8; ++i)
    byte |= this->in.dequeue() << i;
  return byte;
}

void DataManager::finish_transmission()
{
  this->tick();
  while (this->state != CommandState::IDLE || !this->out.is_empty() || this->sender.sending())
    this->tick();
}

void DataManager::read_my_data()
{
  static uint8_t input[8];
  // 8 Byte lesen (4 je Motor)
  // -- Position  (16 Bit)
  // -- Delay     (8 Bit)
  // -- Direction (1 Bit)
  // -- Speed     (7 Bit)
  for (int i = 0; i < 8; i++)
  {
    input[i] = read_byte();
  }

  finish_transmission();
#ifdef DEBUG
  // Serial.println("DataManager >> Loading new image..");
#endif

  MotorData motordata[2];
  motordata[0] = deserialize(&input[0]);
  motordata[1] = deserialize(&input[4]);

  this->moma.set_motor_data(motordata);
  // Serial.println("Image updated");
}

MotorData DataManager::deserialize(uint8_t *stream)
{
  MotorData data;
  data.position = ((stream[0] << 4u) + ((stream[1] >> 4u) & 0x0Fu));
  data.waitSteps = ((stream[1] & 0x0Fu) << 8u) + stream[2];
  data.delay = (stream[3] >> 1u);
  data.direction = stream[3] & 0x01u;
#ifdef DEBUG
  // Serial.println("DataManager >> Serialisierung: Pos = " + String(data.position) + " waitSteps = " + String(data.waitSteps) + " delay = " + String(data.delay) + " direction = " + String(data.direction));
#endif
  return data;
}
