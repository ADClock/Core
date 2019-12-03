#include "DataManager.h"

void DataManager::init()
{
  while (!this->hasIncommingData())
  {
    delay(100);
    Serial.println("Waiting for inital data...");
  }

  this->read_data(); // reads first data

  // TODO Plan auf Aiming setzen
}

bool DataManager::hasIncommingData()
{
  this->receiver.tick();
  return this->receiver.recieving();
  // TODO Prüfung ob failed ?
}
