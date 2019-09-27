
#ifndef _MOTORMANAGER_H_
#define _MOTORMANAGER_H_
#include "Arduino.h"
#include "Calibration.h"

struct DataStruct
{
  uint16_t position;
  uint16_t waitSteps;
  int8_t delay;
  bool direction;
};

class MotorManager
{
public:
  MotorManager(Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2);

  void calibrate();

  void try_step();

  void setMotorData(DataStruct data[2]);

private:
  Motor &motor1;
  Motor &motor2;
  Calibration &calibration1;
  Calibration &calibration2;
};
#endif
