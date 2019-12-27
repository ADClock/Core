#ifndef _REST_ANIMATION_H_
#define _REST_ANIMATION_H_
#include "WebUtils.h"
#include "AnimationManager.h"

extern AnimationManager _animations;
namespace RESTAnimation
{

boolean run_get();
boolean add_post();

boolean pause_animation();
boolean play_animation();

Step parse_step(ApiResponse &response, JsonDocument &json);

}; // namespace RESTAnimation

#endif