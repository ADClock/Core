
#ifndef _CLOCKCOM_H_
#define _CLOCKCOM_H_
#include "mbed.h"
#include "../Config.h"
#include "ClockOutputStream.h"
#include "../data/ClockWall.h"

class ClockCommunication
{
public:
  ClockCommunication(ClockOutputStream &out);

  // Delayed um entsprechende ms und prüft ob Daten anliegen
  bool sendInitCommand();

  bool sendPlan(ClockWall &plan);

    void printResult();

#ifdef DEBUG
  void performSpeedtest();
#endif

private:
  bool sendCommand(const u_int8_t &command);

  size_t getClockX(size_t &position);
  size_t getClockY(size_t &position);

  ClockOutputStream &out;

  long lastSend;
};
#endif
