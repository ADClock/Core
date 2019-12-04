#include "ClockApi.h"

void ClockApi::updateClock(ApiResponse &response, uint8_t x, uint8_t y, JSONValue &value)
{
  if (!isValidCoordinates(x, y))
  {
    // TODO X + Y wären cool
    response.error("Clock position is invalid. Clock not processed.");
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
    // TODO X + Y wären cool
    response.error("Clock position is invalid. Hand not processed.");
    return;
  }
  // Neue Position updaten
  if (value.hasMember("target_pos"))
  {
    updateHandPosition(response, x, y, handId, value["target_pos"].get<int>());
  }

  // Neue Richtung setzen
  if (value.hasMember("direction"))
  {
    updateHandRotation(response, x, y, handId, value["direction"].get<boolean>());
  }

  return;
}

void ClockApi::updateHandPosition(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, size_t degree)
{
  if (!isValidCoordinates(x, y))
  {
    // TODO X + Y wären cool
    response.error("Clock position is invalid. Hand position not processed.");
    return;
  }

  if (!isValidPosition(degree))
  {
    // TODO degree wären cool
    response.error("Degree is invalid. Hand position not processed.");
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

  Serial.println("Hand updated targetPos to " + String(degree));
  return;
}

void ClockApi::updateHandRotation(ApiResponse &response, uint8_t x, uint8_t y, uint8_t handId, bool rotation)
{
  if (!isValidCoordinates(x, y))
  {
    // TODO X + Y wären cool
    response.error("Clock position is invalid. Hand rotation not processed.");
    return;
  }

  if (handId == HOUR_HANDLE)
  {
    // this->datamanager().planned.setDirection(rotation);
    response.inform("Rotation of hour hand was updated");
  }
  else
  {
    // this->data.matrix[x][y].minute.setDirection(rotation);
    response.inform("Rotation of minute hand was updated");
  }

  Serial.println("Hand updated direction to " + String(rotation));
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