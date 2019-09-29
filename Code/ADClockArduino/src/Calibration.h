#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "Arduino.h"
#include "Motor.h"

#define MIN_STEPS_OUTSIDE_FIELD 20

enum CalibrationState
{
  LEAVEMAGNET,
  FINDMAGNET,
  INAPPEAL,
  CENTERING,
  CALIBRATED
};

class Calibration
{
public:
  Calibration(Motor &m, size_t hall_pin);
  void start_calibration();
  bool calibrate();

private:
  bool isInField();
  bool centerInField();

  Motor &motor;
  size_t hall_pin;

  CalibrationState state;
  size_t steps; // Anzahl der Schritte. Wird je nach State anders verwendet.
};

#endif