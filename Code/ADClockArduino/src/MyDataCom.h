#include "Arduino.h"

#define IN_RESPONSE 2
#define IN_DATA 3
#define IN_CLOCK 4

#define OUT_RESPONSE 5
#define OUT_DATA 6
#define OUT_CLOCK 7

class MyDataCom
{

public:
  static void init()
  {
    // Lower Clock Input Pin
    digitalWrite(IN_CLOCK, LOW);
    digitalWrite(OUT_CLOCK, LOW);
    digitalWrite(IN_RESPONSE, LOW);

    // Input als Input definieren
    pinMode(IN_RESPONSE, OUTPUT);
    pinMode(IN_CLOCK, INPUT);
    pinMode(IN_DATA, INPUT);

    // Output als Output definieren
    pinMode(OUT_CLOCK, OUTPUT);
    pinMode(OUT_RESPONSE, INPUT);
    pinMode(OUT_DATA, OUTPUT);
  }

  static void checkForData()
  {
    if (digitalRead(IN_CLOCK) == HIGH)
    {
      reciveData();
    }
    else
    {
      // Serial.println("No data incomming");
    }
  }

  static void sendData(uint8_t data)
  {
    shiftOut(LSBFIRST, data);
    //Serial.println("Sending complete");
  }

private:
  static void reciveData()
  {
    int input = shiftIn(LSBFIRST);
    //Serial.println("Wuppy Data da " + String(input));
  }

  static uint8_t shiftIn(uint8_t bitOrder)
  {
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {
      while (digitalRead(IN_CLOCK) == LOW)
      {
      }
      if (bitOrder == LSBFIRST)
        value |= digitalRead(IN_DATA) << i;
      else
        value |= digitalRead(IN_DATA) << (7 - i);

      digitalWrite(IN_RESPONSE, HIGH);
      while (digitalRead(IN_CLOCK) == HIGH)
      {
      }
      digitalWrite(IN_RESPONSE, LOW);
    }
    return value;
  }

  static void shiftOut(uint8_t bitOrder, uint8_t val)
  {
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
      if (bitOrder == LSBFIRST)
        digitalWrite(OUT_DATA, !!(val & (1 << i)));
      else
        digitalWrite(OUT_DATA, !!(val & (1 << (7 - i))));

      digitalWrite(OUT_CLOCK, HIGH);
      while (digitalRead(OUT_RESPONSE) == LOW)
      {
      }
      digitalWrite(OUT_CLOCK, LOW);
      while (digitalRead(OUT_RESPONSE) == HIGH)
      {
      }
    }
  }
};