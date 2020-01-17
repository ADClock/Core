#ifndef _CONFIG_H_
#define _CONFIG_H_

// Buffer
#define BUFFER_SIZE 512

// Calibration
#define MIN_STEPS_OUTSIDE_FIELD 20

// Communication parameters
#define DELAY_BETWEEN_COMMANDS 5000        // microseconds
#define SENDER_TIMEOUT_RESPONSE_ON 1200    // microseconds
#define SENDER_TIMEOUT_RESPONSE_OFF 1200   // microseconds
#define SENDER_WAIT_FOR_DATA_IN_BUFFER 800 // microseconds (no real timeout!)
#define RECEIVER_TIMEOUT_CLOCK_OFF 1000    // microseconds
#define RECEIVER_TIMEOUT_NEXT_DATA 1000    // microseconds
#define IS_LAST_CLOCK                      // Disables sending Data to next clock

// Motor
#define MAX_STEPS 1705
#define MIN_STEP_DELAY 4000 // us

// Pins
#define HALL_DATA_PIN_1 A1
#define HALL_DATA_PIN_2 A0

#define MOTOR_1_PIN_1 10
#define MOTOR_1_PIN_2 11
#define MOTOR_1_PIN_3 12
#define MOTOR_1_PIN_4 13

#define MOTOR_2_PIN_1 9
#define MOTOR_2_PIN_2 8
#define MOTOR_2_PIN_3 7
#define MOTOR_2_PIN_4 6

// Pins for DataSender (Sending to next Arduino)
#define OUT_RESPONSE 3
#define OUT_DATA 4
#define OUT_CLOCK 5

// Pins for DataReceiver (Receiving from previous Arduino or ESP)
#define IN_RESPONSE A2
#define IN_DATA A3
#define IN_CLOCK 2

// Skip Calibration (for unfinished clock modules with no magnet)
// #define SKIPCALIBRATION
// Enable Debug messages for Serial
// #define DEBUG

#endif