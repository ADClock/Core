#include "RESTAnimation.h"

boolean RESTAnimation::run_get(HttpServer &server)
{
  ApiResponse response;
  String name = String(server.get_path()).substring(15);

  if (response.is_okay())
  {
    _animations.load(name);
    _animations.play_animation();
  }

  WebUtils::finishRequest(server, response);
  return true;
};

boolean RESTAnimation::add_post(HttpServer &server)
{
  ApiResponse response;
  String name = String(server.get_path()).substring(15);
  auto &json = WebUtils::getJsonBody(server, response);
  auto step = parse_step(response, json);

  if (response.is_okay())
  {
    auto result = _animations.append_step(name, step);
    if (result)
    {
      response.inform("Der Schritt wurde erfolgreich hinzugefügt.");
    }
    else
    {
      response.error("Der Schritt konnte nicht hinzugefügt werden.");
    }
  }

  WebUtils::finishRequest(server, response);
  return true;
};

boolean RESTAnimation::pause_animation(HttpServer &server)
{
  ApiResponse response;

  _animations.pause_animation();
  response.inform("Animation paused.");

  WebUtils::finishRequest(server, response);
  return true;
}

boolean RESTAnimation::play_animation(HttpServer &server)
{
  ApiResponse response;

  _animations.play_animation();
  response.inform("Animation continued.");

  WebUtils::finishRequest(server, response);
  return true;
}

Step RESTAnimation::parse_step(ApiResponse &response, JsonDocument &json)
{
  // TODO Validierung
  return {static_cast<StepType>(json["type"].as<int>()), json["value"]};
}