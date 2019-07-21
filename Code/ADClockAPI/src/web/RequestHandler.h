#ifndef _REQUESTHANDLER_H_
#define _REQUESTHANDLER_H_
#include "Arduino.h"
#include "HttpServer.h"

namespace RequestHandler
{
boolean index(HttpServer &server)
{
  server.send_error_code(200);
  server.send_content_type("text/html");
  server.end_headers();
  server.print(F("<html><body>Chris is doof :)</body></html>\n"));
  return true;
}

// POST clock/X/Y
boolean clock_post(HttpServer &server)
{
  // TODO Path parsen und gucken welche Clock aktualisiert werden soll

  server.send_error_code(200);
  server.send_content_type("text/html");
  server.end_headers();
  server.print(("<html><body>Chris is doof :) " + String(server.get_body()) + "</body></html>\n"));
  return true;
};

HttpServer::PathHandler handlers[] = {
    {"/", HttpServer::GET, &index},
    {"/clock"
     "*",
     HttpServer::POST, &clock_post},
    {NULL}}; // Terminate Array with NULL!
} // namespace RequestHandler

#endif