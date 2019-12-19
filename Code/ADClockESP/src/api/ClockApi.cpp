#include "ClockApi.h"

void ClockApi::updateClock(ApiResponse &response, uint8_t x, uint8_t y, JSONValue &value)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Clock not processed.");
    return;
  }

  // Hands aus JSON auslesen und verarbeiten
  if (value.hasMember("hour"))
  {
    updateHand(response, x, y, HOUR_HANDLE, value["hour"]);
  }

  if (value.hasMember("minute"))
  {
    updateHand(response, x, y, MINUTE_HANDLE, value["minute"]);
  }

  return;
}

void ClockApi::updateHand(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, JSONValue &value)
{
  if (!isValidCoordinates(x, y))
  {
    response.error("Clock position (" + String(x) + "/" + String(y) + ") is invalid. Hand not processed.");
    return;
  }
  // Neue Position updaten
  if (value.hasMember("target_pos"))
  {
    if (value["target_pos"].getType() == JSONValue::Type::TypeInt)
    {
      updateHandPosition(response, x, y, handId, value["target_pos"].get<int>());
    }
    else
    {
      response.error("Invalid data-type for 'target_pos'");
    }
  }

  // Neue Richtung setzen
  if (value.hasMember("direction"))
  {
    if (value["direction"].getType() == JSONValue::Type::TypeBoolean)
    {
      updateHandRotation(response, x, y, handId, value["direction"].get<boolean>());
    }
    else
    {
      response.error("Invalid data-type for 'direction'");
    }
  }

  // Step delay updaten
  if (value.hasMember("step_delay"))
  {
    if (value["step_delay"].getType() == JSONValue::Type::TypeBoolean)
    {
      updateHandStepDelay(response, x, y, handId, value["step_delay"].get<int>());
    }
    else
    {
      response.error("Invalid data-type for 'step_delay'");
    }
  }

  // Wait steps updaten
  if (value.hasMember("wait_steps"))
  {
    if (value["wait_steps"].getType() == JSONValue::Type::TypeBoolean)
    {
      updateHandStepDelay(response, x, y, handId, value["wait_steps"].get<int>());
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

bool ClockApi::isValidPosition(size_t position)
{
  return position <= 360;
}

void ClockApi::initClocks()
{
  this->datamanager().init();
}