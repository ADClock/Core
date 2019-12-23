#ifndef _REST_CLOCK_H_
#define _REST_CLOCK_H_
#include "WebUtils.h"
#include "api/ClockApi.h"

struct SelectedClock
{
  size_t x;
  size_t y;
};

namespace RESTClock
{

boolean clock_post(HttpServer &server);
SelectedClock parse_path(HttpServer &server, ApiResponse &response);

}; // namespace RESTClock

#endif