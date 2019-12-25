#include <WiFi.h>
#include <WiFiAP.h>
#include "SPIFFS.h"
#include "web/RequestHandler.h"
#include "Settings.h"

// Daten Uhren
ClockWall aiming;
ClockWall planned;
ClockPositions current;

// Kommunikation Uhren
BitBuffer bufferout;
DataSender clockout(bufferout);
ClockCommunication clockcom(bufferout, clockout);

// Manager
Manager _manager(clockcom, current, aiming, planned);

// Animation Manager
AnimationManager _animations;

HttpServer server(RequestHandler::handlers, NULL);

// Aktuelle Uhrzeit
NTPTime _time;

void setup_wifi_connection()
{

  auto ap_ssid = "ADClock";
  WiFi.softAP(ap_ssid);
  Serial.println("[WiFi] Hosting AP:    " + String(ap_ssid));
  Serial.print("[WiFi] Local Address: ");
  Serial.println(WiFi.softAPIP());

  if (!Settings::has_wifi_config())
  {
    Serial.println("[WiFi] No WiFi Configuration found!");
    return;
  }

  WiFi.mode(WIFI_AP_STA);

  // TODO: Wifi kann automatisch wiederverbunden werden.
  Settings::WiFiSettings config = Settings::load_wifi_config(); // {"Home_Net", "@zwerge99"}; //
  Serial.println("Connecting to '" + String(config.ssid) + "' with password '" + String(config.password) + "'");

  WiFi.begin(config.ssid.c_str(), config.password.c_str());

  size_t status = WiFi.waitForConnectResult();
  if (status != WL_CONNECTED)
  {
    // TODO Das Drinlassen oder rausnehemn?
    if (status == WL_NO_SSID_AVAIL)
    {
      // Settings::delete_wifi_config();
      Serial.println("Failed to read wifi data");
      return;
    }
    Serial.println("Connection failed with Code " + String(status) + "\nRestarting...");
    ESP.restart();
  }

  Serial.print("Connected to: ");
  Serial.println(WiFi.SSID());
  Serial.print("IP Address:   ");
  Serial.println(WiFi.localIP());
}

void setup()
{
  Serial.begin(115200);
  Serial.println("[STARTUP] Booting...");

  // test_json();
  // test_communication_speed();

  if (!SPIFFS.begin(true))
  {
    Serial.println("[SETUP] An Error has occurred while mounting SPIFFS");
    return;
  }

  setup_wifi_connection();

  _manager.init();
  if (!clockcom.transmit())
  {
    Serial.println("[SETUP] Error starting ADClock! Restarting..");
    ESP.restart();
  }

  _time.load_time();

  server.begin();
  Serial.println("[SETUP] HTTP-Server online...");

  /* // Ab jetzt dürfen die Uhren gestellt werden
  manager.allowSendingPlan();
  Serial.println("[STARTUP] Enabled direct plan sending...");
  // Die Zeilen machen das gleiche,.. manger.planned.setMinutePosition(0, 5, 180);
  ClockApi::instance().datamanager().planned.setMinutePosition(0, 5, 180);
  ClockApi::instance().datamanager().planned.setMinutePosition(0, 4, 90);
  ClockApi::instance().datamanager().planned.setHourPosition(0, 5, 270);
  ClockApi::instance().datamanager().planned.setHourPosition(0, 4, 45);
  Serial.println("[STARTUP] Set basic image...");*/
  Serial.println("[SETUP] Finshed!");
}

void loop()
{
  // HTTP Anfragen frühstücken
  server.process();

  // Uhrdaten aktuell halten
  _manager.try_step();

  _animations.tick();

  // Sending data, if there is anything left
  if (!bufferout.is_empty())
    clockcom.transmit();

  // Erst den Plan verschicken, wenn die Uhr initialisiert ist.
  if (!_manager.isInitialized())
    return;

  if (_manager.hasPendingPlan())
  {
    // Erst wenn ein Plan abgeschlossen ist den nächsten senden
    if (!_manager.hasPendingMoves())
    {
      _manager.executePlan();
    }
  }
}