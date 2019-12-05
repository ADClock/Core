#ifndef _CLOCK_API_H_
#define _CLOCK_API_H_

#include "Arduino.h"
#include "Manager.h"
#include "ApiResponse.h"

#define MINUTE_HANDLE 1
#define HOUR_HANDLE 2

class ClockApi
{
public:
  static ClockApi &instance()
  {
    static ClockApi INSTANCE;
    return INSTANCE;
  }

  bool isValidCoordinates(uint8_t x, uint8_t y)
  {
    return WALL_SIZE_X > x && WALL_SIZE_Y > y;
  }

  // Updates a hole Clock
  void updateClock(ApiResponse &response, uint8_t x, uint8_t y, JSONValue &value);

  // Updates a single Handle
  void updateHand(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, JSONValue &value);

  // Was kann alles beim Handle aktualisiert werden?
  void updateHandPosition(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, size_t position);

  void updateHandRotation(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, bool rotation);

  void updateHandStepDelay(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, size_t step_delay);

  void updateHandWaitStep(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, size_t wait_steps);

  void initClocks();

  // Manager undsooo
  void setManager(Manager *m)
  {
    this->manager = m;
  }

  Manager datamanager()
  {
    return *manager;
  }

private:
  ClockApi(){};

  // Validierungsroutinen
  bool isValidPosition(size_t position);

  Manager *manager;
};

#endif