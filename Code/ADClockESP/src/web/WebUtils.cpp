#include "WebUtils.h"

void WebUtils::finishRequest(HttpServer &server, ApiResponse &response)
{
  Serial.println("Finished Http-Request with " + String(response.getHttpCode()));
  server.send_error_code(response.getHttpCode());
  server.send_content_type("application/json");
  server.end_headers();
  serializeJson(response.getJson(), server);
  response.getJson().clear(); // free memory
}

JsonDocument &WebUtils::getJsonBody(HttpServer &server, ApiResponse &response)
{
  String body = server.get_client().readString();

  if (body.length() == 0)
  {
    response.error("No body was sent.");
  }
  else if (body.length() >= 32000)
  {
    response.error("The body was long. Limit is 32000 bytes.");
  }

  char bodyArray[body.length() + 1];
  body.toCharArray(bodyArray, body.length() + 1);

  // 234 = JSON_OBJECT_SIZE(2) + 2*JSON_OBJECT_SIZE(4) + 110;
  static StaticJsonDocument<258> doc;
  auto error = deserializeJson(doc, body);
  if (error)
  {
    response.error(String(error.c_str()));
    Serial.print(F("deserializeJson() failed with code "));
    Serial.println(error.c_str());
    return doc;
  }

  return doc;
}

void WebUtils::send_file(HttpServer &server, String filename)
{
  if (!SPIFFS.exists(filename))
  {
    server.send_error_code(200);
    server.send_content_type("text/html");
    server.end_headers();
    server.print("<html><body>Request resource not found.</body></html>");
    return;
  }

  server.send_error_code(200);
  server.send_content_type("text/html");
  server.end_headers();
  auto file = SPIFFS.open(filename);
  server.print(file.readString());
  file.close();
}