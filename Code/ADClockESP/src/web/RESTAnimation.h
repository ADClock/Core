#ifndef _REST_ANIMATION_H_
#define _REST_ANIMATION_H_
#include "WebUtils.h"
#include "AnimationManager.h"

extern AnimationManager _animations;
namespace RESTAnimation
{

boolean run_get(HttpServer &server);
boolean add_post(HttpServer &server);

boolean pause_animation(HttpServer &server);
boolean play_animation(HttpServer &server);

Step parse_step(ApiResponse &response, JsonDocument &json);

}; // namespace RESTAnimation

#endif