#include <Arduino.h>
#include "Motor.h"
#include "DataManager.h"
//#include "NewCommunication.h"

#define HALL_DATA_PIN_1 A0
#define HALL_DATA_PIN_2 A1

#define MOTOR_1_PIN_1 2
#define MOTOR_1_PIN_2 3
#define MOTOR_1_PIN_3 4
#define MOTOR_1_PIN_4 5

#define MOTOR_2_PIN_1 8
#define MOTOR_2_PIN_2 9
#define MOTOR_2_PIN_3 10
#define MOTOR_2_PIN_4 11

#define IN_RESPONSE A3
#define IN_DATA A4
#define IN_CLOCK A5

#define OUT_RESPONSE 5
#define OUT_DATA 6
#define OUT_CLOCK 7

Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4, HALL_DATA_PIN_1);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4, HALL_DATA_PIN_2);

InputStream in(IN_CLOCK, IN_DATA, IN_RESPONSE);
OutputStream out(OUT_CLOCK, OUT_DATA, OUT_RESPONSE);

DataManager com(in, out, motor1, motor2);

void setup()
{
  Serial.begin(9600);
  // MyDataCom::init();
  Serial.println("Setup done");

  pinMode(IN_RESPONSE, OUTPUT);
  pinMode(OUT_DATA, OUTPUT);
  pinMode(OUT_CLOCK, OUTPUT);

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

  // delay(1000); // Es muss mindestens x ms zwischen zwei Übertragungen gewartet werden, falls es bei der letzten Übertragung einen Fehler gegeben hat.
  // uint8_t image[] = {0x00, 0xA0, 0x00, 0x00, 0x00, 0x50, 0x00, 0x00};

  // if (!out.sendData(0x02))
  // {
  //   Serial.println("Sending FAILED");
  //   return;
  // }
  // long startSending = millis();
  // for (int i = 0; i < 8; i++)
  //   if (!out.sendData(image[i]))
  //   {
  //     Serial.println("Sending FAILED in loop");
  //     return;
  //   }

  // long endSending = millis();
  // Serial.println("Sending done in " + String(endSending - startSending) + " ms");

  motor2.set_target_pos(50);
}

void loop()
{
  // /*
  com.checkForData();
  motor1.try_step();
  motor2.try_step();
  com.delayAndCheck(10);
  // */
}
