#include <avr/io.h>
#include <util/delay.h>
#include <Arduino.h>

#include <ComponentState.h>

#ifndef _BITSTONE_LED_ARRAY
#define _BITSTONE_LED_ARRAY

#define LED_ARRAY_ERROR_PIN 1
#define FORCE_UPDATE_TICK_COUNT 100 // force a shift every so often to get rid of glitches

#define LED_LEVEL_1_DELTA_T 8.0f
#define LED_LEVEL_2_DELTA_T 13.0f
#define LED_LEVEL_3_DELTA_T 16.0f

#define DELTA_T_LEVEL_SWITCH_DIFF 0.5f

class LedArray
{
private:
    ComponentState *componentState;

    uint8_t forceUpdateTicks = 0;

    uint8_t lastDeltaTLevel = 0; // 0 => initial value. levels after will be 1 or more.
    uint8_t deltaTLevel = 0; // same here.

    uint8_t lastLedState = 0b00000000;
    uint8_t ledState = 0b00000000;
    uint8_t k = 0;

    void shiftHighOut();
    void shiftLowOut();
    void setDataLow();
    void setDataHigh();
    void pulseClock();
    void pulseEnable();
    void clearAllLed();
    void setAllLed();


public:
    LedArray(ComponentState *componentState);
    void init();
    void show();
    uint8_t getLevelForDeltaT(float delta_T);
    void update();
};

#endif