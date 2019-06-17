
#ifndef _CLOCKMATRIX_H_
#define _CLOCKMATRIX_H_
#include "mbed.h"
#include "Clock.h"
#include "DataSender.h"

#define CLOCKS_X 12
#define CLOCKS_Y 6

class ClockMatrix
{
public:
  ClockMatrix(DataSender &sender);

  /**
  * Clock Image setzen 
  */
  bool setNextPositionFor(size_t x_from, size_t y_from, size_t x_to, size_t y_to, size_t degreeHour, size_t degreeMinute);

  bool setNextMinutePosition(size_t x, size_t y, size_t degree);
  bool setNextHourPosition(size_t x, size_t y, size_t degree);

  bool printChar(uint8_t segment, char c);

  bool setHourRotation(bool direction);
  bool setMinuteRotation(bool direction);

  bool setAnimationStart(size_t hourStartDeg, size_t minuteStartDeg);

  /**
  * Initalisierung/Tuning 
  */
  void initMatrix();

  /**
  * Bewegung  
  */
  bool hasPendingMove();
  void move();
  bool isMoving();

  /**
  * Hilfsfunktionen
  */
  size_t getClockPosition(size_t x, size_t y);
  size_t getClockX(size_t position);
  size_t getClockY(size_t position);

private:
  bool isValidCoordinates(size_t x, size_t y);
  bool setAnimationStartStep(size_t hourStart, size_t minuteStart);

  DataSender &sender;

  // Matrix der Uhren
  Clock matrix[CLOCKS_X][CLOCKS_Y];

  // Haben die Uhren das hier genannte Image erreicht?
  long reachesPosition;
};
#endif
