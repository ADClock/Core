#include "Arduino.h"

#ifndef _MOTOR_H_
#define _MOTOR_H_

#define MAX_STEPS 1705
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
  void allPinsOff();

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
  bool calibrated;              // Ist der Motor kalibriert?
  bool calibration_read;        // Ist der Magnet im Magnetfeld? (wird für Zentrierung benötigt.)
  bool calibration_was_outside; // Der Magnet muss mindestens einmal aus dem Bereich raus sein, damit er danach sauber reinlaufen kann
  size_t calibrated_steps;      // Anzahl der Schritte, die der Magnet im Magnetfeld ist.
};

#endif