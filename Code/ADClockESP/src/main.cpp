#include <WiFi.h>
#include "Manager.h"
#include "web/RequestHandler.h"

// Daten Uhren
ClockWall aiming;
ClockWall planned;
ClockPositions current;

// Kommunikation Uhren
BitBuffer bufferout;
DataSender clockout(bufferout);
ClockCommunication clockcom(bufferout, clockout);

// Manager
Manager manager(clockcom, current, aiming, planned);

// WLAN Verbindung zu einem Netzwerk herstellen
const char *ssid = "Home_Net";
const char *password = "@zwerge99";

HttpServer server(RequestHandler::handlers, NULL);

void setup_wifi_connection()
{
  Serial.print("Establish WiFi connection");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  size_t attempts = 0;
  while (WiFi.status() != WL_CONNECTED)
  {
    if (attempts > 10)
    {
      Serial.println("Failed! Restarting...");
      ESP.restart();
      return;
    }
    Serial.print(".");
    delay(500);
    //WiFi.reconnect();
    attempts++;
  }

  Serial.print("\nConnected to: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
}

void test_json()
{
  // Test Json
  StaticJsonDocument<JSON_OBJECT_SIZE(10)> doc;
  StaticJsonDocument<JSON_OBJECT_SIZE(2)> obj;
  obj["abc"] = "foo";
  obj["def"] = 42;
  doc["ohh"] = obj;
  doc["ooo"] = true;
  serializeJson(doc, Serial);
}

void test_communication_speed()
{
  long start = 0;
  long end = 0;

  Serial.println("Toggling");
  start = micros();

  for (size_t i = 0; i < 10000; i++)
  {
    digitalWrite(18, 1);
    digitalWrite(18, 0);
  }
  end = micros();
  Serial.println("10000 Pin-Toggles took " + String(end - start) + " µs");

  double total_bits = 0;
  double total_time = 0;
  Serial.println("Speedtesting...");
  for (size_t i = 0; i < 10; i++)
  {
    // Preparing
    clockcom.cleanup_communication();
    clockcom.sendCommand(0x03); // 8 bit command
    // Maximum: 1000 sonst Buffer größer stellen
    for (size_t i = 0; i < 576; i++)
    {
      clockcom.sendByte(0x42);
    }
    total_bits += 8 + 576 * 8;
    start = micros();
    if (!clockcom.transmit())
    {
      Serial.println("Übertragung fehlgeschlagen. Testergebnis ungültig!");
    }
    end = micros();
    long current_time = end - start;
    total_time += current_time;
    Serial.println("Transmission " + String(i) + " took " + String(current_time) + " µs");
  }

  Serial.println("Speedtest finished\nTransmitted " + String(total_bits) + " bits in " + String(total_time) + " µs\nSpeed: " + String(total_bits / (total_time / 1000. / 1000.)) + " bits/second");
}

void setup()
{
  Serial.begin(9600);
  Serial.println("[STARTUP] Booting...");
  pinMode(OUT_RESPONSE, INPUT);

  // test_json();
  // test_communication_speed();

  Serial.println("[STARTUP] Begin Clock init...");
  manager.preventSendingPlan();
  manager.init();
  if (!clockcom.transmit())
  {
    Serial.println("[STARTUP] Error: Clocks not listening!\n[STARTUP] Restarting!");
    ESP.restart();
  }
  Serial.println("[STARTUP] Clocks initalizing...");

  setup_wifi_connection();

  server.begin();

  Serial.println("[STARTUP] HTTP-Server online...");

  ClockApi::instance().setManager(&manager);
  ClockApi::instance().time().load_time();
  Serial.println("[STARTUP] API initalized...");

  // Ab jetzt dürfen die Uhren gestellt werden
  manager.allowSendingPlan();
  Serial.println("[STARTUP] Enabled direct plan sending...");
  // Die Zeilen machen das gleiche,.. manger.planned.setMinutePosition(0, 5, 180);
  ClockApi::instance().datamanager().planned.setMinutePosition(0, 5, 180);
  ClockApi::instance().datamanager().planned.setMinutePosition(0, 4, 90);
  ClockApi::instance().datamanager().planned.setHourPosition(0, 5, 270);
  ClockApi::instance().datamanager().planned.setHourPosition(0, 4, 45);
  Serial.println("[STARTUP] Set basic image...");
  Serial.println("[STARTUP] Finshed!");
}

void loop()
{
  // HTTP Anfragen frühstücken
  server.process();

  // Uhrdaten aktuell halten
  manager.try_step();

  // Sending data, if there is anything left
  if (!bufferout.is_empty())
    clockcom.transmit();

  // Erst den Plan verschicken, wenn die Uhr initalisiert ist.
  if (!manager.isInitialized())
    return;

  if (manager.hasPendingPlan())
  {
    // Erst wenn ein Plan abgeschlossen ist den nächsten senden
    if (!manager.hasPendingMoves())
    {
      manager.executePlan();
    }
    else
    {
      Serial.println("Es gibt bereits einen neuen Plan, aber die Zeiger drehen noch!");
    }
  }
}