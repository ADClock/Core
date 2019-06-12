#include <Arduino.h>
#include "ClockMatrix.h"

OutputStream out;
DataSender sender(out);
ClockMatrix matrix(sender);

void setup()
{
  Serial.begin(9600);
  Serial.println("The wirering:");
  for (size_t y = 0; y < CLOCKS_Y; y++)
  {
    for (size_t x = 0; x < CLOCKS_X; x++)
    {
      Serial.print(String(matrix.getClockPosition(x, y)) + " ");
    }
    Serial.println();
  }

  Serial.println("Calibrate everything...");
  matrix.initMatrix();

  Serial.println("Setup routine complete");
}

void loop()
{
  matrix.initMatrix();
  if (!matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 180, 90))
    Serial.println("Fehler beim Setzen aller Positionen.");

  // if (matrix.hasPendingMove())
  // {
  if (!matrix.isMoving())
  {
    long startSending = micros();

    matrix.move();

    long endSending = micros();
    Serial.println("Sending done in " + String(endSending - startSending) + " µs");
  }
  // }
}
