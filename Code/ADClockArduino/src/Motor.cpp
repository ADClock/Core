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

  quickWrite(pin1, HIGH);
}

void Motor::set_target_pos(size_t pos)
{
  this->target_pos = pos;
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

  this->coil_state++;
  if (this->coil_state > 4)
  {
    this->coil_state = 1;
  }
}

void Motor::try_step()
{
  if (this->current_pos != this->target_pos)
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
    quickWrite(pin1, LOW);
    quickWrite(pin2, LOW);
    quickWrite(pin3, LOW);
    quickWrite(pin4, LOW);
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

  Serial.println(analogRead(this->hall_pin));

  if (analogRead(this->hall_pin) < 100)
  {
    this->calibrated = true;
    return true;
  }
  return false;
}
