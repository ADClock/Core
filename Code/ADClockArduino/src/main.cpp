#include <Arduino.h>
#include "DataCom.h"

#define DATA_INTERRUPT 2

#define HALL_DATA_PIN_1 A0
#define HALL_DATA_PIN_2 A1

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  DataCom::init();

  // Interrupt für Datenempfang
  pinMode(DATA_INTERRUPT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(DATA_INTERRUPT), DataCom::reciveData, CHANGE);
}

void loop()
{
  // put your main code here, to run repeatedly:
  Serial.println("Sending data..");
  DataCom::sendData(42);
  delay(1000);
}