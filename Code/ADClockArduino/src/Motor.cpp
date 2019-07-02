#include "Motor.h"
#include "FastGPIO.h"

void quickWrite(uint8_t pin, bool state)
{
  switch (pin)
  {
  case 6:
    FastGPIO::Pin<6>::setOutputValue(state);
    break;
  case 7:
    FastGPIO::Pin<7>::setOutputValue(state);
    break;
  case 8:
    FastGPIO::Pin<8>::setOutputValue(state);
    break;
  case 9:
    FastGPIO::Pin<9>::setOutputValue(state);
    break;
  case 10:
    FastGPIO::Pin<10>::setOutputValue(state);
    break;
  case 11:
    FastGPIO::Pin<11>::setOutputValue(state);
    break;
  case 12:
    FastGPIO::Pin<12>::setOutputValue(state);
    break;
  case 13:
    FastGPIO::Pin<13>::setOutputValue(state);
    break;

  default:
    break;
  }
}

Motor::Motor(size_t pin1, size_t pin2, size_t pin3, size_t pin4, size_t hallPin)
{
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;
  this->hall_pin = hallPin;
  this->step_delay = 4;
  this->coil_state = 1;

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  pinMode(hall_pin, INPUT_PULLUP);

  this->allPinsOff();

  this->step_delay = MIN_STEP_DELAY;
  this->direction = true; // Vorwärts drehen?
}

void Motor::set_target_pos(size_t pos)
{
  if (pos < 0 || pos > MAX_STEPS)
  {
    Serial.println("Wrong Target Pos.");
    return;
  }
  this->target_pos = pos;
}

void Motor::set_wait_steps(size_t waitSteps)
{
  this->wait_steps = waitSteps;
}

void Motor::set_step_delay(uint8_t delay)
{
  this->step_delay = MIN_STEP_DELAY + delay * 100;
}

void Motor::set_direction(bool direction)
{
  this->direction = direction;
}

void Motor::stepForward()
{
  this->current_pos++;
  if (this->current_pos > MAX_STEPS)
  {
    this->current_pos = 0;
  }

  this->write_step();

  this->coil_state++;
  if (this->coil_state > 4)
  {
    this->coil_state = 1;
  }
}

void Motor::stepBackward()
{
  this->current_pos--;
  if (this->current_pos < 0)
  {
    this->current_pos = MAX_STEPS;
  }

  this->write_step();

  this->coil_state--;
  if (this->coil_state < 1)
  {
    this->coil_state = 4;
  }
}

void Motor::write_step()
{
  switch (this->coil_state)
  {
  case 1:
    quickWrite(pin2, HIGH);
    quickWrite(pin1, LOW);
    break;
  case 2:
    quickWrite(pin3, HIGH);
    quickWrite(pin2, LOW);
    break;
  case 3:
    quickWrite(pin4, HIGH);
    quickWrite(pin3, LOW);
    break;
  case 4:
    quickWrite(pin1, HIGH);
    quickWrite(pin4, LOW);
    break;
  }
}

void Motor::step()
{
  // Serial.println("Stepping.. Direction " + String(direction) + " CurrentPos " + String(current_pos) + " heading to " + String(target_pos));
  if (direction)
  {
    stepForward();
  }
  else
  {
    stepBackward();
  }
}

void Motor::try_step()
{
  if (this->current_pos != this->target_pos)
  {
    auto time_since_step = micros() - this->last_step;
    if (time_since_step > this->step_delay || time_since_step < 0)
    {
      this->last_step = micros();
      if (this->wait_steps > 0)
      {
        this->wait_steps--;
      }
      else
      {
        this->step();
      }
    }
  }
  else
  {
    this->allPinsOff();
  }
}

void Motor::reset_position()
{
  this->current_pos = 0;
}

void Motor::start_calibraton()
{
  this->step_delay = MIN_STEP_DELAY;
  this->direction = true; // Vorwärts drehen?
  this->calibrated = false;
  this->calibration_read = false;
  this->calibrated_steps = 0;
  this->calibration_was_outside = analogRead(hall_pin) >= 100;
}

void Motor::allPinsOff()
{
  quickWrite(pin1, LOW);
  quickWrite(pin2, LOW);
  quickWrite(pin3, LOW);
  quickWrite(pin4, LOW);
}

bool Motor::calibrate()
{
  if (this->calibrated)
    return true;

  if (this->calibration_read)
  {
    if (analogRead(this->hall_pin) < 100)
    {
      this->stepForward();
      this->calibrated_steps++;
      return false;
    }
    else
    {
      for (size_t i = 0; i < this->calibrated_steps / 2; i++)
      {
        this->stepBackward();
        delay(10);
      }
      this->reset_position();
      this->calibrated = true;
      this->allPinsOff();
      return true;
    }
  }
  else
  {
    if (this->calibration_was_outside)
    {
      this->stepForward();
      if (analogRead(this->hall_pin) < 100)
      {
        this->calibration_read = true;
      }
    }
    else
    {
      if (analogRead(this->hall_pin) >= 100)
      {
        this->calibration_was_outside = true;
      }
    }
    return false;
  }
  return false;
}