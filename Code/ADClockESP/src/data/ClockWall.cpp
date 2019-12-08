#include "ClockWall.h"

ClockWall::ClockWall()
{
}

size_t ClockWall::getClockPosition(size_t x, size_t y)
{
  size_t clockPos = x * WALL_SIZE_Y;
  if (x % 2 == 0)
  {
    clockPos += WALL_SIZE_Y - y - 1;
  }
  else
  {
    clockPos += y;
  }
  return clockPos;
}

bool ClockWall::setHourPosition(size_t x, size_t y, size_t degree)
{
  if (!isValidCoordinates(x, y))
    return false;

  matrix[x][y].hour.setPositionDegree(degree);
  return true;
}

bool ClockWall::setMinuteDirection(size_t x, size_t y, bool direction)
{
  if (!isValidCoordinates(x, y))
    return false;

  matrix[x][y].minute.setDirection(direction);
  return true;
}

bool ClockWall::setHourDirection(size_t x, size_t y, bool direction)
{
  if (!isValidCoordinates(x, y))
    return false;

  matrix[x][y].hour.setPositionDegree(direction);
  return true;
}

bool ClockWall::setMinutePosition(size_t x, size_t y, size_t degree)
{
  if (!isValidCoordinates(x, y))
    return false;

  matrix[x][y].minute.setPositionDegree(degree);
  return true;
}

bool ClockWall::setMutiplePositions(size_t x_from, size_t y_from, size_t x_to, size_t y_to, size_t degreeHour, size_t degreeMinute)
{

  if (!isValidCoordinates(x_from, y_from))
    return false;

  if (!isValidCoordinates(x_to, y_to))
    return false;

  for (size_t x = x_from; x <= x_to; x++)
  {
    for (size_t y = y_from; y <= y_to; y++)
    {
      matrix[x][y].hour.setPositionDegree(degreeHour);
      matrix[x][y].minute.setPositionDegree(degreeMinute);
    }
  }

  return true;
}

bool ClockWall::isValidCoordinates(size_t x, size_t y)
{
  // usigned int always >= 0
  return x < WALL_SIZE_X && y < WALL_SIZE_Y;
}

bool ClockWall::printChar(uint8_t segment, char c)
{
  // TODO Fixen
  uint16_t *positions;
  size_t currentElement;
  for (int y = 0; y < WALL_SIZE_Y; y++)
    for (int x = segment; x < (segment + 1) * 3; x++)
    {
      setHourPosition(x, y, positions[currentElement++]);
      setMinutePosition(x, y, positions[currentElement++]);
    }
  return true;
}

bool ClockWall::setHourRotation(bool direction)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      matrix[x][y].hour.setDirection(direction);
    }
  }
  return true;
}

bool ClockWall::setMinuteRotation(bool direction)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      matrix[x][y].minute.setDirection(direction);
    }
  }
  return true;
}

bool ClockWall::setAnimationStart(size_t hourDeg, size_t minuteDeg)
{
  return setAnimationStartStep(hourDeg * STEPS_PER_DEGREE, minuteDeg * STEPS_PER_DEGREE);
}

bool ClockWall::setAnimationStartStep(size_t hourStart, size_t minuteStart)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      matrix[x][y].hour.setSimultaneouslyMove(hourStart);
      matrix[x][y].minute.setSimultaneouslyMove(minuteStart);
    }
  }
  return true;
}

bool ClockWall::setHourSpeed(size_t speed)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      matrix[x][y].hour.setDelayBetweenSteps(speed);
    }
  }
  return true;
}

bool ClockWall::setMinuteSpeed(size_t speed)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      matrix[x][y].minute.setDelayBetweenSteps(speed);
    }
  }
  return true;
}

JSONValue ClockWall::asJson()
{
  JSONValue v;

  v["clocks-x"] = WALL_SIZE_X;
  v["clocks-y"] = WALL_SIZE_Y;
  for (size_t index = 0; index < WALL_SIZE_X; index++)
  {
    v["matrix"][index] = matrix[index][0].asJson();
    // v["matrix"][getClockPosition(x, y)] = matrix[x][y].asJson();
    // v["matrix"][getClockPosition(x, y)]["x"] = static_cast<int>(x);
    // v["matrix"][getClockPosition(x, y)]["y"] = static_cast<int>(y);
  }

  return v;
}

// Übernimmt alle Einstellunge der anderen Wall
void ClockWall::update(ClockWall &wall)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      matrix[x][y].update(wall.getClock(x, y));
    }
  }
}

bool ClockWall::equals(ClockWall &wall)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      if (!matrix[x][y].equals(wall.getClock(x, y)))
        return false;
    }
  }
  return true;
}
