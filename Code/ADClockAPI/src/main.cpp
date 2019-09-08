#include <WiFi.h>
#include "web/RequestHandler.h"
#include "api/ClockApi.h"
#include "data/ClockMatrix.h"

// WLAN Verbindung zu einem Netzwerk herstellen
const char *ssid = "Home_Net";
const char *password = "@xxxx";

HttpServer server(RequestHandler::handlers, NULL);

void setup()
{
  Serial.begin(9600);
  Serial.println("Booting");

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Connection Failed! Rebooting...");
    delay(5000);
    ESP.restart();
  }

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.begin();

  Serial.println("HTTP-Server now online!");
}

void loop()
{
  server.process();
}