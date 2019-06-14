#include "mbed.h"
#include "ClockMatrix.h"
#include "Debug.h"

Serial Debug::serial(SERIAL_TX, SERIAL_RX, 9600);

DigitalOut led_moving(LED1);
DigitalOut led_buttonPressed(LED2);
DigitalOut led_startMoving(LED3);
DigitalIn user_button(BUTTON1);

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

    // Status LEDs aktualisieren
    led_moving = matrix.isMoving();

    bool tryMove = user_button.read();

    led_buttonPressed = tryMove;

    if (!matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 180, 180))
        Debug::println("Fehler beim Setzen aller Positionen. ");

    if (matrix.hasPendingMove() && tryMove)
    {
        if (!matrix.isMoving())
        {
            long startSending = us_ticker_read();
            led_startMoving = 1;
            matrix.move();
            led_startMoving = 0;
            long endSending = us_ticker_read();
            Debug::serial.printf("Sending done in %d µs\n", (endSending - startSending));

            // TODO Später rausnehmen
            matrix.initMatrix();
        }
    }
}

int main()
{
    setup();
    Debug::println("Setup complete");
    while (1)
    {
        loop();
        // Debug::println("looping done");
    }
    Debug::println("Loop done.. Ending ?!");
}
