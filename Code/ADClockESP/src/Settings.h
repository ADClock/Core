#ifndef _SETTINGS_H_
#define _SETTINGS_H_

#include "SPIFFS.h"

namespace Settings
{
struct WiFiSettings
{
  const char *ssid;
  const char *password;
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
  char ssid[64];
  char password[64];
  size_t terminator;
  terminator = wf.readBytesUntil('\n', ssid, sizeof(ssid));

  Serial.println("Length ssid: " + String(terminator));
  //ssid[terminator] = 0;
  terminator = wf.readBytesUntil('\n', password, sizeof(password));
  Serial.println("Length password: " + String(terminator));
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
  file.println(settings.ssid);
  file.println(settings.password);
  file.close();
  Serial.println("WIFI Config has been updated.");
}

}; // namespace Settings

#endif