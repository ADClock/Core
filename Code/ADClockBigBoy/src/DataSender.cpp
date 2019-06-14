#include "DataSender.h"

// Timer timer;
DataSender::DataSender(OutputStream &out) : out(out)
{
  // timer.start();
  // this->lastSend = timer.read_ms() + 2000; // Wir verzögern das erste Senden um 2 Sekunden damit sich die Motoren initalisieren können.
}

bool DataSender::sendInitCommand()
{
  return sendCommand(COMMAND_INIT);
}

bool DataSender::sendImage(const uint8_t arr[], const size_t &length)
{
  if (!sendCommand(COMMAND_IMAGE))
    return false;
  // Image verschicken
  return out.sendDataArray(arr, length);
}

bool DataSender::sendCommand(const uint8_t &command)
{
  // Delay zwischen einzelnen Commands einhalten
  while (/*timer.read_ms() < this->lastSend + DELAY_BETWEEN_COMMANDS*/ false)
  {
    wait_ms(1);
  }
  // this->lastSend = timer.read_ms();

  if (!out.sendData(command))
    return false;
  return true;
}