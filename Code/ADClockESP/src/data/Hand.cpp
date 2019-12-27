#include "Hand.h"

void Hand::setPositionSteps(size_t steps)
{
  this->position = steps;
}

void Hand::setPositionDegree(size_t degree)
{
  this->position = (degree % 360) * STEPS_PER_DEGREE;
}

// TODO In Clockcom packen
uint8_t *Hand::serialize()
{
  static uint8_t image[4];
  image[0] = (position >> 4) & 0xFF;
  image[1] = ((position & 0x0F) << 4);

  image[1] += ((waitSteps >> 8) & 0x0F);
  image[2] = (waitSteps & 0xFF);

  image[3] = ((stepDelay << 1u) & 0xFE);
  image[3] += (direction & 0x01);

  return image;
}

void Hand::init()
{
  this->position = 0;
  this->direction = 0;
  this->waitSteps = 0;
  this->stepDelay = 0;
}

void Hand::setSimultaneouslyMove(size_t fromStep)
{
  if (this->direction)
  {
    if (fromStep <= this->position)
    {
      setWaitSteps(this->position - fromStep);
    }
    else
    {
      setWaitSteps(STEPS_FOR_CIRCLE - fromStep + this->position);
    }
  }
  else
  {
    if (fromStep >= this->position)
    {
      setWaitSteps(fromStep - this->position);
    }
    else
    {
      setWaitSteps(STEPS_FOR_CIRCLE - this->position + fromStep);
    }
  }
}

void Hand::setDirection(bool direction)
{
  this->direction = direction;
}

void Hand::setWaitSteps(size_t steps)
{
  this->waitSteps = steps;
}

void Hand::setWaitDegree(size_t degree)
{
  this->waitSteps = (degree % 360) * STEPS_PER_DEGREE;
}

void Hand::setDelayBetweenSteps(size_t delay)
{
  this->stepDelay = delay;
}

JsonDocument Hand::asJson()
{
  StaticJsonDocument<JSON_OBJECT_SIZE(4)> v;
  v["p"] = static_cast<int>(this->position);
  v["d"] = this->direction;
  v["w"] = static_cast<int>(this->waitSteps);
  v["s"] = static_cast<int>(this->stepDelay);
  return v;
}

void Hand::update(Hand &hand)
{
  this->position = hand.position;
  this->waitSteps = hand.waitSteps;
  this->stepDelay = hand.stepDelay;
  this->direction = hand.direction;
}

bool Hand::equals(Hand &hand)
{
  return this->position == hand.position &&
         this->waitSteps == hand.waitSteps &&
         this->stepDelay == hand.stepDelay &&
         this->direction == hand.direction;
}

size_t Hand::getPosition()
{
  return this->position;
}

size_t Hand::getWaitSteps()
{
  return this->waitSteps;
}

size_t Hand::getStepDelay()
{
  return this->stepDelay;
}

bool Hand::getDirection()
{
  return this->direction;
}