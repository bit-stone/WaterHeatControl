#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#include <ComponentState.h>

#ifndef _BITSTONE_LED_ARRAY
#define _BITSTONE_LED_ARRAY

#define LED_ARRAY_ERROR_PIN 1

class LedArray
{
private:
    ComponentState *componentState;

    uint8_t ledState = 0b00000000;
    uint8_t k = 0;

    void shiftHighOut();
    void shiftLowOut();
    void setDataLow();
    void setDataHigh();
    void pulseClock();
    void pulseEnable();
    void clearAllLed();

public:
    LedArray(ComponentState *componentState);
    void init();
    void show();
    void update();
};

#endif