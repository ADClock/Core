#ifndef _CONFIG_H_
#define _CONFIG_H_
#include "Arduino.h"

// =================================================
// Wall Layout
#define WALL_SIZE_X 2
#define WALL_SIZE_Y 3
#define WALL_CLOCKS WALL_SIZE_X *WALL_SIZE_Y
// =================================================

// =================================================
// Motor Information
#define MIN_STEP_DELAY 4000
#define STEPS_FOR_CIRCLE 1705
#define STEPS_PER_DEGREE STEPS_FOR_CIRCLE / 360.

// =================================================

// =================================================
// Clock Communication
#define COMMAND_INIT 0x01
#define COMMAND_IMAGE 0x02

// TODO Nochmal richtigen Wert einstellen in us
#define DELAY_BETWEEN_COMMANDS 6000

#define CLOCK_INIT_TIME 10000000
// =================================================

// =================================================
// Flags für Testzwecke
#define SEND_ONLY_CLOCKS 2
// #define DEBUG
// =================================================

// =================================================
// Clock DataSender params
#define SENDER_TIMEOUT_RESPONSE_ON 100  //60000
#define SENDER_TIMEOUT_RESPONSE_OFF 100 //60000

// =================================================

// =================================================
// Pins
#define OUT_RESPONSE 25 // zu A2
#define OUT_DATA 26     // zu A3
#define OUT_CLOCK 27    // zu A4

// =================================================

#endif