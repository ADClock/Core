#include "Motor.h"

Motor::Motor(size_t pin1, size_t pin2, size_t pin3, size_t pin4, size_t hallPin)
{
  this->pin1 = pin1;
  this->pin2 = pin2;
  this->pin3 = pin3;
  this->pin4 = pin4;
  this->hall_pin = hallPin;
  this->step_delay = 2;
  this->coil_state = 1;

  pinMode(pin1, OUTPUT);
  pinMode(pin2, OUTPUT);
  pinMode(pin3, OUTPUT);
  pinMode(pin4, OUTPUT);

  pinMode(hall_pin, INPUT_PULLUP);

  digitalWrite(pin1, HIGH);
}

void Motor::set_target_pos(size_t pos)
{
  this->target_pos = pos;
  Serial.println("New target pos " + pos);
}

void Motor::step()
{
  this->current_pos++;
  if (this->current_pos > MAX_STEPS)
  {
    this->current_pos = 0;
  }

  switch (this->coil_state)
  {
  case 1:
    digitalWrite(pin2, HIGH);
    digitalWrite(pin1, LOW);
    break;
  case 2:
    digitalWrite(pin3, HIGH);
    digitalWrite(pin2, LOW);
    break;
  case 3:
    digitalWrite(pin4, HIGH);
    digitalWrite(pin3, LOW);
    break;
  case 4:
    digitalWrite(pin1, HIGH);
    digitalWrite(pin4, LOW);
    break;
  }

  this->coil_state++;
  if (this->coil_state > 4)
  {
    this->coil_state = 1;
  }
}

void Motor::try_step()
{
  if (this->current_pos < this->target_pos)
  {
    auto time_since_step = millis() - this->last_step;
    if (time_since_step > this->step_delay)
    {
      this->last_step = millis();
      this->step();
    }
  }
  else
  {
    digitalWrite(pin1, LOW);
    digitalWrite(pin2, LOW);
    digitalWrite(pin3, LOW);
    digitalWrite(pin4, LOW);
  }
}

void Motor::reset_position()
{
  this->current_pos = 0;
}

void Motor::start_calibraton()
{
  this->calibrated = false;
}

bool Motor::calibrate()
{
  if (this->calibrated)
    return true;

  this->step();
  this->current_pos = 0;
  if (analogRead(this->hall_pin) < 100)
  {
    this->calibrated = true;
    return true;
  }
  return false;
}