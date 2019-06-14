#include "ClockMatrix.h"
#include "Debug.h"

Timer timer2;

ClockMatrix::ClockMatrix(DataSender &sender) : sender(sender)
{
  timer2.start();
}

void ClockMatrix::initMatrix()
{
  sender.sendInitCommand();

  for (size_t x = 0; x < CLOCKS_X; x++)
  {
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
      matrix[x][y].init();
    }
  }
}

bool ClockMatrix::hasPendingMove()
{
  for (size_t x = 0; x < CLOCKS_X; x++)
  {
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
      if (matrix[x][y].hasPendingMove())
        return true;
    }
  }
  return false;
}

size_t ClockMatrix::getClockPosition(size_t x, size_t y)
{
  size_t clockPos = x * CLOCKS_Y;
  if (x % 2 == 0)
  {
    clockPos += CLOCKS_Y - y - 1;
  }
  else
  {
    clockPos += y;
  }
  return clockPos;
}

void ClockMatrix::move()
{
  uint8_t image[8 * CLOCKS_X * CLOCKS_Y];
  unsigned long longestMoveTime = 0;

  unsigned long _clock_estimated_move_time;
  size_t _clock_position;
  uint8_t *_clock_image;

  for (size_t x = 0; x < CLOCKS_X; x++)
  {
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
      _clock_position = getClockPosition(x, y) * 8;
      // Werte der einzelnen Uhr laden
      _clock_image = matrix[x][y].nextStepAsImage();
      for (size_t i = 0; i < 8; i++)
      {
        image[_clock_position + i] = _clock_image[i];
      }

      _clock_estimated_move_time = matrix[x][y].getCalculateMoveTime();
      if (_clock_estimated_move_time > longestMoveTime)
        longestMoveTime = _clock_estimated_move_time;

      // Vermerken, dass die Uhren bewegt wurden
      matrix[x][y].move();
    }
  }

  if (!sender.sendImage(image, 16 /** CLOCKS_X * CLOCKS_Y*/))
  {
    // Image konnte nicht verschickt werden.
    Debug::println("Das Image konnte nicht verschickt werden.");
    return;
  }

  // Clock Image wurde erfolgreich geschickt
  this->reachesPosition = timer2.read_ms() + longestMoveTime + 10;
  // Debug::serial.printf("Moving until %d ", reachesPosition);
}

bool ClockMatrix::setNextHourPosition(size_t x, size_t y, size_t degree)
{
  if (!isValidCoordinates(x, y))
    return false;

  matrix[x][y].setNextHourPosition(degree);
  return true;
}

bool ClockMatrix::setNextMinutePosition(size_t x, size_t y, size_t degree)
{
  if (!isValidCoordinates(x, y))
    return false;

  matrix[x][y].setNextMinutePosition(degree);
  return true;
}

bool ClockMatrix::setNextPositionFor(size_t x_from, size_t y_from, size_t x_to, size_t y_to, size_t degreeHour, size_t degreeMinute)
{

  if (!isValidCoordinates(x_from, y_from))
    return false;

  if (!isValidCoordinates(x_to, y_to))
    return false;

  for (size_t x = x_from; x <= x_to; x++)
  {
    for (size_t y = y_from; y <= y_to; y++)
    {
      matrix[x][y].setNextHourPosition(degreeHour);
      matrix[x][y].setNextMinutePosition(degreeMinute);
    }
  }

  return true;
}

bool ClockMatrix::isValidCoordinates(size_t x, size_t y)
{
  // usigned int always >= 0
  return x < CLOCKS_X && y < CLOCKS_Y;
}

bool ClockMatrix::isMoving()
{
  return timer2.read_ms() < this->reachesPosition;
}
