
#ifndef _DATASENDER_H
#define _DATASENDER_H
#include "Arduino.h"
#include "OutputStream.h"

#define COMMAND_INIT 0x01
#define COMMAND_IMAGE 0x02

#define DELAY_BETWEEN_COMMANDS 1 // TODO Nochmal richtigen Wert einstellen

class DataSender
{
public:
  DataSender(OutputStream &out);

  // Delayed um entsprechende ms und prüft ob Daten anliegen
  bool sendInitCommand();

  bool sendImage(const byte arr[], const size_t &length);

private:
  bool sendCommand(const byte &command);

  OutputStream &out;

  unsigned long lastSend;
};
#endif
