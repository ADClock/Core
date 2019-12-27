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
boolean clock_post();
SelectedClock parse_path(ApiResponse &response);

}; // namespace RESTClock

#endif