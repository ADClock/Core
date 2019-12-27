#ifndef _REST_CONFIG_H_
#define _REST_CONFIG_H_
#include "Arduino.h"
#include "WebUtils.h"
#include "Settings.h"
namespace RESTConfig
{

// POST /wifi
boolean wifi_post()
{
  ApiResponse response;
  auto doc = WebUtils::getJsonBody(response);
  // TODO Check if ssid and password is present
  Settings::WiFiSettings config = {doc["ssid"], doc["password"]};
  Settings::set_wifi_config(config);
  response.inform("Wifi config set");

  WebUtils::finishRequest(response);
  return true;
};

// DELETE /wifi
boolean wifi_delete()
{
  ApiResponse response;
  Settings::delete_wifi_config();
  response.inform("Wifi config deleted");
  WebUtils::finishRequest(response);
  return true;
};
}; // namespace RESTConfig

#endif