#include <Arduino.h>
// #include "MyDataCom.h"
#include "NewCommunication.h"
#include "Motor.h"

#define HALL_DATA_PIN_1 A0
#define HALL_DATA_PIN_2 A1

#define MOTOR_1_PIN_1 0
#define MOTOR_1_PIN_2 1
#define MOTOR_1_PIN_3 2
#define MOTOR_1_PIN_4 3

#define MOTOR_2_PIN_1 4
#define MOTOR_2_PIN_2 5
#define MOTOR_2_PIN_3 6
#define MOTOR_2_PIN_4 7

Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4, HALL_DATA_PIN_1);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4, HALL_DATA_PIN_1);

NewCommunication com;

void setup()
{
  Serial.begin(9600);
  // MyDataCom::init();
  Serial.println("Setup done");

  // motor1.start_calibraton();
  // while (!motor1.calibrate() || !motor2.calibrate())
  // {
  //   delay(2);
  // }
}

void loop()
{
  // MyDataCom::checkForData();
  // /*for (int i = 0; i < 500; i++)
  // {
  //   MyDataCom::sendData(42);
  // }
  // Serial.println("500 byte send");*/

  // motor1.set_target_pos(1);

  // while (1)
  // {
  //   motor1.try_step();
  // }

  com.write(0x7A);
  delay(10);

  // Serial.println((char)com.read());
  // com.read();
}
