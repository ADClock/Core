#include "DataSender.h"

DataSender::DataSender(OutputStream &out) : out(out)
{
  this->lastSend = millis() + 2000; // Wir verzögern das erste Senden um 2 Sekunden damit sich die Motoren initalisieren können.
}

bool DataSender::sendInitCommand()
{
  return sendCommand(COMMAND_INIT);
}

bool DataSender::sendImage(byte arr[], size_t length)
{
  if (!sendCommand(COMMAND_IMAGE))
    return false;
  // Image verschicken
  return out.sendDataArray(arr, length);
}

bool DataSender::sendCommand(byte command)
{
  // Delay zwischen einzelnen Commands einhalten
  while (millis() < this->lastSend + DELAY_BETWEEN_COMMANDS)
  {
    delay(1);
  }
  this->lastSend = millis();

  if (!out.sendData(command))
    return false;
  return true;
}