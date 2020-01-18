#include "AnimationManager.h"

void AnimationManager::tick()
{
  if (this->pause)
    return;

  this->animation.tick();
}

/**
 * Loads Animation from SPIFFS.
 * 
 * If there is no animation for given name it does nothing
 * */
void AnimationManager::load(String &name)
{
  this->animation = Animation(name);
}

bool AnimationManager::exists(String &name)
{
  return SPIFFS.exists("/animations/" + name + ".ani");
}
bool AnimationManager::remove(String &name)
{
  if (exists(name))
    return SPIFFS.remove("/animations/" + name + ".ani");
  return false;
}

bool AnimationManager::append_step(String &name, Step &step)
{
  auto file = SPIFFS.open("/animations/" + name + ".ani", FILE_APPEND);
  file.printf("%u/%u;", static_cast<int>(step.type), step.value);
  file.close();
  return true;
}

bool AnimationManager::save_wall(String &name, ClockWall &wall)
{
  Step step;

  // Warten bis vorheriger Plan versendet wurde
  step.type = StepType::WAIT_UNTIL_PLAN_SEND;
  step.value = 0;
  if (!append_step(name, step))
    return false;

  // Plan versenden verbieten
  step.type = StepType::DISABLE_PLAN_SEND;
  step.value = 0;
  if (!append_step(name, step))
    return false;

  // Jede Clock einzeln speichern
  step.type = StepType::SELECT_CLOCK;
  step.value = 1;
  if (!append_step(name, step))
    return false;

  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    step.type = StepType::SELECT_X;
    step.value = x;
    if (!append_step(name, step))
      return false;

    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      step.type = StepType::SELECT_Y;
      step.value = y;
      if (!append_step(name, step))
        return false;

      // Stundenzeiger auswählen
      step.type = StepType::SELECT_HAND;
      step.value = 1; // Stundenzeiger
      if (!append_step(name, step))
        return false;

      // Position des Stundenzeigers abspeichern
      step.type = StepType::TARGET_POS;
      step.value = wall.getClock(x, y).hour.getPositionDegree();
      Serial.printf("Save target pos hour for x=%u y=%u to %u\n", x, y, step.value);
      if (!append_step(name, step))
        return false;

      // Minutenzeiger auswählen
      step.type = StepType::SELECT_HAND;
      step.value = 2; // Minutenzeiger
      if (!append_step(name, step))
        return false;

      // Position des Minutenzeigers abspeichern
      step.type = StepType::TARGET_POS;
      step.value = wall.getClock(x, y).minute.getPositionDegree();
      Serial.printf("Save target pos minute for x=%u y=%u to %u\n", x, y, step.value);
      if (!append_step(name, step))
        return false;
    }
  }

  // Plan versenden erlauben
  step.type = StepType::ENABLE_PLAN_SEND;
  step.value = 0;
  if (!append_step(name, step))
    return false;

  return true;
}