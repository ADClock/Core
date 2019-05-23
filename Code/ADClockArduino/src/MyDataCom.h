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
  MyDataCom()
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

  void checkForData()
  {
    if (digitalRead(IN_CLOCK) == HIGH)
    {
      recieveData();
    }
    else
    {
      // Serial.println("No data incomming");
    }
  }

  void sendData(uint8_t data)
  {
    shiftOut(LSBFIRST, data);
    //Serial.println("Sending complete");
  }

private:
  void recieveData()
  {
    int input = shiftIn(LSBFIRST);
    //Serial.println("Wuppy Data da " + String(input));

    if (input == 0x01)
    {
      // Init
    }
    else if (input == 0x02)
    {
      // Image lesen, erste Clock Stellung lesen und Rest weiterschicken.
      recieveClockImage();
      shiftOut(LSBFIRST, 0x02);
      shiftInShiftOut();
    }
  }

  void recieveClockImage()
  {
    // 8 Byte lesen (4 je Motor)
    //     -- Position  (16 Bit)
    // -- Delay     (8 Bit)
    // -- Speed     (7 Bit)
    // -- Direction (1 Bit)
    for (int i = 0; i < 8; i++)
    {
      shiftIn(LSBFIRST);
    }
  }

  void shiftInShiftOut()
  {
    // Solange an der Clock ein Signal anliegt..
    while (waitForSignalChange(IN_CLOCK, LOW))
    {
      sendBit(readBit());
    }
  }

  bool waitForSignalChange(uint8_t pin, int signal)
  {
    int counter = 0;
    while (digitalRead(pin) == signal)
    {
      if (counter++ > 100000)
        return false;
    }
    return true;
  }

  uint8_t shiftIn(uint8_t bitOrder)
  {
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i)
    {

      if (bitOrder == LSBFIRST)
        value |= readBit() << i;
      else
        value |= readBit() << (7 - i);
    }
    return value;
  }

  void shiftOut(uint8_t bitOrder, uint8_t val)
  {
    uint8_t i;

    for (i = 0; i < 8; i++)
    {
      if (bitOrder == LSBFIRST)
        sendBit(!!(val & (1 << i)));
      else
        sendBit(!!(val & (1 << (7 - i))));
    }
  }

  int readBit()
  {
    // Warten  bis Clock an ist..
    if (waitForSignalChange(IN_CLOCK, LOW))
    {
      Serial.println("[shiftIn] No clock signal (no high)");
      return 0x00;
    }
    int bit = digitalRead(IN_DATA);

    // Mitteilen, dass lesen erfolgreich war
    digitalWrite(IN_RESPONSE, HIGH);
    // Warten das Clock ausgeht
    if (waitForSignalChange(IN_CLOCK, HIGH))
    {
      Serial.println("[shiftIn] No clock reset to low");
      return 0x00;
    }
    // Response wieder ausschalten
    digitalWrite(IN_RESPONSE, LOW);

    return bit;
  }

  /**
 * Versendet einen Bit, setzt anschließend die Clock und wartet darauf, dass das Bit gelesen wurde.
 * */
  void sendBit(int bit)
  {
    digitalWrite(OUT_DATA, bit);
    digitalWrite(OUT_CLOCK, HIGH);
    if (waitForSignalChange(OUT_RESPONSE, LOW))
    {
      Serial.println("[shiftOut] No response recieved");
      return;
    }
    digitalWrite(OUT_CLOCK, LOW);
    if (waitForSignalChange(OUT_RESPONSE, LOW))
    {
      Serial.println("[shiftOut] Response doesnt reset");
      return;
    }
  }
};