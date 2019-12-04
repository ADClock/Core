#ifndef _API_RESPONSE_H_
#define _API_RESPONSE_H_

#include "../parser/JSONValue.h"
#include "Arduino.h"

// Alle API Aktionen müssen irgendwie gescheit beantwortet werden.
// Am besten so, dass der User auch weiß, was schief gelaufen ist.

class ApiResponse
{
public:
  ApiResponse()
  {
    json = JSONValue();
    counter = 0;
    maxValence = 0;
  }

  void inform(const char *message)
  {
    addMessage(message, 1);
  }

  void warn(const char *message)
  {
    addMessage(message, 15);
  }

  void error(const char *message)
  {
    addMessage(message, 30);
  }

  size_t getHttpCode()
  {
    if (maxValence >= 30)
    {
      return 400; // Wertigkeit 30 -> Fehler in Anfrage
    }

    return 200; // Alles gut gegangen
  }

  JSONValue &getJson()
  {
    json["valence"] = maxValence;
    return json;
  }

private:
  void addMessage(const char *message, uint8_t valence)
  {
    Serial.println(String(valence) + ": " + message);
    json["messages"][counter]["message"] = message;
    json["messages"][counter]["valence"] = valence;
    counter++;

    if (valence > maxValence)
    {
      maxValence = valence;
    }
  }

  JSONValue json;
  uint8_t maxValence;
  uint8_t counter;
};

#endif