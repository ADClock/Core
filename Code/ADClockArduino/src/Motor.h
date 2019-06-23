#include "Arduino.h"

#ifndef _MOTOR_H_
#define _MOTOR_H_

#define MAX_STEPS 2050
#define MIN_STEP_DELAY 4000

class Motor
{
public:
  Motor(size_t pin1, size_t pin2, size_t pin3, size_t pin4, size_t hallPin);
  void step();
  void stepForward();
  void stepBackward();
  void set_target_pos(size_t pos);
  void set_step_delay(uint8_t delay);
  void set_wait_steps(size_t waitSteps);
  void set_direction(bool direction);
  void try_step();
  void reset_position();
  void start_calibraton();
  bool calibrate();

private:
  void write_step();
  
  size_t pin1;
  size_t pin2;
  size_t pin3;
  size_t pin4;
  size_t hall_pin;

  size_t current_pos;
  size_t target_pos;
  size_t wait_steps;
  bool direction;
  size_t step_delay;

  unsigned long last_step;
  size_t coil_state;
  bool calibrated;
  bool calibration_read;
  size_t calibrated_steps;
};

#endif