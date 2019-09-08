#ifndef _CLOCK_API_H_
#define _CLOCK_API_H_

#include "../communication/InputStream.h"
#include "../communication/OutputStream.h"
#include "../data/ClockMatrix.h"
#include "ApiResponse.h"

#define MINUTE_HANDLE 1
#define HOUR_HANDLE 2

class ClockApi
{
public:
  static ClockApi &instance()
  {
    static ClockApi INSTANCE;
    return INSTANCE;
  }

  // Updates a hole Clock
  void updateClock(ApiResponse &response, uint8_t x, uint8_t y, JSONValue &value);

  // Updates a single Handle
  void updateHand(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, JSONValue &value);

  // Was kann alles beim Handle aktualisiert werden?
  void updateHandPosition(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, size_t position);

  void updateHandRotation(ApiResponse &response, uint8_t x, uint8_t y, uint8_t hand, bool rotation);

  ClockMatrix data;

private:
  ClockApi(){};

  // Validierungsroutinen
  bool isValidPosition(size_t position);

  // Keine Ahnung ob das hier soweit sinnvoll ist.
  InputStream in;
  OutputStream out;
};

#endif