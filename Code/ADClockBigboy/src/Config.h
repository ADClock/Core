#ifndef _CONFIG_H_
#define _CONFIG_H_

// =================================================
// Wall Layout
#define WALL_SIZE_X 12
#define WALL_SIZE_Y 6
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

// TODO Nochmal richtigen Wert einstellen in ms
#define DELAY_BETWEEN_COMMANDS 1000
// =================================================

// =================================================
// Flags für Testzwecke
#define SEND_ONLY_CLOCKS 2
#define DEBUG
// =================================================

// Pins
#define OUT_RESPONSE D3
#define OUT_DATA D4
#define OUT_CLOCK D5

#endif