
#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_
#include "Arduino.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "MotorManager.h"

class DataManager
{
public:
  DataManager(InputStream &in, OutputStream &out, MotorManager &moma);

  // Delayed um entsprechende ms und prüft ob Daten anliegen
  void delayAndCheck(size_t ms);

  void checkForData();

private:
  // Liest die Daten ein. Anhand des ersten Bits wird entschieden was gemacht werden soll.
  void reciveData();

  // List das eigene Clockimage ein
  void readMyClockImage();

  // Versendet alle folgenden Daten an den nächsten Controller
  void pipeIncommingData();

  DataStruct deserialze(uint8_t stream[4]);

  InputStream &in;
  const OutputStream &out;
  MotorManager &moma;
};
#endif
