#ifndef _ANIMATION_MANAGER_H_
#define _ANIMATION_MANAGER_H_

#include "Arduino.h"
#include "SPIFFS.h"
#include "data/Animation.h"
// #include "api/ClockApi.h"

class AnimationManager
{
public:
  void tick();

  void load(String &name);

  void pause_animation() { this->pause = true; };
  void play_animation() { this->pause = false; };

  bool exists(String &name);
  bool remove(String &name);
  bool append_step(String &name, Step &step);
  bool save_wall(String &name, ClockWall &wall);

private:
  Animation animation;
  bool pause;
};

#endif