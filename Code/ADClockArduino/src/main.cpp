#include <Arduino.h>
#include "MyDataCom.h"

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  MyDataCom::init();
  Serial.println("Setup done");
}

void loop()
{
  MyDataCom::checkForData();
  /*for (int i = 0; i < 500; i++)
  {
    MyDataCom::sendData(42);
  }
  Serial.println("500 byte send");*/
}
