

#ifndef _INPUTSTREAM_H_
#define _INPUTSTREAM_H_

#include "Arduino.h"

class InputStream
{
public:
  InputStream(uint8_t clockPin, uint8_t dataPin, uint8_t responsePin);

  // Prüft ob ein Signal an der Clockleitung anliegt.
  bool hasData();

  // Liest ein Byte aus
  byte readData();

private:
  // Liest das Datenbit ein
  int readDataBit();
  // Verschickt die Meldung, dass Daten gelesen wurden.
  bool sendDataReadingComplete();

  // wait for clock
  bool waitForClock();

  uint8_t clockPin;
  uint8_t dataPin;
  uint8_t responsePin;
};

#endif
