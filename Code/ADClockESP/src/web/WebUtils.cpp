#include "WebUtils.h"
#include "WebServer.h"

void WebUtils::finishRequest(ApiResponse &response)
{
  Serial.println("Finished Http-Request with " + String(response.getHttpCode()));
  String s;
  serializeJson(response.getJson(), s);
  response.getJson().clear(); // free memory
  server.send(response.getHttpCode(), "application/json", s);
}

JsonDocument &WebUtils::getJsonBody(ApiResponse &response)
{
  String body = server.arg("plain");
  Serial.print("The Body: ");
  Serial.println(body);

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

void WebUtils::send_file(String filename)
{
  if (!SPIFFS.exists(filename))
  {
    server.send(404, "text/plain", "Requested resource not found!");
    return;
  }

  auto file = SPIFFS.open(filename);
  server.streamFile(file, get_content_type(filename));
  file.close();
}

String WebUtils::get_content_type(String &filename)
{
  if (filename.endsWith(".html"))
    return "text/html";
  else if (filename.endsWith(".css"))
    return "text/css";
  else if (filename.endsWith(".js"))
    return "application/javascript";
  else if (filename.endsWith(".ico"))
    return "image/x-icon";
  return "text/plain";
}