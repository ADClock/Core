#include "ClockApi.h"

void ClockApi::updateClock(ApiResponse &response, uint8_t x, uint8_t y, JsonDocument &value)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Clock not processed.");
    return;
  }

  // Hands aus JSON auslesen und verarbeiten
  if (value.containsKey("hour"))
  {
    updateHand(response, x, y, HOUR_HANDLE, value["hour"]);
  }

  if (value.containsKey("minute"))
  {
    updateHand(response, x, y, MINUTE_HANDLE, value["minute"]);
  }

  return;
}

void ClockApi::updateHand(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, JsonObject value)
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
      updateHandPosition(response, x, y, handId, value["target_pos"].as<int>());
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
      updateHandRotation(response, x, y, handId, value["direction"].as<boolean>());
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
      updateHandStepDelay(response, x, y, handId, value["step_delay"].as<int>());
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
      updateHandStepDelay(response, x, y, handId, value["wait_steps"].is<int>());
    }
    else
    {
      response.error("Invalid data-type for 'wait_steps'");
    }
  }

  return;
}

void ClockApi::updateHandPosition(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, size_t degree)
{
  if (!isValidCoordinates(x, y))
  {
    // TODO X + Y wären cool
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand position not processed.");
    return;
  }

  if (!isValidPosition(degree))
  {
    response.error("Degree (" + String(degree) + ") is invalid. Hand position not processed.");
    return;
  }

  if (handId == HOUR_HANDLE)
  {
    this->datamanager().planned.setHourPosition(x, y, degree);
    response.inform("Position of hour hand was updated");
  }
  else
  {
    this->datamanager().planned.setMinutePosition(x, y, degree);
    response.inform("Position of minute hand was updated");
  }

  return;
}

void ClockApi::updateHandRotation(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, bool rotation)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand rotation not processed.");
    return;
  }

  if (handId == HOUR_HANDLE)
  {
    this->datamanager().planned.setHourDirection(x, y, rotation);
    response.inform("Rotation of hour hand was updated");
  }
  else
  {
    this->datamanager().planned.setMinuteDirection(x, y, rotation);
    response.inform("Rotation of minute hand was updated");
  }

  return;
}

void ClockApi::updateHandStepDelay(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, size_t step_delay)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand step delay not processed.");
    return;
  }

  if (handId == HOUR_HANDLE)
  {
    this->datamanager().planned.setHourStepDelay(x, y, step_delay);
    response.inform("Step delay of hour hand was updated");
  }
  else
  {
    this->datamanager().planned.setMinuteStepDelay(x, y, step_delay);
    response.inform("Step delay of minute hand was updated");
  }

  return;
}

void ClockApi::updateHandWaitStep(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, size_t wait_steps)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand wait steps not processed.");
    return;
  }

  if (handId == HOUR_HANDLE)
  {
    this->datamanager().planned.setHourWaitSteps(x, y, wait_steps);
    response.inform("Wait steps of hour hand was updated");
  }
  else
  {
    this->datamanager().planned.setMinuteWaitSteps(x, y, wait_steps);
    response.inform("Wait steps of minute hand was updated");
  }

  return;
}

void ClockApi::showCurrentTime(ApiResponse &response)
{
  if (!this->time().is_time_set())
  {
    this->time().load_time();
  }
  auto hour = 360 - 360. / 12 * (this->time().get_hour() % 12);
  auto minute = 360 - 360. / 60 * this->time().get_minute();
  this->datamanager().planned.setMutiplePositions(0, 0, WALL_SIZE_X - 1, WALL_SIZE_Y - 1, hour, minute);
  response.inform("Current time set on each clock.");

  Serial.println("Hands set to hour=" + String(hour) + " minute=" + String(minute));
}

bool ClockApi::isValidPosition(size_t position)
{
  return position <= 360;
}

void ClockApi::initClocks()
{
  this->datamanager().init();
}