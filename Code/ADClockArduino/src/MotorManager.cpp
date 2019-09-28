#include "MotorManager.h"

MotorManager::MotorManager(Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2) : motor1(motor1), motor2(motor2), calibration1(calibration1), calibration2(calibration2)
{
}

void MotorManager::calibrate()
{
  calibration1.start_calibration();
  calibration2.start_calibration();
  bool motor1Calibrated = false;
  bool motor2Calibrated = false;
  do
  {
    motor1Calibrated = calibration1.calibrate();
    motor2Calibrated = calibration2.calibrate();
    delay(10);
  } while (!motor1Calibrated || !motor2Calibrated);
}

void MotorManager::try_step()
{
  this->motor1.try_step();
  this->motor2.try_step();
}

void MotorManager::setMotorData(DataStruct data[2])
{
  motor1.set_target_pos(data[0].position);
  motor1.set_step_delay(data[0].delay);
  motor1.set_wait_steps(data[0].waitSteps);
  motor1.set_direction(data[0].direction);

  motor2.set_target_pos(data[1].position);
  motor2.set_step_delay(data[1].delay);
  motor2.set_wait_steps(data[1].waitSteps);
  motor2.set_direction(data[1].direction);

#ifdef DEBUG
  Serial.println("MotorManager >> Motorendaten neu gesetzt.");
#endif
}