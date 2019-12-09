
#ifndef _CLOCKPOS_H_
#define _CLOCKPOS_H_
#include "Arduino.h"
#include "../Config.h"
#include "ClockWall.h"
#include <cstdlib>
using namespace std;

struct MotorData
{
  size_t current_pos;
  size_t wait_steps;
  unsigned long last_step;
};

struct ClockPosition
{
  MotorData minute;
  MotorData hour;
};

class ClockPositions
{
public:
  ClockPositions();

  void try_step(size_t x, size_t y, Clock &clock);

  bool isOnPosition(size_t x, size_t y, Clock &clock);

  void initClockPosition(size_t x, size_t y);

  void update(ClockWall &wall);

  JSONValue asJson();
  /**
  * Hilfsfunktionen für Positionen
  */
  size_t getClockPosition(size_t x, size_t y);

private:
  void setWaitSteps(size_t x, size_t y, size_t wait_steps_hour, size_t wait_steps_minute);

  void try_step(MotorData &data, Hand &hand);

  ClockPosition current[WALL_SIZE_X][WALL_SIZE_Y];
};
#endif
