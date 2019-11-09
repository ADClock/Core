
#ifndef _CLOCKWALL_H_
#define _CLOCKWALL_H_
#include "mbed.h"
#include "../Config.h"
#include "../Debug.h"
#include "Clock.h"

class ClockWall
{
public:
  ClockWall();

  /**
  * Clock Image setzen 
  */
  bool setMutiplePositions(size_t x_from, size_t y_from, size_t x_to, size_t y_to, size_t degreeHour, size_t degreeMinute);

  bool setHourPosition(size_t x, size_t y, size_t degree);
  bool setMinutePosition(size_t x, size_t y, size_t degree);

  bool printChar(uint8_t segment, char c);

  bool setHourRotation(bool direction);
  bool setMinuteRotation(bool direction);

  bool setAnimationStart(size_t hourStartDeg, size_t minuteStartDeg);

  bool setHourSpeed(size_t speed);
  bool setMinuteSpeed(size_t speed);

  /**
   * Web API Kommunikation
   */

  MbedJSONValue asJson();
  void update(MbedJSONValue &json);
  void update(ClockWall &wall);

  bool equals(ClockWall &wall);

  /**
  * Hilfsfunktionen für Positionen
  */
  size_t getClockPosition(size_t x, size_t y);

  Clock &getClock(size_t x, size_t y) { return matrix[x][y]; };

private:
  bool isValidCoordinates(size_t x, size_t y);
  bool setAnimationStartStep(size_t hourStart, size_t minuteStart);

  // Matrix der Uhren
  Clock matrix[WALL_SIZE_X][WALL_SIZE_X];
};
#endif
