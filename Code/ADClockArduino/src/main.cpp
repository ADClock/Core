#include <Arduino.h>
#include "Config.h"
#include "DataManager.h"

Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4);

Calibration calibration1(motor1, HALL_DATA_PIN_1);
Calibration calibration2(motor2, HALL_DATA_PIN_2);

MotorManager moma(motor1, motor2, calibration1, calibration2);

InputStream in;
OutputStream out;

DataManager com(in, out, moma);

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

#ifdef DEBUG
  Serial.println("Setup done");
#endif

  moma.calibrate();

  // rotateUntilTomorrow();
}

void loop()
{
  com.checkForData();
  moma.try_step();

  // testDataTransferSpeed();
}
