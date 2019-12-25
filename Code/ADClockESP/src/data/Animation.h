#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Arduino.h"
#include "../Config.h"
#include "Manager.h"
#include "NTPTime.h"
#include "SPIFFS.h"

extern Manager _manager;
extern ClockWall planned;
extern NTPTime _time;

enum ClockSelection
{
  ALL,
  SINGLE
};

enum StepType
{
  INIT,
  DELAY,
  WAIT_UNTIL_PLAN_SEND,
  WAIT_UNTIL_CLOCKS_STAND_STILL,
  REPEAT,
  SELECT_CLOCK,
  SELECT_X,
  SELECT_Y,
  SELECT_HAND,
  TARGET_POS,
  DIRECTION,
  WAIT_STEPS,
  STEP_DELAY,
  MOVE_IN_SYNC,
  SHOW_CURRENT_TIME_ON_EACH,
  SHOW_CURRENT_TIME
};

struct Step
{
  StepType type;
  size_t value;
};

class Animation
{
public:
  Animation(String name = "");

  void tick();

  bool has_pending_step();
  void reset_animation();

private:
  void execute_step();
  void load_next_step();
  void set_current_step(String &step);

  ClockSelection clock;
  size_t x;
  size_t y;
  HandSelection hand;

  Step current_step;
  unsigned long last_load;

  String data;
  unsigned int data_pos;
  boolean no_more_step;
};

#endif