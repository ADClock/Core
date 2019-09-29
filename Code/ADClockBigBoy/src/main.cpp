#include "mbed.h"
#include "Manager.h"
#include "Debug.h"

bool running;

Serial Debug::serial(SERIAL_TX, SERIAL_RX, 9600);

Timer btnTimer;

DigitalOut led_moving(LED1);
DigitalOut led_buttonPressed(LED2);
DigitalOut led_startMoving(LED3);
DigitalIn user_button(BUTTON1);

// Kommunikation Uhren
ClockOutputStream clockout;
ClockCommunication clockcom(clockout);

// Kommunikation API
OutputStream out;
ApiCommunication apicom(out);

// Daten Uhren
ClockPositions current;
ClockWall aiming;
ClockWall planned;

// Manager
Manager manager(clockcom, apicom, current, aiming, planned);

size_t currentTestImage = 0;

void setup()
{
    // Anzeigen, dass nun initalisiert wird
    led_startMoving = 1;
    led_buttonPressed = 1;
    wait_ms(1000);
    btnTimer.start();

    // Initalisierung
    manager.init();
    manager.preventSendingPlan();

    // Wieso reicht das nicht? Plan wird nicht gesendet
    planned.setMutiplePositions(0, 0, WALL_SIZE_X - 1, WALL_SIZE_Y - 1, 180, 90);

    led_moving = 1;
    wait_ms(50);
    led_moving = 0;
    led_buttonPressed = 0;
    led_startMoving = 0;
    wait_ms(25);
}

void loop()
{
    manager.try_step();

    // Status LEDs aktualisieren
    led_moving = manager.hasPendingMoves();

    if (user_button.read())
    {
        auto press_start = btnTimer.read_ms();
        while (user_button.read())
        {
            led_buttonPressed = btnTimer.read_ms() - press_start < 1000;
        }

        if (btnTimer.read_ms() - press_start < 1000)
        {
            auto cclock = planned.getClock(0, 0);
            auto hourDeg = cclock.hour.getPosition() % 360 + 10;
            auto minuteDeg = cclock.minute.getPosition() % 360 + 20;
            planned.setAnimationStart(0, 0);
            planned.setMutiplePositions(0, 0, WALL_SIZE_X - 1, WALL_SIZE_Y - 1, hourDeg, minuteDeg);
            manager.allowSendingPlan();
            Debug::println("Button pressed > short: testimage");
        }
        else
        {
            manager.init();
            manager.preventSendingPlan();
            Debug::println("Button pressed > long: init");
        }

        led_buttonPressed = 0;
    }

    if (manager.hasPendingPlan())
    {
        if (!manager.hasPendingMoves())
        {
            // long startSending = us_ticker_read();
            led_startMoving = 1;
            manager.executePlan();
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
