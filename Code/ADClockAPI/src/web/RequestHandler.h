#ifndef _REQUESTHANDLER_H_
#define _REQUESTHANDLER_H_
#include "Arduino.h"
#include "HttpServer.h"
#include "../api/ClockApi.h"
#include "../api/ApiResponse.h"
#include "../data/ClockMatrix.h"

namespace RequestHandler
{
void finishRequest(HttpServer &server, ApiResponse &response)
{
  Serial.println("Finished Http-Request with " + String(response.getHttpCode()));
  server.send_error_code(response.getHttpCode());
  server.send_content_type("application/json");
  server.end_headers();
  server.print(response.getJson().serialize().c_str());
  Serial.println(response.getJson().serialize().c_str());
}

JSONValue &getJsonBody(HttpServer &server, ApiResponse &response)
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

  static JSONValue jsonValue;
  auto errors = parse(jsonValue, bodyArray);
  if (errors.length() > 0)
  {
    response.error(errors.c_str());
    Serial.println("JSON-Error: " + String(errors.c_str()));
  }

  return jsonValue;
}

boolean index(HttpServer &server)
{
  server.send_error_code(200);
  server.send_content_type("text/html");
  server.end_headers();
  server.print(F("<html><body>Chris is doof :)</body></html>\n"));
  return true;
}

// POST /clock/X/Y
boolean clock_post(HttpServer &server)
{
  // TODO Path parsen und gucken welche Clock aktualisiert werden soll
  String path = String(server.get_path());

  uint8_t x = path.substring(7, 7).toInt();
  uint8_t y = path.substring(9, 9).toInt();

  ApiResponse response;
  ClockApi::instance().updateClock(response, x, y, getJsonBody(server, response));

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

  Clock c = ClockApi::instance().data.getClock(x, y);

  server.send_error_code(200);
  server.send_content_type("application/json");
  server.end_headers();
  server.print(c.asJson().serialize().c_str());
  return true;
};

HttpServer::PathHandler handlers[] = {
    {"/", HttpServer::GET, &index},
    {"/clock"
     "*",
     HttpServer::POST, &clock_post},
    {"/clock"
     "*",
     HttpServer::GET, &clock_get},
    {NULL}}; // Terminate Array with NULL!

} // namespace RequestHandler

#endif