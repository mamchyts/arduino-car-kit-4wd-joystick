#include "stdint.h"
#include <Arduino.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <SPI.h>

#define DEBUG 0

uint8_t const PIN_BTN_A = 2;
uint8_t const PIN_BTN_B = 3;
uint8_t const PIN_BTN_C = 4;
uint8_t const PIN_BTN_D = 5;

uint8_t const PIN_JOY_X = A0;
uint8_t const PIN_JOY_Y = A1;

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00001";

void setup()
{
    if (DEBUG) {
        Serial.begin(115200);
    }

    pinMode(PIN_BTN_A, INPUT);
    pinMode(PIN_BTN_B, INPUT);
    pinMode(PIN_BTN_C, INPUT);
    pinMode(PIN_BTN_D, INPUT);

    pinMode(PIN_JOY_X, INPUT);
    pinMode(PIN_JOY_Y, INPUT);

    // setup nRF24L01 TX
    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}

uint64_t millisPrevious = 0;

// array of BTNs state (A, B, C, D) and JOY positions (X, Y)
int8_t joy[6] = {0};

void loop()
{
    // all calculation only once per period
    uint64_t millisNow = millis();
    if ((millisNow - millisPrevious) >= 20) {
        millisPrevious = millisNow;

        joy[0] = !digitalRead(PIN_BTN_A);
        joy[1] = !digitalRead(PIN_BTN_B);
        joy[2] = !digitalRead(PIN_BTN_C);
        joy[3] = !digitalRead(PIN_BTN_D);

        joy[4] = map(analogRead(PIN_JOY_X), 0, 700, -1, 2);
        joy[5] = map(analogRead(PIN_JOY_Y), 0, 700, -1, 2);

        // send by wireless
        radio.write(&joy, sizeof(joy));

        if (DEBUG) {
            Serial.print("\tA ");
            Serial.print(joy[0]);
            Serial.print("\tB ");
            Serial.print(joy[1]);
            Serial.print("\tC ");
            Serial.print(joy[2]);
            Serial.print("\tD ");
            Serial.print(joy[3]);
            Serial.print("\tX ");
            Serial.print(joy[4]);
            Serial.print("\tY ");
            Serial.println(joy[5]);
        }
    }
}
