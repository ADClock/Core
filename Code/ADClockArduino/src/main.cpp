#include <Arduino.h>
#include "DataCom.h"
#include "Motor.h"

#define DATA_INTERRUPT 2

#define HALL_DATA_PIN_1 A0
#define HALL_DATA_PIN_2 A1

#define MOTOR_1_PIN_1 8
#define MOTOR_1_PIN_2 9
#define MOTOR_1_PIN_3 10
#define MOTOR_1_PIN_4 11

Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4, HALL_DATA_PIN_1);

void setup()
{
  Serial.begin(9600);
  DataCom::init();

  // Interrupt für Datenempfang
  pinMode(DATA_INTERRUPT, INPUT_PULLUP);
  // attachInterrupt(digitalPinToInterrupt(DATA_INTERRUPT), DataCom::reciveData, CHANGE);

  motor1.start_calibraton();
  while (!motor1.calibrate())
  {
    delay(2);
  }
}

void loop()
{
  Serial.println("Sending data..");
  // DataCom::sendData(42);
  // delay(1000);

  motor1.set_target_pos(1);

  while (1)
  {
    motor1.try_step();
  }
}