
#ifndef _DATASENDER_H
#define _DATASENDER_H
#include "mbed.h"
#include "OutputStream.h"

#define COMMAND_INIT 0x01
#define COMMAND_IMAGE 0x02

// TODO Nochmal richtigen Wert einstellen
#define DELAY_BETWEEN_COMMANDS 1

class DataSender
{
public:
  DataSender(OutputStream &out);

  // Delayed um entsprechende ms und prüft ob Daten anliegen
  bool sendInitCommand();

  bool sendImage(const u_int8_t arr[], const size_t &length);

private:
  bool sendCommand(const u_int8_t &command);

  OutputStream &out;

  long lastSend;
};
#endif
