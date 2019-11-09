#include "ApiCommunication.h"

ApiCommunication::ApiCommunication(ApiSender &out, ApiReceiver &in, BitBuffer &bufferout, BitBuffer &bufferin) : out(out), in(in), bufferout(bufferout), bufferin(bufferin)
{
}

void ApiCommunication::tick()
{
  this->in.tick();
  while (this->in.recieving())
  {
    this->in.tick();
    if (this->in.failed())
    {
      this->in.reset();
      break;
    }
  }

  handle_requests();

  this->out.tick();
  while (this->out.sending())
  {
    this->out.tick();
    if (this->out.failed())
    {
      this->out.reset();
      break;
    }
  }
}

void ApiCommunication::handle_requests()
{
  if (this->bufferin.size() < 8)
  {
    return; // Kein Vollständiger Command übermittelt
  }

  // TODO Richtige API Requests einbauen
  bufferout.enqueue(this->bufferin.size() == 8);
  this->bufferin.clear();
}