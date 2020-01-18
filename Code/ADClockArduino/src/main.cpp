#include <Arduino.h>
#include "Config.h"
#include "DataManager.h"

Motor motor1(MOTOR_1_PIN_1, MOTOR_1_PIN_2, MOTOR_1_PIN_3, MOTOR_1_PIN_4);
Motor motor2(MOTOR_2_PIN_1, MOTOR_2_PIN_2, MOTOR_2_PIN_3, MOTOR_2_PIN_4);

Calibration calibration1(motor1, HALL_DATA_PIN_1);
Calibration calibration2(motor2, HALL_DATA_PIN_2);

MotorManager moma(motor1, motor2, calibration1, calibration2);

BitBuffer outbuffer;
BitBuffer inbuffer;
DataSender sender(outbuffer);
DataReceiver receiver(inbuffer);

DataManager com(sender, outbuffer, receiver, inbuffer, moma);

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

// Triggers on Clock-Signal from DataReceiver
void isr_data_receiving()
{
  if (FastGPIO::Pin<IN_CLOCK>::isInputHigh())
  {
    receiver.read_next_bit();
  }
  else
  {
    receiver.receive_clock_off();
  }
}

// Triggers on Response-Signal from DataSender
void isr_data_sending()
{
  if (FastGPIO::Pin<OUT_RESPONSE>::isInputHigh())
  {
    sender.receive_response_on();
  }
  else
  {
    sender.receive_response_off();
  }
}

void setup()
{

  Serial.begin(115200);
  Serial.println("Setup");
#ifdef DEBUG
  Serial.begin(115200);
#endif
#ifdef DEBUG
  Serial.println("Setup...");
  // testPinSpeed();
  //testDataTransferSpeed();
  // testBuffer();
#endif

  moma.calibrate();

  // ISR for DataReceiver
  attachInterrupt(digitalPinToInterrupt(IN_CLOCK), isr_data_receiving, CHANGE);
#ifndef IS_LAST_CLOCK
  // ISR for DataSender
  attachInterrupt(digitalPinToInterrupt(OUT_RESPONSE), isr_data_sending, CHANGE);
#endif

#ifdef DEBUG
  // rotateUntilTomorrow();
  Serial.println("Setup done!");
#endif
}

void loop()
{
  com.tick();
  moma.try_step();
}
