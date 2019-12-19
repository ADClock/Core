#ifndef _CONFIG_H_
#define _CONFIG_H_

// Kalibierung überspringen, wenn z.B. kein Magnet verbaut ist
// #define SKIPCALIBRATION
// Debug Nachrichten +ber Serial senden?
// #define DEBUG

// TODO Nochmal richtigen Wert einstellen in us
#define DELAY_BETWEEN_COMMANDS 1000000
// Clock DataSender params
#define SENDER_TIMEOUT_RESPONSE_ON 90000
#define SENDER_TIMEOUT_RESPONSE_OFF 200000

#define RECEIVER_TIMEOUT_CLOCK_OFF 190000
#define RECEIVER_TIMEOUT_NEXT_DATA 900000

// #define IS_LAST_CLOCK

// Motor
#define MAX_STEPS 1705
// in micros
#define MIN_STEP_DELAY 4000

// Pins
#define HALL_DATA_PIN_1 A0
#define HALL_DATA_PIN_2 A1

#define MOTOR_1_PIN_1 10
#define MOTOR_1_PIN_2 11
#define MOTOR_1_PIN_3 12
#define MOTOR_1_PIN_4 13

#define MOTOR_2_PIN_1 9
#define MOTOR_2_PIN_2 8
#define MOTOR_2_PIN_3 7
#define MOTOR_2_PIN_4 6

// Pins for DataSender
#define OUT_RESPONSE 3
#define OUT_DATA 4
#define OUT_CLOCK 5

// Pins for DataReceiver
#define IN_RESPONSE A2
#define IN_DATA A3
#define IN_CLOCK 2

#endif