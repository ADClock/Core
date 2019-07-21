#include "Hand.h"

void Hand::setNextPositionSteps(size_t steps)
{
  this->next_position = steps;
}

void Hand::setNextPositionDegree(size_t degree)
{
  this->next_position = degree * STEPS_PER_DEGREE;
}

void Hand::init()
{
  this->position = 0;
  this->next_position = 0;
  this->next_direction = RIGHT;
  this->next_waitSteps = 0;
  this->next_stepDelay = 0;
}

void Hand::setSimultaneouslyMove(size_t fromStep)
{
  if (this->next_direction)
  {
    if (fromStep <= this->position)
    {
      setNextWaitSteps(this->position - fromStep);
    }
    else
    {
      setNextWaitSteps(STEPS_FOR_CIRCLE - fromStep + this->position);
    }
  }
  else
  {
    if (fromStep >= this->position)
    {
      setNextWaitSteps(fromStep - this->position);
    }
    else
    {
      setNextWaitSteps(STEPS_FOR_CIRCLE - this->position + fromStep);
    }
  }
}

void Hand::setDirection(bool direction)
{
  this->next_direction = direction;
}

void Hand::setNextWaitSteps(size_t steps)
{
  this->next_waitSteps = steps;
}

void Hand::setNextDelayBetweenSteps(size_t delay)
{
  this->next_stepDelay = delay;
}

JSONValue Hand::asJson()
{
  JSONValue v;
  v["pos"] = static_cast<int>(this->position);
  v["target_pos"] = static_cast<int>(this->next_position);
  v["direction"] = this->next_direction;
  v["waiting"] = static_cast<int>(this->next_waitSteps);
  v["delay"] = static_cast<int>(this->next_stepDelay);
  return v;
}