#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "SPIFFS.h"

namespace Settings
{
struct WiFiSettings
{
  String ssid;
  String password;
};

static bool has_wifi_config()
{
  return SPIFFS.exists("/wifi.config");
}

static void delete_wifi_config()
{
  if (has_wifi_config())
    SPIFFS.remove("/wifi.config");
}

static WiFiSettings load_wifi_config()
{
  auto wf = SPIFFS.open("/wifi.config");
  auto ssid = wf.readStringUntil(';');
  auto password = wf.readString();
  // password[terminator] = 0;
  wf.close();
  Serial.print("ssid: ");
  Serial.println(ssid);
  Serial.print("PW: ");
  Serial.println(password);
  return {ssid, password};
}

static void set_wifi_config(WiFiSettings &settings)
{
  delete_wifi_config();

  auto file = SPIFFS.open("/wifi.config", "w");
  file.print(settings.ssid);
  file.print(';');
  file.print(settings.password);
  file.close();
  Serial.println("WIFI Config has been updated.");
}

}; // namespace Settings

#endif