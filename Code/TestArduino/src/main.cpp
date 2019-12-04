#include "Arduino.h"

void setup()
{
  Serial.begin(9600);
  Serial.println("Booting");

  pinMode(7, OUTPUT);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("Pin 25: " + String(digitalRead(7)) + "  Pin26: " + String(digitalRead(8)) + "  Pin27: " + String(digitalRead(9)));
  digitalWrite(7, digitalRead(9));
  delay(10);
}