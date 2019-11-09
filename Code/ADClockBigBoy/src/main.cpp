#include "mbed.h"
#include "Manager.h"
#include "Debug.h"
#include "fastio/FastIO.h"

bool running;

// #ifdef DEBUG
Serial Debug::serial(SERIAL_TX, SERIAL_RX, 115200);
// #endif

Timer btnTimer;

FastOut<LED1> led_moving;
FastOut<LED2> led_buttonPressed;
FastOut<LED3> led_sendPlan;
FastIn<BUTTON1> user_button;

// Kommunikation API
BitBuffer apiBin;
ApiReceiver in(apiBin);
BitBuffer apiBout;
ApiSender out(apiBout);
ApiCommunication apicom(out, in, apiBout, apiBin);

// Daten Uhren
ClockWall aiming;
ClockWall planned;
ClockPositions current;

// Kommunikation Uhren
BitBuffer bufferout;
DataSender clockout(bufferout);
ClockCommunication clockcom(bufferout, clockout);

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
    // clockcom.performSpeedtest();
    int start = 0;
    int end = 0;
    FastIn<D15> pout;
    start = btnTimer.read_us();
    for (int i = 0; i < 1000; i++)
    {
        pout.read();
    }
    end = btnTimer.read_us();

    Debug::serial.printf("Main >> 1000 Pin on off took %d µs\n", (int)(end - start));

#endif

    // Initalisierung
    manager.init();
    manager.preventSendingPlan();

    // Wieso reicht das nicht? Plan wird nicht gesendet
    planned.setMutiplePositions(0, 0, WALL_SIZE_X - 1, WALL_SIZE_Y - 1, 180, 90);

    led_moving = 1;
    wait_ms(200);
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

    led_sendPlan = clockout.sending();

    if (user_button.read())
    {
        auto press_start = btnTimer.read_ms();
        while (user_button.read())
        {
            led_buttonPressed = btnTimer.read_ms() - press_start < 1000;
        }

        if (btnTimer.read_ms() - press_start < 1000)
        {
            auto cclock = planned.getClock(0, 5);
            auto hourDeg = cclock.hour.getPosition() % 360 + 10;
            auto minuteDeg = cclock.minute.getPosition() % 360 + 20;
            planned.setAnimationStart(0, 0);
            // planned.setMutiplePositions(0, 0, WALL_SIZE_X - 1, WALL_SIZE_Y - 1, hourDeg, minuteDeg);
            planned.setHourPosition(0, 5, hourDeg);
            planned.setMinutePosition(0, 5, minuteDeg);
            planned.setHourPosition(0, 4, hourDeg + 30);
            planned.setMinutePosition(0, 4, minuteDeg + 35);
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
            uint32_t startSending = us_ticker_read();
            led_sendPlan = 1;
            manager.executePlan();
            led_sendPlan = 0;
            uint32_t endSending = us_ticker_read();
            Debug::serial.printf("Sending done in %u µs\n", (int)(endSending - startSending));

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
