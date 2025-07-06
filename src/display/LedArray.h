#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#ifndef _BITSTONE_LED_ARRAY
#define _BITSTONE_LED_ARRAY

#define LED_ARRAY_ERROR_PIN 1

class LedArray
{
private:
    uint8_t ledState = 0b01010101;
    uint8_t k = 0;

public:
    LedArray();
    void init();
    void clearAllLed();
    void setErrorLed();
    void show();
    void shiftHighOut();
    void shiftLowOut();
    void setDataLow();
    void setDataHigh();
    void pulseClock();
    void pulseEnable();
};

#endif