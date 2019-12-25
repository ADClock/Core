
#ifndef _CLOCKWALL_H_
#define _CLOCKWALL_H_
#include "Arduino.h"
#include "../Config.h"
#include "Clock.h"

enum HandSelection
{
  BOTH,
  HOUR,
  MINUTE
};

class ClockWall
{
public:
  ClockWall();

  bool set_position(HandSelection selection, size_t degree);
  bool set_position(size_t x, size_t y, HandSelection selection, size_t degree);

  bool set_step_delay(HandSelection selection, size_t step_delay);
  bool set_step_delay(size_t x, size_t y, HandSelection selection, size_t step_delay);

  bool set_wait_degree(HandSelection selection, size_t degree);
  bool set_wait_degree(size_t x, size_t y, HandSelection selection, size_t degree);

  bool set_direction(HandSelection selection, bool direction);
  bool set_direction(size_t x, size_t y, HandSelection selection, bool direction);

  bool printChar(uint8_t segment, char c);
  bool show_time(uint8_t hour, uint8_t minute);

  /**
   * Web API Kommunikation
   */

  JsonDocument asJson();
  void update(JsonObject &json);
  void update(ClockWall &wall);

  // Überprüft ob alle Einstellungen gleich sind
  bool equals(ClockWall &wall);
  // Überprüft ob alle Target Positions gleich sind. (Falls nicht, könnte der Plan erneut versendet werden)
  bool different_target(ClockWall &wall);

  /**
  * Hilfsfunktionen für Positionen
  */
  static size_t getClockPosition(size_t x, size_t y);

  Clock &getClock(size_t x, size_t y) { return matrix[x][y]; };

private:
  bool valid_clock_cords(size_t x, size_t y);

  // Matrix der Uhren
  Clock matrix[WALL_SIZE_X][WALL_SIZE_X];
};
#endif
