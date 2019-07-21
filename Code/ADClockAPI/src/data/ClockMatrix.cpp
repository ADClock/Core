#include "ClockMatrix.h"

ClockMatrix::ClockMatrix()
{
  initMatrix();
}

void ClockMatrix::initMatrix()
{
  for (size_t x = 0; x < CLOCKS_X; x++)
  {
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
      matrix[x][y].init();
    }
  }
}

bool ClockMatrix::isValidCoordinates(size_t x, size_t y)
{
  // usigned int always >= 0
  return x < CLOCKS_X && y < CLOCKS_Y;
}

JSONValue ClockMatrix::asJson()
{
  JSONValue v;

  v["clocks-x"] = CLOCKS_X;
  v["clocks-y"] = CLOCKS_Y;
  // TODO Running out of Memory
  for (size_t index = 0; index < CLOCKS_X; index++)
  {
    v["matrix"][index] = matrix[0][0].asJson();
    // v["matrix"][getClockPosition(x, y)] = matrix[x][y].asJson();
    // v["matrix"][getClockPosition(x, y)]["x"] = static_cast<int>(x);
    // v["matrix"][getClockPosition(x, y)]["y"] = static_cast<int>(y);
  }

  return v;
}