#include "ClockMatrix.h"

ClockMatrix::ClockMatrix(DataSender &sender) : sender(sender)
{
  this->initMatrix();
}

void ClockMatrix::initMatrix()
{
  sender.sendInitCommand();
}

bool ClockMatrix::hasPendingMove()
{
  for (size_t x = 0; x < CLOCKS_X; x++)
  {
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
      if ((*matrix[x][y]).hasPendingMove())
        return true;
    }
  }
  return false;
}

long getClockPosition(size_t x, size_t y)
{
  // TODO
  return 42;
}
void ClockMatrix::move()
{
  byte image[8 * CLOCKS_X * CLOCKS_Y];
  unsigned long currentClockMoveTime;
  unsigned long longestMoveTime = 0;

  for (size_t x = 0; x < CLOCKS_X; x++)
  {
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
      // Werte der einzelnen Uhr laden
      currentClockMoveTime = (*matrix[x][y]).getCalculateMoveTime();
      image[8 * getClockPosition(x, y)] = *(*matrix[x][y]).nextStepAsImage();

      if (currentClockMoveTime > longestMoveTime)
        longestMoveTime = currentClockMoveTime;
    }
  }

  if (!sender.sendImage(image, 8 * CLOCKS_X * CLOCKS_Y))
  {
    // Image konnte nicht verschickt werden.
    return;
  }

  // Clock Image wurde erfolgreich geschickt
  this->reachesPosition += longestMoveTime; // TODO Aktuelle Zeit + longestMoveTime

  // Vermerken, dass die Uhren bewegt wurden
  for (size_t x = 0; x < CLOCKS_X; x++)
  {
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
      (*matrix[x][y]).move();
    }
  }
}

bool ClockMatrix::setNextHourPosition(size_t x, size_t y, size_t degree)
{
  if (!isValidCoordinates(x, y))
    return false;

  (*matrix[x][y]).setNextHourPosition(degree);
  return true;
}

bool ClockMatrix::setNextMinutePosition(size_t x, size_t y, size_t degree)
{
  if (!isValidCoordinates(x, y))
    return false;

  (*matrix[x][y]).setNextMinutePosition(degree);
  return true;
}

bool ClockMatrix::setNextPositionFor(size_t x_from, size_t y_from, size_t x_to, size_t y_to, size_t degreeMinute, size_t degreeHour)
{

  if (!isValidCoordinates(x_from, y_from))
    return false;

  if (!isValidCoordinates(x_to, y_to))
    return false;

  for (size_t x = x_from; x <= x_to; x++)
  {
    for (size_t y = y_from; y <= y_to; y++)
    {
      (*matrix[x][y]).setNextMinutePosition(degreeMinute);
      (*matrix[x][y]).setNextHourPosition(degreeHour);
    }
  }

  return true;
}