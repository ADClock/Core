#ifndef _CONFIG_H_
#define _CONFIG_H_

// Debug Nachrichten +ber Serial senden?
// #define DEBUG

// TODO Nochmal richtigen Wert einstellen in us
#define DELAY_BETWEEN_COMMANDS 1000000
// Clock DataSender params
#define SENDER_TIMEOUT_RESPONSE_ON 90000
#define SENDER_TIMEOUT_RESPONSE_OFF 200000

#define RECEIVER_TIMEOUT_CLOCK_OFF 190000
#define RECEIVER_TIMEOUT_NEXT_DATA 900000

// Pins for DataSender
#define OUT_RESPONSE 13
#define OUT_DATA 12
#define OUT_CLOCK 14

// Pins for DataReceiver
#define IN_RESPONSE 32
#define IN_DATA 35
#define IN_CLOCK 34

#endif