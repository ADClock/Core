
#ifndef _APICOMM_H_
#define _APICOMM_H_
#include "mbed.h"
#include "ApiSender.h"
#include "ApiReceiver.h"
#include "../../MbedJSONValue.h"

class ApiCommunication
{
public:
  ApiCommunication(ApiSender &out, ApiReceiver &in, BitBuffer &bufferout, BitBuffer &bufferin);

  void tick(); // Check for Data, Handle Requests

  void handle_requests();

private:
  bool respond(const MbedJSONValue &json);

  BitBuffer &bufferout;
  ApiSender &out;

  BitBuffer &bufferin;
  ApiReceiver &in;

  long lastSend;
};
#endif
