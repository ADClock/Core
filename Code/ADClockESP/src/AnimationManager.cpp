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