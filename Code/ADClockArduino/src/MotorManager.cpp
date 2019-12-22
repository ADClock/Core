#include "MotorManager.h"
MotorManager::MotorManager(Motor &motor1, Motor &motor2, Calibration &calibration1, Calibration &calibration2) : motor1(motor1), motor2(motor2), calibration1(calibration1), calibration2(calibration2)
{
}

void MotorManager::calibrate() const
{
  calibration1.start_calibration();
  calibration2.start_calibration();
  bool motor1Calibrated = false;
  bool motor2Calibrated = false;
  size_t stepcounter = 0;
  do
  {
    motor1Calibrated = calibration1.calibrate();
    motor2Calibrated = calibration2.calibrate();
    stepcounter++;
    delay(5);

    if (stepcounter > MAX_STEPS * 3)
    {
#ifdef DEBUG
      Serial.println("moma > Kalibierung abgebrochen.");
#endif
      return;
    }
  } while (!motor1Calibrated || !motor2Calibrated);
}

void MotorManager::try_step() const
{
  this->motor1.try_step();
  this->motor2.try_step();
}

void MotorManager::set_motor_data(MotorData data[2]) const
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
  Serial.println("m1=" + String(data[0].position) + " m2=" + String(data[1].position));
  // Serial.println("MotorManager >> Motorendaten neu gesetzt.");
#endif
}