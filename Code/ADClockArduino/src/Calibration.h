#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "Arduino.h"
#include "Motor.h"

#define MIN_STEPS_OUTSIDE_FIELD 20

class Calibration
{
public:
  Calibration(Motor &m, size_t hall_pin);
  void start_calibration();
  bool calibrate();

private:
  bool isInField();
  bool centerInField();

  Motor motor;
  size_t hall_pin;

  bool calibrated;         // Ist der Motor kalibriert?
  bool field_read;         // Wurde das gesamte Magnetfeld durchquert?
  bool should_go_outside;  // Ist der Idiot schon im Magnetfeld und will trotzdem kalibrieren?
  bool was_in_field;       // War der Motor nun bereits mindestens einmal im Feld?
  size_t ouside_steps;     // Anzahl der Schritte außerhalb des Magnetfelds
  size_t calibrated_steps; // Anzahl der Schritte, die für das Durchqueren des Feldes benötigt wird.
};

#endif