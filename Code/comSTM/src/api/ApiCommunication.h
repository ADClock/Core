
#ifndef _APICOMM_H_
#define _APICOMM_H_
#include "mbed.h"
#include "OutputStream.h"

class ApiCommunication
{
public:
  ApiCommunication(OutputStream &out);

  // Delayed um entsprechende ms und prüft ob Daten anliegen
  bool sendInitCommand();

  bool sendImage(const u_int8_t arr[], const size_t &length);

private:
  bool sendCommand(const u_int8_t &command);

  OutputStream &out;

  long lastSend;
};
#endif
