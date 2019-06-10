

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

  // waits short time for data
  bool waitForData();

private:
  // Liest die Clockleitung aus
  bool readClock();

  // Liest das Datenbit ein
  int readDataBit();
  // Verschickt die Meldung, dass Daten gelesen wurden.
  bool sendDataReadingComplete();

  uint8_t clockPin;
  uint8_t dataPin;
  uint8_t responsePin;
  bool _hasData; // Cache der Clock-Leitung um eine wiederholte Abfrage des Pins zu verhindern.
};

#endif
