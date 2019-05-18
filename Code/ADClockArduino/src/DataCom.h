#include "Arduino.h"

#define IN_DATA 3
#define IN_CLOCK 4
#define OUT_DATA 5
#define OUT_CLOCK 6
#define OUT_INTERRUPT 7

class DataCom
{

public:
  static void init()
  {
    // Lower Clock Input Pin
    digitalWrite(IN_CLOCK, LOW);
    digitalWrite(OUT_CLOCK, LOW);
  }

  static void sendData(int data)
  {
    digitalWrite(OUT_INTERRUPT, HIGH);
    shiftOut(OUT_DATA, OUT_CLOCK, LSBFIRST, data);
    digitalWrite(OUT_INTERRUPT, LOW);
  }

  static void reciveData()
  {
    Serial.print("Data incomming.. ");
    byte incomming = shiftIn(IN_DATA, IN_CLOCK, LSBFIRST);
    Serial.println(incomming);
  }
};