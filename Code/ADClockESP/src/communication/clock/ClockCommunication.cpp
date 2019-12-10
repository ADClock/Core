#include "ClockCommunication.h"

ClockCommunication::ClockCommunication(BitBuffer &buffer, DataSender &sender) : buffer(buffer), sender(sender)
{
}

void ClockCommunication::sendInitCommand()
{
#ifdef DEBUG
  Debug::println("ClockCom >> Sending init command");
#endif
  sendCommand(COMMAND_INIT);
}

void ClockCommunication::sendPlan(ClockWall &plan)
{
#ifdef DEBUG
  // Debug::println("ClockCom >> Sending plan...");
  for (size_t i = 0; i < WALL_CLOCKS; i++)
  {
    auto &clock = plan.getClock(getClockX(i), getClockY(i));

    Debug::serial.printf("Clock %u x=%u y=%u hourPos=%u minutePos=%u\n", i, getClockX(i), getClockY(i), clock.hour.getPosition(), clock.minute.getPosition());
  }
#endif

  sendCommand(COMMAND_IMAGE);

  for (size_t i = 0; i < WALL_CLOCKS; i++)
  {
#ifdef SEND_ONLY_CLOCKS
    if (i == SEND_ONLY_CLOCKS)
    {
      return;
    }
#endif

    auto &clock = plan.getClock(getClockX(i), getClockY(i));

    auto *hour = clock.hour.serialize();
    sendData(hour, 4);
    auto *minute = clock.minute.serialize();
    sendData(minute, 4);
  }

  return;
}

void ClockCommunication::sendCommand(uint8_t command)
{
  cleanup_communication();
  //Serial.println("CockCom: Send Command..");
  sendByte(command);
}

void ClockCommunication::sendData(uint8_t *data, size_t length)
{
  for (size_t i = 0; i < length; i++)
    sendByte(data[i]);
}

void ClockCommunication::sendByte(uint8_t byte)
{
  for (uint8_t i = 0; i < 8; i++)
    this->buffer.enqueue(!!(byte & (1 << i)));
}

// Liefert zu der Position in der Verkabelungskette die richtige Position in der Matrix
size_t ClockCommunication::getClockX(size_t &pos)
{
  return pos / WALL_SIZE_Y;
}

size_t ClockCommunication::getClockY(size_t &pos)
{
  if ((pos / WALL_SIZE_Y) % 2 == 0)
  {
    return WALL_SIZE_Y - (pos % WALL_SIZE_Y) - 1;
  }
  else
  {
    return (pos % WALL_SIZE_Y);
  }
}

bool ClockCommunication::tramsmit()
{
  this->sender.tick();
  while (is_transmitting())
    this->sender.tick();

  return was_successful();
}

bool ClockCommunication::is_transmitting()
{
  return this->sender.sending() && !this->sender.failed();
}

bool ClockCommunication::was_successful()
{
  return !this->sender.failed();
}

void ClockCommunication::cleanup_communication()
{
  // Serial.println("CockCom: Cleaning..");
  while (sender.sending() || sender.time_waiting() < DELAY_BETWEEN_COMMANDS)
  {
    sender.tick(); // Push out previous command
    if (sender.failed())
      sender.reset();
    delay(1); // wait vor last command to finish
  }
}