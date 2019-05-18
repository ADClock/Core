#include <Arduino.h>

#define HALL_DATA_PIN_1 A0
#define HALL_DATA_PIN_2 A1

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  auto a = analogRead(HALL_DATA_PIN_1);
  Serial.println(a);
}