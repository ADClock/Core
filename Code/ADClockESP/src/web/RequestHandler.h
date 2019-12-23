#ifndef _REQUESTHANDLER_H_
#define _REQUESTHANDLER_H_
#include "Arduino.h"
#include "Settings.h"
#include "HttpServer.h"
#include "../api/ClockApi.h"
#include "../api/ApiResponse.h"
#include "WebUtils.h"
#include "RESTClock.h"

namespace RequestHandler
{

boolean index(HttpServer &server)
{
  WebUtils::send_file(server, F("/index.html"));
  return true;
}

boolean send_file(HttpServer &server)
{
  WebUtils::send_file(server, server.get_path());
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
  auto doc = WebUtils::getJsonBody(server, response);
  // TODO Check if ssid and password is present
  Settings::WiFiSettings config = {doc["ssid"], doc["password"]};
  Settings::set_wifi_config(config);
  response.inform("Wifi config set");

  WebUtils::finishRequest(server, response);
  return true;
};

// DELETE /wifi
boolean wifi_delete(HttpServer &server)
{
  ApiResponse response;
  Settings::delete_wifi_config();
  response.inform("Wifi config deleted");
  WebUtils::finishRequest(server, response);
  return true;
};

// GET /currenttime
boolean currenttime_get(HttpServer &server)
{
  ApiResponse response;
  ClockApi::instance().showCurrentTime(response);

  WebUtils::finishRequest(server, response);
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
    {"/wifi", HttpServer::DELETE, &wifi_delete},
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
     HttpServer::POST, &RESTClock::clock_post},
    {"/clock"
     "*",
     HttpServer::GET, &clock_get},
    {"/*", HttpServer::GET, &send_file},
    {NULL}}; // Terminate Array with NULL!

}; // namespace RequestHandler

#endif