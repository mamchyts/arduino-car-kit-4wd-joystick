#include "stdint.h"
#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>

uint8_t const PIN_BTN_A = 2;
uint8_t const PIN_BTN_B = 3;
uint8_t const PIN_BTN_C = 4;
uint8_t const PIN_BTN_D = 5;
uint8_t const PIN_JOY_X = A0;
uint8_t const PIN_JOY_Y = A1;

RF24 radio(9, 10); // CE, CSN

uint32_t address = 1234566789;
uint8_t payload[10] = {0};

void setup()
{
    pinMode(PIN_BTN_A, INPUT);
    pinMode(PIN_BTN_B, INPUT);
    pinMode(PIN_BTN_C, INPUT);
    pinMode(PIN_BTN_D, INPUT);
    pinMode(PIN_JOY_X, INPUT);
    pinMode(PIN_JOY_Y, INPUT);

    // setup nRF24L01 TX
    radio.begin();
    radio.setChannel(60);
    radio.setAutoAck(false);
    radio.setPALevel(RF24_PA_LOW);
    radio.openReadingPipe(1, address);
    radio.setPayloadSize(sizeof(payload));
    radio.stopListening();
    radio.openWritingPipe(address);
}

uint64_t millisPrevious = 0;

void loop()
{
    // all calculation only once per period
    uint64_t millisNow = millis();
    if ((millisNow - millisPrevious) >= 20) {
        millisPrevious = millisNow;

        payload[0] = !digitalRead(PIN_BTN_A);
        payload[1] = !digitalRead(PIN_BTN_B);
        payload[2] = !digitalRead(PIN_BTN_C);
        payload[3] = !digitalRead(PIN_BTN_D);
        payload[4] = map(analogRead(PIN_JOY_X), 0, 1023, 0, 255);
        payload[5] = map(analogRead(PIN_JOY_Y), 0, 1023, 0, 255);

        // send by wireless
        radio.write(&payload, radio.getPayloadSize());
    }
}
