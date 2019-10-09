#ifndef _CONFIG_H_
#define _CONFIG_H_

// =================================================
// Wall Layout
#define WALL_SIZE_X 12
#define WALL_SIZE_Y 6
#define WALL_CLOCKS WALL_SIZE_X *WALL_SIZE_Y
// =================================================

// =================================================
// Motor Information
#define MIN_STEP_DELAY 4000
#define STEPS_FOR_CIRCLE 1705
#define STEPS_PER_DEGREE STEPS_FOR_CIRCLE / 360.

#define RIGHT true
#define LEFT false
// =================================================

// =================================================
// Clock Communication
#define COMMAND_INIT 0x01
#define COMMAND_IMAGE 0x02

// TODO Nochmal richtigen Wert einstellen in us
#define DELAY_BETWEEN_COMMANDS 100000
// =================================================

// =================================================
// Flags für Testzwecke
#define SEND_ONLY_CLOCKS 2
// #define DEBUG
// =================================================

// =================================================
// Clock DataSender params
#define SENDER_TIMEOUT_RESPONSE_ON 60000
#define SENDER_TIMEOUT_RESPONSE_OFF 60000

// =================================================

// =================================================
// Pins
#define SENDER_RESPONSE D3
#define SENDER_DATA D4
#define SENDER_CLOCK D5
// =================================================

#endif