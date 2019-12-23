#include "RESTClock.h"

boolean RESTClock::clock_post(HttpServer &server)
{
  ApiResponse response;
  auto clock = parse_path(server, response);
  auto &json = WebUtils::getJsonBody(server, response);

  if (response.is_okay())
    ClockApi::instance().updateClock(response, clock.x, clock.y, json);

  WebUtils::finishRequest(server, response);
  return true;
};

SelectedClock RESTClock::parse_path(HttpServer &server, ApiResponse &response)
{
  // TODO Im Fehlerfall APIResponse füllen
  String path = String(server.get_path());

  String x_path = path.substring(7, 8);
  String y_path = path.substring(9, 10);
  uint8_t x = x_path.toInt();
  uint8_t y = y_path.toInt();
  return {x, y};
}