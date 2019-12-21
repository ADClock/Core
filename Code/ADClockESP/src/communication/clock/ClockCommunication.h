
#ifndef _CLOCKCOM_H_
#define _CLOCKCOM_H_
#include "Arduino.h"
#include "../../Config.h"
#include "../BitBuffer.h"
#include "DataSender.h"
#include "../../data/ClockWall.h"

class ClockCommunication
{
public:
  ClockCommunication(BitBuffer &buffer, DataSender &sender);

  // Delayed um entsprechende ms und prüft ob Daten anliegen
  void sendInitCommand();

  void sendPlan(ClockWall &plan);

  // sends hole data. returns result
  bool transmit();

  // checks if currently transmitting
  bool is_transmitting();

  // reads the result from last transmission
  bool was_successful();

  // Garantiert, dass Übertragungskanal frei ist
  void cleanup_communication();

  void sendCommand(uint8_t command);

  void sendData(uint8_t *data, size_t length);
  void sendByte(uint8_t byte);

private:
  size_t getClockX(size_t &position);
  size_t getClockY(size_t &position);

  BitBuffer &buffer;
  DataSender &sender;
};
#endif
