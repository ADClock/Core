
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "Arduino.h"
#include "communication/clock/ClockCommunication.h"
#include "data/ClockWall.h"
#include "data/ClockPositions.h"

class Manager
{
public:
  Manager(ClockCommunication &clockcom, ClockPositions &current, ClockWall &aiming, ClockWall &planned);

  void try_step(); // Aktualisiert die current Matrix (nachhalten der Zeigerpositionen)

  bool hasPendingMoves(); // Prüft ob die Zeiger noch nicht auf Ihrer Position angekommen sind.

  void executePlan(); // Verschickt das geplante ClockImage an die Uhren und vermerkt dies

  bool hasPendingPlan(); // Prüft, ob Änderungen geplant sind.

  void init(); // Initalisiert die Matrix

  void preventSendingPlan() { plan_protected = true; }
  void allowSendingPlan() { plan_protected = false; }

  ClockCommunication &clockcom;

  ClockPositions &current;

  ClockWall &aiming;

  ClockWall &planned;

private:
  bool plan_protected; // Wenn der Plan protected ist, wird er nicht direkt ausgeführt. // TODO
};
#endif
