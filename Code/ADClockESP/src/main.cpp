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

void setup()
{
  Serial.begin(9600);
  Serial.println("Booting");
  pinMode(OUT_RESPONSE, INPUT);
  // +++++++++++++++++++++++++
  // Pintest
  // pinMode(OUT_RESPONSE, INPUT);
  // Serial.println("Respone Pin says: " + String(digitalRead(OUT_RESPONSE)) + " should be 1");
  // Serial.println("Pintest Clock On");
  // digitalWrite(OUT_CLOCK, 1);
  // delay(1000);
  // Serial.println("Respone Pin says: " + String(digitalRead(OUT_RESPONSE)) + " should be 1");
  // delay(1000);
  // digitalWrite(OUT_CLOCK, 0);
  // Serial.println("Pintest Data On");
  // digitalWrite(OUT_DATA, 1);
  // delay(1000);
  // Serial.println("Respone Pin says: " + String(digitalRead(OUT_RESPONSE)) + " should be 0");
  // delay(1000);
  // digitalWrite(OUT_DATA, 0);
  // Serial.println("Pintest complete");
  // +++++++++++++++++++++++++

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

  // Uhren vorbereiten
  manager.preventSendingPlan();
  manager.init();
  clockcom.tramsmit(); // TODO Das gescheiter lösen..

  ClockApi::instance().setManager(&manager);
  Serial.println("Clock API initalisiert!");

  // Ab jetzt dürfen die Uhren gestellt werden
  manager.allowSendingPlan();
  Serial.println("Ab jetzzt können sich die Zeiger drehen.. ");
  manager.planned.setMinutePosition(0, 5, 180);
}

void loop()
{
  // HTTP Anfragen frühstücken
  server.process();

  // Uhrdaten aktuell halten
  manager.try_step();

  // Sending data
  clockout.tick();

  if (manager.hasPendingPlan())
  {
    // Erst wenn ein Plan abgeschlossen ist den nächsten senden
    if (!manager.hasPendingMoves())
    {
      manager.executePlan();
    }
  }
}