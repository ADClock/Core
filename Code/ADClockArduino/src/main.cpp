#include <Arduino.h>
#include "Motor.h"
#include "DataManager.h"
//#include "NewCommunication.h"

#define HALL_DATA_PIN_1 A0
#define HALL_DATA_PIN_2 A1

#define MOTOR_1_PIN_1 10
#define MOTOR_1_PIN_2 11
#define MOTOR_1_PIN_3 12
#define MOTOR_1_PIN_4 13

#define MOTOR_2_PIN_1 6
#define MOTOR_2_PIN_2 7
#define MOTOR_2_PIN_3 8
#define MOTOR_2_PIN_4 9

Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4, HALL_DATA_PIN_1);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4, HALL_DATA_PIN_2);

InputStream in;
OutputStream out;

DataManager com(in, out, motor1, motor2);

void setup()
{
  Serial.begin(9600);

  Serial.println("Setup done");

  // motor1.start_calibraton();
  // motor2.start_calibraton();
  // bool motor1Calibrated = false;
  // bool motor2Calibrated = false;
  // do
  // {
  //   motor1Calibrated = motor1.calibrate();
  //   motor2Calibrated = motor2.calibrate();
  //   delay(10);
  // } while (!motor1Calibrated || !motor2Calibrated);

  motor2.set_target_pos(50);
}

void loop()
{
  com.checkForData();
  motor1.try_step();
  motor2.try_step();

  // delay(90); // Es muss mindestens x ms zwischen zwei Übertragungen gewartet werden, falls es bei der letzten Übertragung einen Fehler gegeben hat.
  // uint8_t image[] = {0x00, 0xA0, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00};

  // long startSending = micros();
  // if (!out.sendData(0x02))
  // {
  //   Serial.println("Sending FAILED");
  //   return;
  // }
  // for (int i = 0; i < 8; i++)
  //   if (!out.sendData(image[i]))
  //   {
  //     Serial.println("Sending FAILED in loop");
  //     return;
  //   }

  // long endSending = micros();
  // Serial.println("Sending done in " + String(endSending - startSending) + " µs");
}
