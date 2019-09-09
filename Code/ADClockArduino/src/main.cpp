#include <Arduino.h>
#include "Calibration.h"
#include "DataManager.h"

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

Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4);

Calibration calibration1(motor1, HALL_DATA_PIN_1);
Calibration calibration2(motor2, HALL_DATA_PIN_2);

InputStream in;
OutputStream out;

DataManager com(in, out, motor1, motor2, calibration1, calibration2);

// Wir steppen im Kreis und zählen wie viele Schritte es von Hall High bis Hall High ist.
void rotateUntilTomorrow()
{
  bool state = false; // Ist der Pin High or Low?
  bool lastState = false;
  size_t steps = 0;
  while (1)
  {
    motor2.step();
    steps++;
    lastState = state;
    state = analogRead(HALL_DATA_PIN_2) < 100;
    if (lastState && !state)
    {
      Serial.println("Round took " + String(steps) + " steps.");
      steps = 0;
    }
    delay(4);
  }
}

void testDataTransferSpeed()
{
  long startTime = millis();
  out.sendData(0x03); // Command Speedtest
  for (int i = 0; i < 100.000; i++)
  {
    out.sendData(0x42);
  }
  long end = millis();
  Serial.println("100.000 Bytes took " + String(end - startTime) + "ms to send.");
  delay(1000);
}

void setup()
{
  Serial.begin(9600);

  Serial.println("Setup done");

  com.calibrate();

  // rotateUntilTomorrow();
}

void loop()
{
  com.checkForData();
  motor1.try_step();
  motor2.try_step();

  // testDataTransferSpeed();
}
