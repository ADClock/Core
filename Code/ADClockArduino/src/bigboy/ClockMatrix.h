
#ifndef _CLOCKMATRIX_H_
#define _CLOCKMATRIX_H_
#include "Arduino.h"
#include "Clock.h"
#include "DataSender.h"

#define CLOCKS_X 12
#define CLOCKS_Y 6

class ClockMatrix
{
public:
  ClockMatrix(DataSender &sender);

  bool setNextPositionFor(size_t x_from, size_t y_from, size_t x_to, size_t y_to, size_t degreeMinute, size_t degreeHour);

  bool setNextMinutePosition(size_t x, size_t y, size_t degree);
  bool setNextHourPosition(size_t x, size_t y, size_t degree);

  bool hasPendingMove();

  void initMatrix();

  void move();

private:
  bool isValidCoordinates(size_t x, size_t y);

  DataSender &sender;

  // Matrix der Uhren
  Clock *matrix[CLOCKS_X][CLOCKS_Y];

  // Haben die Uhren das hier genannte Image erreicht?
  unsigned long reachesPosition;
};
#endif
