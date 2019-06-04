#include "MyDataCom.h"

MyDataCom::MyDataCom(const Motor &foo) : motor2(foo)
{

  // Lower Clock Input Pin
  digitalWrite(IN_CLOCK, LOW);
  digitalWrite(OUT_CLOCK, LOW);
  digitalWrite(IN_RESPONSE, LOW);

  // Input als Input definieren
  pinMode(IN_RESPONSE, OUTPUT);
  pinMode(IN_CLOCK, INPUT);
  pinMode(IN_DATA, INPUT);

  // Output als Output definieren
  pinMode(OUT_CLOCK, OUTPUT);
  pinMode(OUT_RESPONSE, INPUT);
  pinMode(OUT_DATA, OUTPUT);
}

void MyDataCom::checkForData()
{
  if (digitalRead(IN_CLOCK) == HIGH)
  {
    recieveData();
  }
  else
  {
    // Serial.println("No data incomming");
  }
}

void MyDataCom::sendData(uint8_t data)
{
  shiftOut(LSBFIRST, data);
  //Serial.println("Sending complete");
}

void MyDataCom::recieveData()
{
  int input = shiftIn(LSBFIRST);
  Serial.println("Wuppy Data da " + String(input));

  if (input == 0x01)
  {
    // Init
    //shiftOut(LSBFIRST, 0x01);
    motor2.start_calibraton();
  }
  else if (input == 0x02)
  {
    // Image lesen, erste Clock Stellung lesen und Rest weiterschicken.
    recieveClockImage();
    // shiftOut(LSBFIRST, 0x02);
    // shiftInShiftOut();
  }
}

void MyDataCom::recieveClockImage()
{
  uint8_t input[8];
  // 8 Byte lesen (4 je Motor)
  // -- Position  (16 Bit)
  // -- Delay     (8 Bit)
  // -- Speed     (7 Bit)
  // -- Direction (1 Bit)
  for (int i = 0; i < 8; i++)
  {
    input[i] = shiftIn(LSBFIRST);
  }

  // Motor bewegen
  uint16_t foo = *input;
  Serial.println("Clock image " + String(input[0]) + String(input[1]) + String(input[2]) + String(input[3]) + String(input[4]) + String(input[5]) + String(input[6]) + String(input[7]) +
                 " und target: " + String(foo));
  motor2.set_target_pos(foo);
}

void MyDataCom::shiftInShiftOut()
{
  // Solange an der Clock ein Signal anliegt..
  while (waitForSignalChange(IN_CLOCK, LOW))
  {
    sendBit(readBit());
  }
  Serial.println("No more data found");
}

// true -> Signal war nicht da
// false -> Signal ist da
bool MyDataCom::waitForSignalChange(uint8_t pin, int signal)
{
  // Signal steht richtig?
  if (digitalRead(pin) != signal)
  {
    Serial.println("Pin " + String(pin) + " war garnicht " + String(signal));
    // Ist oke, dann ist das Signal ja bereits auf dem gewünschten..
    return false;
  }

  int counter = 0;
  while (digitalRead(pin) == signal)
  {
    if (counter++ > 100000000)
      return true;
  }
  return false;
}

int MyDataCom::readBit()
{
  // Warten  bis Clock an ist.. (falls sie aus ist)
  if (digitalRead(IN_CLOCK) == LOW && waitForSignalChange(IN_CLOCK, LOW))
  {
    Serial.println("[shiftIn] No clock signal (no high)");
    return 0x00;
  }
  int bit = digitalRead(IN_DATA);

  // Mitteilen, dass lesen erfolgreich war
  digitalWrite(IN_RESPONSE, HIGH);
  // Warten das Clock ausgeht
  if (waitForSignalChange(IN_CLOCK, HIGH))
  {
    Serial.println("[shiftIn] No clock reset to low");
    return 0x00;
  }
  // Response wieder ausschalten
  digitalWrite(IN_RESPONSE, LOW);

  return bit;
}

uint8_t MyDataCom::shiftIn(uint8_t bitOrder)
{
  uint8_t value = 0;
  uint8_t i;

  for (i = 0; i < 8; ++i)
  {

    if (bitOrder == LSBFIRST)
      value |= readBit() << i;
    else
      value |= readBit() << (7 - i);
  }
  return value;
}

void MyDataCom::shiftOut(uint8_t bitOrder, uint8_t val)
{
  uint8_t i;

  for (i = 0; i < 8; i++)
  {
    if (bitOrder == LSBFIRST)
      sendBit(!!(val & (1 << i)));
    else
      sendBit(!!(val & (1 << (7 - i))));
  }
}

/**
 * Versendet einen Bit, setzt anschließend die Clock und wartet darauf, dass das Bit gelesen wurde.
 * */
void MyDataCom::sendBit(int bit)
{
  digitalWrite(OUT_DATA, bit);
  digitalWrite(OUT_CLOCK, HIGH);
  if (waitForSignalChange(OUT_RESPONSE, LOW))
  {
    Serial.println("[shiftOut] No response recieved");
    return;
  }
  digitalWrite(OUT_CLOCK, LOW);
  if (waitForSignalChange(OUT_RESPONSE, LOW))
  {
    Serial.println("[shiftOut] Response doesnt reset");
    return;
  }
}