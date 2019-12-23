#ifndef _API_RESPONSE_H_
#define _API_RESPONSE_H_

#include "Arduino.h"
#include "ArduinoJson.h"

// Alle API Aktionen müssen irgendwie gescheit beantwortet werden.
// Am besten so, dass der User auch weiß, was schief gelaufen ist.

class ApiResponse
{
public:
  ApiResponse() : json(DynamicJsonDocument(1024))
  {
    maxValence = 0;
    json.createNestedArray("messages");
  }

  void inform(String message)
  {
    addMessage(message, 1);
  }

  void warn(String message)
  {
    addMessage(message, 15);
  }

  void error(String message)
  {
    addMessage(message, 30);
  }

  bool is_okay()
  {
    return maxValence < 30;
  }

  size_t getHttpCode()
  {
    if (!is_okay())
    {
      return 400; // Wertigkeit 30 -> Fehler in Anfrage
    }

    return 200; // Alles gut gegangen
  }

  JsonDocument &getJson()
  {
    json["valence"] = maxValence;
    return json;
  }

private:
  void addMessage(String &message, uint8_t valence)
  {
    /* Serial.println(String(valence) + ": " + message);
    JsonArray arr = json["messages"].as<JsonArray>();
    JsonObject obj = arr.createNestedObject();
    obj["message"] = String(message);
    obj["valence"] = valence;
*/
    if (valence > maxValence)
    {
      maxValence = valence;
    }
  }

  JsonDocument json;
  uint8_t maxValence;
};

#endif