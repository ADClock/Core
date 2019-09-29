#include "ClockCommunication.h"

Timer timer;
ClockCommunication::ClockCommunication(ClockOutputStream &out) : out(out)
{
  timer.start();
  this->lastSend = timer.read_ms();
}

bool ClockCommunication::sendInitCommand()
{
#ifdef DEBUG
  Debug::println("ClockCom >> Sending init command");
#endif
  return sendCommand(COMMAND_INIT);
}

bool ClockCommunication::sendPlan(ClockWall &plan)
{
#ifdef DEBUG
  // Debug::println("ClockCom >> Sending plan...");
#endif
  if (!sendCommand(COMMAND_IMAGE))
    return false;

  for (size_t i = 0; i < WALL_SIZE_Y * WALL_SIZE_X; i++)
  {
#ifdef SEND_ONLY_CLOCKS
    if (i == SEND_ONLY_CLOCKS)
    {
      return true;
    }
#endif
    auto &clock = plan.getClock(getClockX(i), getClockY(i));
    if (!this->out.sendByteArray(clock.hour.serialize(), 4))
      return false;

    if (!this->out.sendByteArray(clock.minute.serialize(), 4))
      return false;
  }

  return true;
}

bool ClockCommunication::sendCommand(const uint8_t &command)
{
  // Delay zwischen einzelnen Commands einhalten
  while (timer.read_ms() < this->lastSend + DELAY_BETWEEN_COMMANDS)
  {
    wait_ms(1);
  }
  this->lastSend = timer.read_ms();

  if (!out.sendByte(command))
    return false;
  return true;
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

#ifdef DEBUG
void ClockCommunication::performSpeedtest()
{
  size_t dataLength = 100000;
  int start = 0;
  int end = 0;

  u_int8_t data[dataLength];

  for (int i = 0; i < 3; i++)
  {
    start = timer.read_us();
    sendCommand(0x03);
    out.sendByteArray(data, dataLength);
    end = timer.read_us();
    Debug::printf("ClockCom >> Speedtest finished in %d µs\n", (int)(end - start));
  }
}
#endif