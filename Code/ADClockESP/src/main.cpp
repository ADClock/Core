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
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    if (attempts > 3)
    {
      Serial.println("Failed! Restarting...");
      ESP.restart();
      return;
    }
    Serial.print(".");
    delay(1000);
    WiFi.reconnect();
    attempts++;
  }

  Serial.print("\nConnected to: ");
  Serial.println(ssid);
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());
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
    if (!clockcom.tramsmit())
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

  test_communication_speed();

  setup_wifi_connection();

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
  if (!bufferout.is_empty())
    Serial.println("Buffer remaining bits " + String(bufferout.size()));

  if (manager.hasPendingPlan())
  {
    // Erst wenn ein Plan abgeschlossen ist den nächsten senden
    if (!manager.hasPendingMoves())
    {
      manager.executePlan();
    }
  }
}