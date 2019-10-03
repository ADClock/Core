#include "Manager.h"
#include "SpeedCheck.h"
SpeedCheck execu{"> execute plan: "};

Manager::Manager(ClockCommunication &clockcom, ApiCommunication &apicom, ClockPositions &current, ClockWall &aiming, ClockWall &planned) : clockcom(clockcom), apicom(apicom), current(current), aiming(aiming), planned(planned)
{
}

// Initalisiert die Uhren
void Manager::init()
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      // Initalisierung des aktuellen Images
      this->current.initClockPosition(x, y);

      // Die Zeiger laufen aktuell zur init-Position
      this->aiming.getClock(x, y).init();
    }
  }

  if (!clockcom.sendInitCommand())
  {
#ifdef DEBUG
    Debug::println("Manager >> Init Command nicht verschickt");
#endif
  }
  else
  {
    Debug::println("Manager >> Init Command verschickt");
  }
}

// Führt den Plan aus.
// - Verschickt den Plan an die Clocks.
// - Setzt Aiming entsprechend
void Manager::executePlan()
{
  execu.start();
  if (this->clockcom.sendPlan(this->planned))
  {
    // Den Plan als Goal setzen
    this->aiming.update(this->planned);
    // Motordaten entsprechend aktualisieren (z.B. waitSteps übernehmen)
    this->current.update(this->aiming);
#ifdef DEBUG
    // Debug::println("Manager >> Sending done. Executing plan...");
#endif
  }
  else
  {
#ifdef DEBUG
    // Debug::println("Manager >> Failed to send plan.");
#endif
  }

  execu.stop();
  execu.printResult();
}

bool Manager::hasPendingMoves()
{

  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      if (!this->current.isOnPosition(x, y, this->aiming.getClock(x, y)))
        return true;
    }
  }
  return false;
}

bool Manager::hasPendingPlan()
{
  return !plan_protected && !this->aiming.equals(this->planned);
}

void Manager::try_step()
{
  for (size_t x = 0; x < WALL_SIZE_X; x++)
  {
    for (size_t y = 0; y < WALL_SIZE_Y; y++)
    {
      this->current.try_step(x, y, this->aiming.getClock(x, y));
    }
  }
}