#include "mbed.h"
#include "ClockMatrix.h"
#include "Debug.h"

bool running;

Serial Debug::serial(SERIAL_TX, SERIAL_RX, 9600);

DigitalOut led_moving(LED1);
DigitalOut led_buttonPressed(LED2);
DigitalOut led_startMoving(LED3);
DigitalIn user_button(BUTTON1);

OutputStream out;
DataSender sender(out);
ClockMatrix matrix(sender);

size_t currentTestImage = 0;

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

void setTestImage()
{
    switch (currentTestImage)
    {
    case 0:
        matrix.initMatrix();
        matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 45, 45);
        break;
    case 1:
        matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 90, 135);
        matrix.setMinuteSpeed(100);
        break;

    case 2:
        matrix.setMinuteSpeed(0);
        matrix.setMinuteRotation(false);
        matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 135, 90);
        break;
    case 3:
        matrix.setHourRotation(false);
        matrix.setMinuteRotation(true);
        matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 90, 135);
        break;
    case 4:
        matrix.setHourRotation(true);
        matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 180, 180);
        matrix.setAnimationStart(90, 90);
        break;
    case 5:
        matrix.setNextPositionFor(0, 0, CLOCKS_X - 1, CLOCKS_Y - 1, 270, 90);
        break;
    default:
        break;
    }

    currentTestImage++;
    if (currentTestImage > 5)
        currentTestImage = 0;
}

void loop()
{

    // Status LEDs aktualisieren
    led_moving = matrix.isMoving();

    bool tryMove = user_button.read();
    led_buttonPressed = tryMove;

    // Test Image setzen, wenn Knopf gedrückt wurde
    if (tryMove && !matrix.isMoving() && !matrix.hasPendingMove())
    {
        // Setting Test Image
        setTestImage();
        Debug::println("Testimage set");
    }

    // Uhren bewegen
    if (matrix.hasPendingMove())
    {
        if (!matrix.isMoving())
        {
            // long startSending = us_ticker_read();
            led_startMoving = 1;
            matrix.move();
            Debug::println("Moved");
            led_startMoving = 0;
            // long endSending = us_ticker_read();
            // Debug::serial.printf("Sending done in %d µs\n", (endSending - startSending));

            /* auto v = matrix.asJson();
            Debug::println("Json Objekt erstellt.");

            string str = v.serialize();
            Debug::println("Json serializiert.");

            for (size_t index = 0; index < str.length(); index++)
            {
                Debug::serial.printf("%c", str[index]);
            }*/
        }
    }
}

int main()
{
    setup();
    Debug::println("Setup complete");
    running = true; // Soll hässliches Verhalten beim Reset lösen.
    while (running)
    {
        loop();
        // Debug::println("looping done");
    }
    Debug::println("Loop done.. Ending ?!");
}
