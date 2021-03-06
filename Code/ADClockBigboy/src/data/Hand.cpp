#include "Hand.h"

void Hand::setPositionSteps(size_t steps)
{
  this->position = steps;
}

void Hand::setPositionDegree(size_t degree)
{
  this->position = degree * STEPS_PER_DEGREE;
}

// TODO In Clockcom packen
u_int8_t *Hand::serialize()
{
  static u_int8_t image[4];
  // Stepper 1: Hour
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
  this->direction = RIGHT;
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

void Hand::setDelayBetweenSteps(size_t delay)
{
  this->stepDelay = delay;
}

MbedJSONValue Hand::asJson()
{
  MbedJSONValue v;
  v["pos"] = static_cast<int>(this->position);
  v["direction"] = this->direction;
  v["waiting"] = static_cast<int>(this->waitSteps);
  v["delay"] = static_cast<int>(this->stepDelay);
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