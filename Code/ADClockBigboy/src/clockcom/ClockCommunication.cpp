#include "ClockCommunication.h"

Timer timer;
ClockCommunication::ClockCommunication(ClockOutputStream &out) : out(out)
{
  timer.start();
  this->lastSend = timer.read_ms() + DELAY_BETWEEN_COMMANDS * 3;
}

bool ClockCommunication::sendInitCommand()
{
#ifdef DEBUG
  Debug::println("ClockCom >> Sending init command");
#endif
  return sendCommand(0x01);
}

bool ClockCommunication::sendPlan(ClockWall &plan)
{
#ifdef DEBUG
  Debug::println("ClockCom >> Sending plan...");
#endif
  if (!sendCommand(0x02))
    return false;

  for (size_t i = 0; i < WALL_SIZE_Y * WALL_SIZE_X; i++)
  {
#ifdef SEND_ONLY_CLOCKS
    if (i == SEND_ONLY_CLOCKS)
    {
      return true;
    }
#endif

    Clock &clock = plan.getClock(getClockX(i), getClockY(i));
    u_int8_t *data = serialize(clock);
    if (!this->out.sendByteArray(data, 8))
      return false;
  }

  return true;
}

bool ClockCommunication::sendCommand(const uint8_t command)
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
size_t ClockCommunication::getClockX(size_t pos)
{
  return pos / WALL_SIZE_Y;
}

size_t ClockCommunication::getClockY(size_t pos)
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

u_int8_t *ClockCommunication::serialize(Clock &clock)
{
  return clock.serialize();
}