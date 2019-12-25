// #include "ADClock.h"

// Manager &ADClock::manager()
// {
//   return _manager;
// }

// AnimationManager &ADClock::animations()
// {
//   return _animations;
// }
// NTPTime &ADClock::time()
// {
//   return _time;
// }

#ifdef DEBUG

void test_json()
{
  // Test Json
  StaticJsonDocument<JSON_OBJECT_SIZE(10)> doc;
  StaticJsonDocument<JSON_OBJECT_SIZE(2)> obj;
  obj["abc"] = "foo";
  obj["def"] = 42;
  doc["ohh"] = obj;
  doc["ooo"] = true;
  serializeJson(doc, Serial);
}

void test_communication_speed()
{
  long start = 0;
  long end = 0;

  Serial.println("Toggling");
  start = micros();

  for (size_t i = 0; i < 10000; i++)
  {
    digitalWrite(18, 1);
    digitalWrite(18, 0);
  }
  end = micros();
  Serial.println("10000 Pin-Toggles took " + String(end - start) + " µs");

  double total_bits = 0;
  double total_time = 0;
  Serial.println("Speedtesting...");
  for (size_t i = 0; i < 10; i++)
  {
    // Preparing
    clockcom.cleanup_communication();
    clockcom.sendCommand(0x03); // 8 bit command
    // Maximum: 1000 sonst Buffer größer stellen
    for (size_t i = 0; i < 576; i++)
    {
      clockcom.sendByte(0x42);
    }
    total_bits += 8 + 576 * 8;
    start = micros();
    if (!clockcom.transmit())
    {
      Serial.println("Übertragung fehlgeschlagen. Testergebnis ungültig!");
    }
    end = micros();
    long current_time = end - start;
    total_time += current_time;
    Serial.println("Transmission " + String(i) + " took " + String(current_time) + " µs");
  }

  Serial.println("Speedtest finished\nTransmitted " + String(total_bits) + " bits in " + String(total_time) + " µs\nSpeed: " + String(total_bits / (total_time / 1000. / 1000.)) + " bits/second");
}

#endif