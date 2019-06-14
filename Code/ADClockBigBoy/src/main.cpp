#include "mbed.h"
#include "ClockMatrix.h"
#include "Debug.h"

DigitalOut myled(LED1);

OutputStream out;
DataSender sender(out);
ClockMatrix matrix(sender);

void setup()
{
    for (size_t y = 0; y < CLOCKS_Y; y++)
    {
        for (size_t x = 0; x < CLOCKS_X; x++)
        {
            Debug::print(" " + matrix.getClockPosition(x, y));
        }
        Debug::println("");
    }

    // Initalisierung
    matrix.initMatrix();
}

void loop()
{
    if (!matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 180, 90))
        Debug::println("Fehler beim Setzen aller Positionen.");

    // if (matrix.hasPendingMove())
    // {
    if (!matrix.isMoving())
    {
        // long startSending = micros();

        matrix.move();

        // long endSending = micros();
        // serial.printf("Sending done in %s µs\n", (endSending - startSending));
    }
    // }
}

int main()
{
    setup();
    Debug::println("Setup complete");
    while (1)
    {
        loop();
        Debug::println("looping done");
    }
    Debug::println("Loop done.. Ending ?!");
}
