
#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_
#include "Arduino.h"
#include "InputStream.h"
#include "OutputStream.h"
#include "Motor.h"

class DataManager
{
public:
  DataManager(InputStream &in, OutputStream &out, Motor &motor1, Motor &motor2);

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

  InputStream in;
  OutputStream out;
  Motor motor1;
  Motor motor2;
};
#endif
