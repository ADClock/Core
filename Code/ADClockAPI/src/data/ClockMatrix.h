
#ifndef _CLOCKMATRIX_H_
#define _CLOCKMATRIX_H_
#include "Arduino.h"
#include "Clock.h"

#define CLOCKS_X 12
#define CLOCKS_Y 6

class ClockMatrix
{
public:
  ClockMatrix();

  const Clock &getClock(size_t x, size_t y) { return matrix[x][y]; }

  /**
   * Web API Kommunikation
   */
  JSONValue asJson();

  /**
  * Initalisierung/Tuning 
  */
  void initMatrix();

  bool isValidCoordinates(size_t x, size_t y);

  // Matrix der Uhren
  Clock matrix[CLOCKS_X][CLOCKS_Y];

private:
};
#endif
