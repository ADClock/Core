#include "ClockApi.h"

/**
 * Updates Clock by JsonData
 * 
 * Json must contain 'both', 'hour' or 'minute' Json Hand Data
 * 
 * Example:
 * {
 *    "hour": {
 *        "target_pos": 180,
 *        "direction": false,
 *        "step_delay": 0,
 *        "wait_steps": 0
 *    },
 *    "minute": {
 *        "target_pos": 90,
 *        "direction": false
 *    }
 * }
 * 
 * */
void ClockApi::updateClock(ApiResponse &response, uint8_t x, uint8_t y, JsonDocument &value)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Clock not processed.");
    return;
  }
  if (value.containsKey("both"))
  {
    update_hand(response, x, y, HOUR, value["both"]);
  }

  if (value.containsKey("hour"))
  {
    update_hand(response, x, y, HOUR, value["hour"]);
  }

  if (value.containsKey("minute"))
  {
    update_hand(response, x, y, MINUTE, value["minute"]);
  }

  return;
}

void ClockApi::update_hand(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, JsonObject value)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand not processed.");
    return;
  }
  // Neue Position updaten
  if (value.containsKey("target_pos"))
  {
    if (value["target_pos"].is<int>())
    {
      update_position(response, x, y, hand, value["target_pos"].as<int>());
    }
    else
    {
      response.error("Invalid data-type for 'target_pos'");
    }
  }

  // Neue Richtung setzen
  if (value.containsKey("direction"))
  {
    if (value["direction"].is<boolean>())
    {
      update_direction(response, x, y, hand, value["direction"].as<boolean>());
    }
    else
    {
      response.error("Invalid data-type for 'direction'");
    }
  }

  // Step delay updaten
  if (value.containsKey("step_delay"))
  {
    if (value["step_delay"].is<int>())
    {
      update_step_delay(response, x, y, hand, value["step_delay"].as<int>());
    }
    else
    {
      response.error("Invalid data-type for 'step_delay'");
    }
  }

  // Wait steps updaten
  if (value.containsKey("wait_steps"))
  {
    if (value["wait_steps"].is<int>())
    {
      update_wait_degree(response, x, y, hand, value["wait_steps"].is<int>());
    }
    else
    {
      response.error("Invalid data-type for 'wait_steps'");
    }
  }

  return;
}

void ClockApi::update_position(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, size_t degree)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand position not processed.");
    return;
  }

  if (!isValidPosition(degree))
  {
    response.error("Degree (" + String(degree) + ") is invalid. Hand position not processed.");
    return;
  }

  planned.set_position(x, y, hand, degree);
  response.inform("Position of hand was updated");

  return;
}

void ClockApi::update_direction(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, bool rotation)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand rotation not processed.");
    return;
  }

  planned.set_direction(x, y, hand, rotation);
  response.inform("Rotation of hand was updated");

  return;
}

void ClockApi::update_step_delay(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, size_t step_delay)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand step delay not processed.");
    return;
  }

  planned.set_step_delay(x, y, hand, step_delay);
  response.inform("Step delay of hand was updated");

  return;
}

void ClockApi::update_wait_degree(ApiResponse &response, uint8_t x, uint8_t y, HandSelection hand, size_t wait_steps)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand wait steps not processed.");
    return;
  }

  planned.set_wait_degree(x, y, hand, wait_steps);
  response.inform("Wait steps of hand was updated");

  return;
}

void ClockApi::showCurrentTime(ApiResponse &response)
{

  if (!_time.is_time_set())
  {
    _time.load_time();
  }
  planned.show_time(_time.get_hour(), _time.get_minute());
  response.inform("Current time set on each clock.");
}

bool ClockApi::isValidPosition(size_t position)
{
  return position <= 360;
}

void ClockApi::initClocks()
{
  _manager.init();
}