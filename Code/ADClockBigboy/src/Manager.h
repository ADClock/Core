
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "mbed.h"
#include "com/clock/ClockCommunication.h"
#include "data/ClockWall.h"
#include "data/ClockPositions.h"
#include "com/esp/ApiCommunication.h"

class Manager
{
public:
  Manager(ClockCommunication &clockcom, ApiCommunication &apicom, ClockPositions &current, ClockWall &aiming, ClockWall &planned);

  void try_step(); // Aktualisiert die current Matrix (nachhalten der Zeigerpositionen)

  bool hasPendingMoves(); // Prüft ob die Zeiger noch nicht auf Ihrer Position angekommen sind.

  void executePlan(); // Verschickt das geplante ClockImage an die Uhren und vermerkt dies

  bool hasPendingPlan(); // Prüft, ob Änderungen geplant sind.

  void init(); // Initalisiert die Matrix

  void preventSendingPlan() { plan_protected = true; }
  void allowSendingPlan() { plan_protected = false; }

  // Api Kommunikation mit dem ESP
  void notify_api();
  void read_api();

private:
  ClockCommunication &clockcom;
  ApiCommunication &apicom;

  ClockPositions &current;

  ClockWall &aiming;

  ClockWall &planned;
  bool plan_protected; // Wenn der Plan protected ist, wird er nicht direkt ausgeführt. // TODO
};
#endif
