#ifndef _CALIBRATION_H_
#define _CALIBRATION_H_

#include "Arduino.h"
#include "Motor.h"

enum CalibrationState
{
  LEAVEMAGNET,
  FINDMAGNET,
  INFIELD,
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
  size_t steps;
};

#endif