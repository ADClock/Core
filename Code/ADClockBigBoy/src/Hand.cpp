#include "Hand.h"

bool Hand::hasPendingMove()
{
  return this->position != this->next_position;
}

void Hand::move()
{
  this->position = this->next_position;
  this->next_waitSteps = 0;
}

void Hand::setNextPositionSteps(size_t steps)
{
  this->next_position = steps;
}

void Hand::setNextPositionDegree(size_t degree)
{
  this->next_position = degree * STEPS_PER_DEGREE;
}

u_int8_t *Hand::nextStepAsImage()
{
  static u_int8_t image[4];
  // Stepper 1: Hour
  image[0] = (next_position >> 4) & 0xFF;
  image[1] = ((next_position & 0x0F) << 4);

  image[1] += ((next_waitSteps >> 8) & 0x0F);
  image[2] = (next_waitSteps & 0xFF);

  image[3] = (next_stepDelay & 0xFE);
  image[3] += (next_direction & 0x01);

  return image;
}

size_t Hand::getPendingSteps()
{
  auto stepCount = next_waitSteps;
  if (this->next_direction)
  {
    // Forward
    if (this->next_position < this->position)
    {
      stepCount += STEPS_FOR_CIRCLE - this->position + this->next_position;
    }
    else
    {
      stepCount += this->next_position - this->position;
    }
  }
  else
  {
    // Backwards
    if (this->next_position > this->position)
    {
      stepCount += this->position + (STEPS_FOR_CIRCLE - this->next_position);
    }
    else
    {
      stepCount += this->position - this->next_position;
    }
  }
  return stepCount;
}

long Hand::getCalculateMoveTime()
{
  return getPendingSteps() * (this->next_stepDelay + MIN_STEP_DELAY);
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