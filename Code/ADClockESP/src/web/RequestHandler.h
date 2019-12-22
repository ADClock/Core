#ifndef _REQUESTHANDLER_H_
#define _REQUESTHANDLER_H_
#include "Arduino.h"
#include "Settings.h"
#include "HttpServer.h"
#include "../api/ClockApi.h"
#include "../api/ApiResponse.h"

namespace RequestHandler
{
void finishRequest(HttpServer &server, ApiResponse &response)
{
  Serial.println("Finished Http-Request with " + String(response.getHttpCode()));
  server.send_error_code(response.getHttpCode());
  server.send_content_type("application/json");
  server.end_headers();
  serializeJson(response.getJson(), server);
  response.getJson().clear(); // free memory
}

JsonDocument &getJsonBody(HttpServer &server, ApiResponse &response)
{
  String body = server.get_client().readString();

  if (body.length() == 0)
  {
    response.error("No body was sent.");
  }
  else if (body.length() >= 32000)
  {
    response.error("The body was long. Limit is 32000 bytes.");
  }

  char bodyArray[body.length() + 1];
  body.toCharArray(bodyArray, body.length() + 1);

  // 234 = JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + 110;
  static StaticJsonDocument<258> doc;
  auto error = deserializeJson(doc, body);
  if (error)
  {
    response.error(String(error.c_str()));
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return doc;
  }

  return doc;
}

boolean index(HttpServer &server)
{
  server.send_error_code(200);
  server.send_content_type("text/html");
  server.end_headers();
  if (!SPIFFS.exists("/index.html"))
  {
    server.print(F("<html><body>Chris is doof :)</body></html>\n"));
    return true;
  }
  auto file = SPIFFS.open("/index.html");
  server.print(file.readString());
  file.close();
  return true;
}

boolean init_get(HttpServer &server)
{
  server.send_error_code(200);
  server.send_content_type("text/html");
  server.end_headers();
  ClockApi::instance().initClocks();
  server.print(F("<html><body>Init triggerd</body></html>\n"));
  return true;
}

// POST /wifi
boolean wifi_post(HttpServer &server)
{
  ApiResponse response;
  auto doc = getJsonBody(server, response);
  // TODO Check if ssid and password is present
  Settings::WiFiSettings config = {doc["ssid"], doc["password"]};
  Settings::set_wifi_config(config);

  finishRequest(server, response);
  return true;
};

// POST /clock/X/Y
boolean clock_post(HttpServer &server)
{
  // TODO Path parsen und gucken welche Clock aktualisiert werden soll
  String path = String(server.get_path());

  String x_path = path.substring(7, 8);
  String y_path = path.substring(9, 10);
  uint8_t x = x_path.toInt();
  uint8_t y = y_path.toInt();

  Serial.println("POST '" + String(path) + "'clock x=" + String(x) + " ('" + x_path + "')" + " y=" + String(y) + " ('" + y_path + "')");
  ApiResponse response;
  ClockApi::instance().updateClock(response, x, y, getJsonBody(server, response));

  finishRequest(server, response);
  return true;
};

// GET /currenttime
boolean currenttime_get(HttpServer &server)
{
  ApiResponse response;
  ClockApi::instance().showCurrentTime(response);

  finishRequest(server, response);
  return true;
};

// GET /clock/X/Y
boolean clock_get(HttpServer &server)
{
  // TODO Path parsen und gucken welche Clock aktualisiert werden soll
  String path = String(server.get_path());

  uint8_t x = path.substring(7, 7).toInt();
  uint8_t y = path.substring(9, 9).toInt();

  Clock c = ClockApi::instance().datamanager().aiming.getClock(x, y);

  server.send_error_code(200);
  server.send_content_type("application/json");
  server.end_headers();
  serializeJson(c.asJson(), server);
  return true;
};

// GET /aiming/all
boolean aiming_all_get(HttpServer &server)
{
  server.send_error_code(200);
  server.send_content_type("application/json");
  server.end_headers();
  serializeJson(ClockApi::instance().datamanager().aiming.asJson(), server);
  return true;
};

// GET /planned/all
boolean planned_all_get(HttpServer &server)
{
  server.send_error_code(200);
  server.send_content_type("application/json");
  server.end_headers();
  serializeJson(ClockApi::instance().datamanager().planned.asJson(), server);
  return true;
};

// GET /current/all
boolean current_all_get(HttpServer &server)
{
  server.send_error_code(200);
  server.send_content_type("application/json");
  server.end_headers();
  DynamicJsonDocument doc(1024);
  doc.set(ClockApi::instance().datamanager().current.asJson());
  serializeJson(doc, server);
  return true;
};

HttpServer::PathHandler handlers[] = {
    {"/", HttpServer::GET, &index},
    {"/wifi", HttpServer::POST, &wifi_post},
    {"/init", HttpServer::GET, &init_get},
    {"/currenttime", HttpServer::GET, &currenttime_get},
    {"/planned",
     HttpServer::GET, &planned_all_get},
    {"/aiming",
     HttpServer::GET, &aiming_all_get},
    {"/current",
     HttpServer::GET, &current_all_get},
    {"/clock"
     "*",
     HttpServer::POST, &clock_post},
    {"/clock"
     "*",
     HttpServer::GET, &clock_get},
    {NULL}}; // Terminate Array with NULL!

} // namespace RequestHandler

#endif