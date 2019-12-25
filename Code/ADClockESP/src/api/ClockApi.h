#ifndef _CLOCK_API_H_
#define _CLOCK_API_H_
#include "../Config.h"
#include "../data/ClockWall.h"
#include "../Manager.h"
#include "../data/NTPTime.h"
#include "ApiResponse.h"

extern ClockWall planned;
extern Manager _manager;
extern NTPTime _time;
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
  void updateClock(ApiResponse &response, uint8_t x, uint8_t y, JsonDocument &value);

  void update_hand(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, JsonObject value);

  // Was kann alles beim Handle aktualisiert werden?
  void update_position(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, size_t position);

  void update_direction(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, bool rotation);

  void update_step_delay(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, size_t step_delay);

  void update_wait_degree(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, size_t wait_steps);

  void showCurrentTime(ApiResponse &response);

  void initClocks();

private:
  ClockApi(){};

  // Validierungsroutinen
  bool isValidPosition(size_t position);
};

#endif