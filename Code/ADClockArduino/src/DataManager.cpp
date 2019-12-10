#include "DataManager.h"

DataManager::DataManager(DataSender &sender, BitBuffer &out, DataReceiver &receiver, BitBuffer &in, MotorManager &moma) : sender(sender), out(out), receiver(receiver), in(in), moma(moma)
{
}

void DataManager::tick()
{
  this->receiver.tick();
  this->sender.tick();

  switch (state)
  {
  case CommandState::IDLE:
    if (!this->in.is_empty())
    {
      this->state = CommandState::READING_COMMAND;
    }
    else
    {
      // Wenn kein Command ankommt können wir ja auch alles senden
      if (!this->out.is_empty())
      {
        this->sender.tick();

        while (this->sender.sending() && !this->sender.failed())
          this->sender.tick();

        if (this->sender.failed())
          this->sender.reset();
      }
    }
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
      processImage();
    }
    break;

  case CommandState::PIPEING:
    while (!this->in.is_empty())
    {
      this->out.enqueue(this->in.dequeue());
    }
  case CommandState::TESTING:
    in.clear(); // Dafür sorgen, dass Buffer nicht überläuft

  default:
    break;
  }

  if (this->receiver.complete())
  {
    this->state = CommandState::IDLE;
    this->receiver.reset();
    // Serial.println("receiving complete");
  }

  if (this->receiver.failed())
  {
    this->state = CommandState::IDLE;
    this->receiver.reset();

    while (this->receiver.time_waiting() < DELAY_BETWEEN_COMMANDS)
    {
      this->sender.tick();
    }
  }
}

void DataManager::set_current_command()
{
  auto command = read_byte();
  this->receiver.confirm();

  // Serial.println("Processing command.. cmd = " + String(command));
  switch (command)
  {
  case 0x01:
    sendCommand(0x01);
    finish_transmission();
    moma.calibrate();
    break;

  case 0x02:
    sendCommand(0x02);
    this->state = CommandState::READING_IMAGE;
    break;

  case 0x03:
    this->state = CommandState::TESTING;
    break;

  default:
    break;
  }
}

void DataManager::sendCommand(uint8_t command)
{
  while (sender.sending() || sender.time_waiting() < DELAY_BETWEEN_COMMANDS)
  {
    if (sender.failed())
      sender.reset();
    this->receiver.tick();
  }
  sendByte(command);
}

void DataManager::sendByte(uint8_t byte)
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
  while (this->state != CommandState::IDLE || !this->out.is_empty())
    this->tick();
}

void DataManager::processImage()
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

  DataStruct motordata[2];
  motordata[0] = deserialze(&input[0]);
  motordata[1] = deserialze(&input[4]);

  this->moma.setMotorData(motordata);
}

DataStruct DataManager::deserialze(uint8_t *stream)
{
  DataStruct data;
  data.position = ((stream[0] << 4u) + ((stream[1] >> 4u) & 0x0Fu));
  data.waitSteps = ((stream[1] & 0x0Fu) << 8u) + stream[2];
  data.delay = (stream[3] >> 1u);
  data.direction = stream[3] & 0x01u;
#ifdef DEBUG
  // Serial.println("DataManager >> Serialisierung: Pos = " + String(data.position) + " waitSteps = " + String(data.waitSteps) + " delay = " + String(data.delay) + " direction = " + String(data.direction));
#endif
  return data;
}
