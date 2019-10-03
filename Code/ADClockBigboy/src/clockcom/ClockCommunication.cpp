#include "ClockCommunication.h"
#include "../SpeedCheck.h"

SpeedCheck findClock{">>> find Clock: "};
SpeedCheck serialize{">>> serialize: "};
SpeedCheck theplan{">> send plan: "};
SpeedCheck snddta{">>> send Data: "};
SpeedCheck sndcmd{">>> send Image Command: "};

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
  theplan.start();
#ifdef DEBUG
  // Debug::println("ClockCom >> Sending plan...");
#endif

  sndcmd.start();
  if (!sendCommand(COMMAND_IMAGE))
    return false;
  sndcmd.stop();

  for (size_t i = 0; i < WALL_CLOCKS; i++)
  {
#ifdef SEND_ONLY_CLOCKS
    if (i == SEND_ONLY_CLOCKS)
    {
      theplan.stop();
      return true;
    }
#endif

    findClock.start();
    auto &clock = plan.getClock(getClockX(i), getClockY(i));
    findClock.stop();
    findClock.pushback();

    serialize.start();
    auto *hour = clock.hour.serialize();
    serialize.stop();
    serialize.pushback();

    snddta.start();
    if (!this->out.sendSerializedHand(hour))
      return false;
    snddta.stop();
    snddta.pushback();

    serialize.start();
    auto *minute = clock.minute.serialize();
    serialize.stop();
    serialize.pushback();

    snddta.start();
    if (!this->out.sendSerializedHand(minute))
      return false;
    snddta.stop();
    snddta.pushback();
  }

  return true;
}

bool ClockCommunication::sendCommand(const uint8_t &command)
{
  // Delay zwischen einzelnen Commands einhalten
  while (us_ticker_read() < this->lastSend + DELAY_BETWEEN_COMMANDS && us_ticker_read() > this->lastSend)
  {
    wait_ms(1);
  }
  this->lastSend = us_ticker_read();

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
  size_t dataLength = 10;
  u_int8_t data[dataLength];

  Debug::serial.printf("sizeof uint32_t %u\n", sizeof(uint32_t));

  for (int i = 0; i < 3; i++)
  {
    wait(2);
    sendCommand(0x03);
    int start = us_ticker_read();
    out.sendByteArray(data, dataLength);
    int end = us_ticker_read();
    int duration = (end - start);
    Debug::serial.printf("ClockCom >> Speedtest finished in %d µs\n", duration);
  }
}

void ClockCommunication::printResult()
{
  theplan.printResult();

  sndcmd.printResult();

  findClock.printResult();

  serialize.printResult();

  snddta.printResult();
}

#endif