

#ifndef _INPUTSTREAM_H_
#define _INPUTSTREAM_H_

#include "Arduino.h"

class InputStream
{
public:
  InputStream();

  // Prüft ob ein Signal an der Clockleitung anliegt.
  bool hasData();

  // Liest ein Byte aus
  byte readData();

  // waits short time for data
  bool waitForData();

private:
  // Liest das Datenbit ein
  int readDataBit();
  // Verschickt die Meldung, dass Daten gelesen wurden.
  bool sendDataReadingComplete();

  bool _hasData; // Cache der Clock-Leitung um eine wiederholte Abfrage des Pins zu verhindern.
};

#endif
