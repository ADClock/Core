#include "ClockPositions.h"

Timer clockTimer;
ClockPositions::ClockPositions()
{
  clockTimer.start();
}

void ClockPositions::initClockPosition(size_t x, size_t y)
{
  this->current[x][y].hour.current_pos = 0;
  this->current[x][y].minute.current_pos = 0;
}

bool ClockPositions::isOnPosition(size_t x, size_t y, Clock &clock)
{
  return this->current[x][y].hour.current_pos == clock.hour.getPosition() && this->current[x][y].minute.current_pos == clock.minute.getPosition();
}

void ClockPositions::try_step(size_t x, size_t y, Clock &clock)
{
  try_step(this->current[x][y].hour, clock.hour);
  try_step(this->current[x][y].minute, clock.minute);
}

void ClockPositions::try_step(MotorData &data, Hand &hand)
{
  if (data.current_pos == hand.getPosition())
    return;

  long time_since_step = clockTimer.read_us() - data.last_step;
  if (time_since_step > (long)(MIN_STEP_DELAY + (hand.getStepDelay() * 100)) || time_since_step < 0)
  {
    data.last_step = clockTimer.read_us();
    if (data.wait_steps > 0)
    {
      data.wait_steps--;
    }
    else
    {
#ifdef DEBUG
      // Debug::printf("ClockPositions >> try_step stepping from %d \n", data.current_pos);
#endif
      // Execute one step
      if (hand.getDirection())
      {
        data.current_pos++;
        if (data.current_pos > STEPS_FOR_CIRCLE)
        {
          data.current_pos = 0;
        }
      }
      else
      {
        if (data.current_pos == 0)
        {
          data.current_pos = STEPS_FOR_CIRCLE;
        }
        else
        {
          data.current_pos--;
        }
      }
    }
  }
}

void ClockPositions::setWaitSteps(size_t x, size_t y, size_t wait_steps_hour, size_t wait_steps_minute)
{
  this->current[x][y].hour.wait_steps = wait_steps_hour;
  this->current[x][y].minute.wait_steps = wait_steps_minute;
}

void ClockPositions::update(ClockWall &wall)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      auto clock = wall.getClock(x, y);
      setWaitSteps(x, y, clock.hour.getWaitSteps(), clock.hour.getWaitSteps());
    }
  }
}