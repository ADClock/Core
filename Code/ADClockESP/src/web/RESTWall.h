#ifndef _REST_WALL_H_
#define _REST_WALL_H_
#include "Arduino.h"
#include "WebUtils.h"
#include "../api/ClockApi.h"
namespace RESTWall
{

boolean init_get()
{
  ClockApi::instance().initClocks();
  server.send(200, "text/html", F("<html><body>Init triggerd</body></html>\n"));
  return true;
}

// GET /currenttime
boolean currenttime_get()
{
  ApiResponse response;
  ClockApi::instance().showCurrentTime(response);

  WebUtils::finishRequest(response);
  return true;
};
}; // namespace RESTWall

#endif