#ifndef _WEB_UTILS_H_
#define _WEB_UTILS_H_
#include "api/ApiResponse.h"
#include "HttpServer.h"
#include "SPIFFS.h"

namespace WebUtils
{
void finishRequest(HttpServer &server, ApiResponse &response);

JsonDocument &getJsonBody(HttpServer &server, ApiResponse &response);

void send_file(HttpServer &server, String filename);

}; // namespace WebUtils

#endif