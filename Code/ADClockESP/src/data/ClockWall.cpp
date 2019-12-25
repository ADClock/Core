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

bool ClockWall::set_position(HandSelection selection, size_t degree)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      set_position(x, y, selection, degree);
    }
  }
  return true;
}

bool ClockWall::set_position(size_t x, size_t y, HandSelection selection, size_t degree)
{
  if (!valid_clock_cords(x, y))
    return false;

  if (selection == HOUR || selection == BOTH)
    matrix[x][y].hour.setPositionDegree(degree);

  if (selection == MINUTE || selection == BOTH)
    matrix[x][y].minute.setPositionDegree(degree);

  return true;
}

bool ClockWall::set_wait_degree(HandSelection selection, size_t degree)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      set_wait_degree(x, y, selection, degree);
    }
  }
  return true;
}

bool ClockWall::set_wait_degree(size_t x, size_t y, HandSelection selection, size_t degree)
{
  if (!valid_clock_cords(x, y))
    return false;

  if (selection == HOUR || selection == BOTH)
    matrix[x][y].hour.setWaitDegree(degree);

  if (selection == MINUTE || selection == BOTH)
    matrix[x][y].minute.setWaitDegree(degree);

  return true;
}

bool ClockWall::set_step_delay(HandSelection selection, size_t step_delay)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      set_step_delay(x, y, selection, step_delay);
    }
  }
  return true;
}

bool ClockWall::set_step_delay(size_t x, size_t y, HandSelection selection, size_t step_delay)
{
  if (!valid_clock_cords(x, y))
    return false;

  if (selection == HOUR || selection == BOTH)
    matrix[x][y].hour.setDelayBetweenSteps(step_delay);

  if (selection == MINUTE || selection == BOTH)
    matrix[x][y].minute.setDelayBetweenSteps(step_delay);

  return true;
}

bool ClockWall::set_direction(HandSelection selection, bool direction)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      set_direction(x, y, selection, direction);
    }
  }
  return true;
}

bool ClockWall::set_direction(size_t x, size_t y, HandSelection selection, bool direction)
{
  if (!valid_clock_cords(x, y))
    return false;

  if (selection == HOUR || selection == BOTH)
    matrix[x][y].hour.setDirection(direction);

  if (selection == MINUTE || selection == BOTH)
    matrix[x][y].minute.setDirection(direction);

  return true;
}

bool ClockWall::valid_clock_cords(size_t x, size_t y)
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
      set_position(x, y, HOUR, positions[currentElement++]);
      set_position(x, y, MINUTE, positions[currentElement++]);
    }
  return true;
}

JsonDocument ClockWall::asJson()
{
  DynamicJsonDocument doc(8192);

  doc["clocks-x"] = WALL_SIZE_X;
  doc["clocks-y"] = WALL_SIZE_Y;

  JsonArray xelem = doc["matrix"].to<JsonArray>();
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      //v["matrix"][index] = matrix[index][0].asJson();
      //v["matrix"][getClockPosition(x, y)] = matrix[x][y].asJson();
      //v["matrix"][getClockPosition(x, y)]["x"] = static_cast<int>(x);
      //v["matrix"][getClockPosition(x, y)]["y"] = static_cast<int>(y);
      JsonDocument foo = this->matrix[x][y].asJson();
      foo["x"] = static_cast<int>(x);
      foo["y"] = static_cast<int>(y);
      xelem.add(foo);
    }
  }

  return doc;
}

// Übernimmt alle Einstellunge der anderen Wall
void ClockWall::update(ClockWall &wall)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      matrix[x][y].update(wall.matrix[x][y]);
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

//  Ein Plan muss nur neu verschickt werden, wenn sich die Zielposition unterscheidet.
bool ClockWall::different_target(ClockWall &wall)
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      if (matrix[x][y].hour.getPosition() != wall.matrix[x][y].hour.getPosition())
        return true;
      if (matrix[x][y].minute.getPosition() != wall.matrix[x][y].minute.getPosition())
        return true;
    }
  }
  return false;
}

bool ClockWall::show_time(uint8_t hour, uint8_t minute)
{

  auto hour_degree = 360 - 360. / 12 * (hour % 12);
  auto minute_degree = 360 - 360. / 60 * minute;
  set_position(HOUR, hour_degree);
  set_position(MINUTE, minute_degree);
}