#include "RESTAnimation.h"
#include "WebServer.h"

extern WebServer server;

boolean RESTAnimation::run_get()
{
  ApiResponse response;
  String name = server.arg("name"); // TODO Validieren ob vorhaden

  if (response.is_okay())
  {
    _animations.load(name);
    _animations.play_animation();
  }

  WebUtils::finishRequest(response);
  return true;
};

boolean RESTAnimation::add_post()
{
  ApiResponse response;
  String name = server.arg("name"); // TODO Validieren ob vorhaden
  auto &json = WebUtils::getJsonBody(response);
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

  WebUtils::finishRequest(response);
  return true;
};

boolean RESTAnimation::save_current_get()
{
  ApiResponse response;
  String name = server.arg("name");

  if (response.is_okay())
  {
    auto result = _animations.save_wall(name, _manager.planned);
    if (result)
    {
      response.inform("Die Wand wurde erfolgreich gespeichert.");
    }
    else
    {
      response.error("Die Wand wurde nicht gespeichert.");
    }
  }

  WebUtils::finishRequest(response);
  return true;
};

boolean RESTAnimation::pause_animation()
{
  ApiResponse response;

  _animations.pause_animation();
  response.inform("Animation paused.");

  WebUtils::finishRequest(response);
  return true;
}

boolean RESTAnimation::play_animation()
{
  ApiResponse response;

  _animations.play_animation();
  response.inform("Animation continued.");

  WebUtils::finishRequest(response);
  return true;
}

Step RESTAnimation::parse_step(ApiResponse &response, JsonDocument &json)
{
  // TODO Validierung
  return {static_cast<StepType>(json["type"].as<int>()), json["value"]};
}