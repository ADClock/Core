#include "Arduino.h"

#define IN_DATA 4
#define IN_CLOCK 3

#define OUT_DATA 5
#define OUT_CLOCK 6

class NewCommunication
{
public:
  NewCommunication()
  {
    pinMode(IN_CLOCK, INPUT);
    pinMode(IN_DATA, INPUT);

    pinMode(OUT_CLOCK, OUTPUT);
    pinMode(OUT_DATA, OUTPUT);

    digitalWrite(OUT_CLOCK, LOW);
    digitalWrite(OUT_DATA, LOW);
  }

  void write(byte data)
  {
    for (size_t i = 0; i < 8; i++)
    {
      digitalWrite(OUT_DATA, !!(data & (1 << i)));
      digitalWrite(OUT_CLOCK, HIGH);
      delayMicroseconds(5);
      digitalWrite(OUT_CLOCK, LOW);
      delayMicroseconds(5);
    }
  }

  byte read()
  {
    byte inByte = 0x00;
    for (size_t i = 0; i < 8; ++i)
    {
      while (!digitalRead(IN_CLOCK))
      {
      }
      inByte |= digitalRead(IN_DATA) << i;
      while (digitalRead(IN_CLOCK))
      {
      }
    }
    if (inByte == 0x7A)
    {
      a++;
      if (a == 500)
      {
        Serial.println("rdy");
        a = 0;
      }
    }
    else
    {
      Serial.println("ERROR");
    }
    return inByte;
  }

private:
  int a = 0;
};