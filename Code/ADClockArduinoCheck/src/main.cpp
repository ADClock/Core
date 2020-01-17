#include <Arduino.h>
#include "Config.h"
#include "MotorManager.h"
Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4);

Calibration calibration1(motor1, HALL_DATA_PIN_1);
Calibration calibration2(motor2, HALL_DATA_PIN_2);

MotorManager moma(motor1, motor2, calibration1, calibration2);

#ifdef DEBUG
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
  unsigned long startTime = micros();
  com.send_command(0x03); // Command Speedtest
  for (int i = 0; i < 1000; i++)
  {
    com.tick();
    com.send_byte(0x42);
  }
  unsigned long end = micros();
  Serial.println("1000 Bytes took " + String(end - startTime) + " µs to send.");
  delay(1000);
}

void testPinSpeed()
{
  unsigned long startTime = micros();
  for (int i = 0; i < 1000; i++)
  {
    FastGPIO::Pin<4>::setOutputValue(1);
    FastGPIO::Pin<4>::setOutputValue(0);
  }
  unsigned long end = micros();
  Serial.println("1000 x Pin on off took " + String(end - startTime) + " µs.");
}

void testBuffer()
{

  Serial.println("Buffer size (expected 0) = " + String(outbuffer.size()));
  for (size_t i = 0; i < 200; i++)
  {
    outbuffer.enqueue(true);
  }
  Serial.println("Buffer size (expected 200) = " + String(outbuffer.size()));

  for (size_t y = 0; y < 10; y++)
  {
    for (size_t i = 0; i < 75; i++)
    {
      outbuffer.dequeue();
      outbuffer.enqueue(true);
    }
    Serial.println("Buffer size (expected 200) = " + String(outbuffer.size()));
  }
}
#endif

void setup()
{

  Serial.begin(115200);
  Serial.println("Setup...");

  Serial.println("Setup done");
}

unsigned long last_step_motor1;
unsigned long last_step_motor2;

void loop()
{
  if ((millis() - last_step_motor1) > (calibration1.isInField() ? 20 : 5))
  {
    motor1.stepForward();
    last_step_motor1 = millis();
  }
  if ((millis() - last_step_motor2) > (calibration2.isInField() ? 20 : 5))
  {
    motor2.stepForward();
    last_step_motor2 = millis();
  }
}
