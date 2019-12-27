#include "RESTClock.h"
#include "WebServer.h"

extern WebServer server;

boolean RESTClock::clock_post()
{
  ApiResponse response;
  auto clock = parse_path(response);
  auto &json = WebUtils::getJsonBody(response);

  if (response.is_okay())
    ClockApi::instance().updateClock(response, clock.x, clock.y, json);

  WebUtils::finishRequest(response);
  return true;
};

SelectedClock RESTClock::parse_path(ApiResponse &response)
{

  if (!server.hasArg("x"))
  {
    response.error("Please add argument x.");
    return {0, 0};
  }

  if (!server.hasArg("y"))
  {
    response.error("Please add argument y.");
    return {0, 0};
  }

  String x_arg = server.arg("x");
  String y_arg = server.arg("y");
  // TODO Nur validieren wenn gültig
  uint8_t x = x_arg.toInt();
  uint8_t y = y_arg.toInt();
  return {x, y};
}