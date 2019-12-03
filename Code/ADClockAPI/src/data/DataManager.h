#ifndef _DATAMANAGER_H_
#define _DATAMANAGER_H_
#include "Arduino.h"
#include "ClockWall.h"
#include "ClockPositions.h"
#include "../communication/DataSender.h"
#include "../communication/DataReceiver.h"

class DataManager
{
public:
  bool hasPendingMoves(); // Prüft ob die Zeiger noch nicht auf Ihrer Position angekommen sind.
  bool hasPendingPlan();  // Prüft, ob Änderungen geplant sind.

  bool hasIncommingData(); //
  void read_data();
  void send_plan();

  void init(); // Inits this shit.

private:
  DataSender &sender;
  DataReceiver &receiver;

  ClockPositions &current;

  ClockWall &aiming;

  ClockWall &planned;
};

#endif