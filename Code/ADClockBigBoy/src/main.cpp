#include "mbed.h"
#include "Manager.h"
#include "Debug.h"
#include "fastio/FastIO.h"

bool running;

// #ifdef DEBUG
Serial Debug::serial(SERIAL_TX, SERIAL_RX, 9600);
// #endif

Timer btnTimer;

FastOut<LED1> led_moving;
FastOut<LED2> led_buttonPressed;
FastOut<LED3> led_sendPlan;
FastIn<BUTTON1> user_button;

// Kommunikation API
OutputStream out;
ApiCommunication apicom(out);

// Daten Uhren
ClockWall aiming;
ClockWall planned;
ClockPositions current;

// Kommunikation Uhren
ClockOutputStream clockout;
ClockCommunication clockcom(clockout);

// Manager
Manager manager(clockcom, apicom, current, aiming, planned);

size_t currentTestImage = 0;

void setup()
{
    // Anzeigen, dass nun initalisiert wird
    led_sendPlan = 1;
    led_buttonPressed = 1;
    wait_ms(1000);
    btnTimer.start();

#ifdef DEBUG
    // Testing the Speed
    //clockcom.performSpeedtest();
    int start = 0;
    int end = 0;
    DigitalOut pout(D8);
    start = btnTimer.read_us();
    for (int i = 0; i < 100000; i++)
    {
        pout = 0;
        pout = 1;
    }
    end = btnTimer.read_us();

    Debug::printf("Main >> Speedtest finished in %d µs\n", (int)(end - start));

#endif

    // Initalisierung
    manager.init();
    manager.preventSendingPlan();

    // Wieso reicht das nicht? Plan wird nicht gesendet
    planned.setMutiplePositions(0, 0, WALL_SIZE_X - 1, WALL_SIZE_Y - 1, 180, 90);

    led_moving = 1;
    wait_ms(50);
    led_moving = 0;
    led_buttonPressed = 0;
    led_sendPlan = 0;
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
#ifdef DEBUG
            Debug::println("Button pressed > short: testimage");
#endif
        }
        else
        {
            manager.init();
            manager.preventSendingPlan();
#ifdef DEBUG
            Debug::println("Button pressed > long: init");
#endif
        }

        led_buttonPressed = 0;
    }

    if (manager.hasPendingPlan())
    {
        if (!manager.hasPendingMoves())
        {
            long startSending = us_ticker_read();
            led_sendPlan = 1;
            manager.executePlan();
            led_sendPlan = 0;
            long endSending = us_ticker_read();
            Debug::serial.printf("Sending done in %d µs\n", (int)(endSending - startSending));

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
#ifdef DEBUG
    Debug::println("Setup complete");
#endif
    running = true; // Soll hässliches Verhalten beim Reset lösen.
    while (running)
    {
        loop();
        // Debug::println("looping done");
    }
#ifdef DEBUG
    Debug::println("Loop done.. Ending ?!");
#endif
}
