#ifndef _WEB_UTILS_H_
#define _WEB_UTILS_H_
#include "api/ApiResponse.h"
#include "WebServer.h"
#include "SPIFFS.h"

extern WebServer server;

namespace WebUtils
{
void finishRequest(ApiResponse &response);

JsonDocument &getJsonBody(ApiResponse &response);

void send_file(String filename = server.uri());

String get_content_type(String &filename);

}; // namespace WebUtils

#endif