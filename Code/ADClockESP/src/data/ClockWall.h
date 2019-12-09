
#ifndef _CLOCKWALL_H_
#define _CLOCKWALL_H_
#include "Arduino.h"
#include "../Config.h"
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

  bool setHourDirection(size_t x, size_t y, bool direction);
  bool setMinuteDirection(size_t x, size_t y, bool direction);

  bool setHourStepDelay(size_t x, size_t y, size_t step_delay);
  bool setMinuteStepDelay(size_t x, size_t y, size_t step_delay);

  bool setHourWaitSteps(size_t x, size_t y, size_t wait_steps);
  bool setMinuteWaitSteps(size_t x, size_t y, size_t wait_steps);

  bool printChar(uint8_t segment, char c);

  bool setHourRotation(bool direction);
  bool setMinuteRotation(bool direction);

  bool setAnimationStart(size_t hourStartDeg, size_t minuteStartDeg);

  bool setHourSpeed(size_t speed);
  bool setMinuteSpeed(size_t speed);

  /**
   * Web API Kommunikation
   */

  JSONValue asJson();
  void update(JSONValue &json);
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
