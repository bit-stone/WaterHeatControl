#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <display/SegmentDisplay.h>
#include <ComponentState.h>

#ifndef _BITSTONE_DISPLAY_CONTROLLER
#define _BITSTONE_DISPLAY_CONTROLLER

#define DISPLAY_CONTROLLER_MODE_RPM_AND_DELTA_T 0
#define DISPLAY_CONTROLLER_MODE_PWM 2
#define DISPLAY_CONTROLLER_MODE_TEMPERATURE 1

#define DISPLAY_CONTROLLER_MODE_MAX_VALUE 2

#define SEGMENT_DISPLAY_WITH_DECIMAL_POINT 0b10000000

#define SEGMENT_DISPLAY_SYMBOL_DEGREE 0b01100011
#define SEGMENT_DISPLAY_SYMBOL_L 0b00111000
#define SEGMENT_DISPLAY_SYMBOL_O 0b00111111
#define SEGMENT_DISPLAY_SYMBOL_W_LEFT 0b10111000
#define SEGMENT_DISPLAY_SYMBOL_W_RIGHT 0b00011110

#define SEGMENT_DISPLAY_SYMBOL_H 0b01110110
#define SEGMENT_DISPLAY_SYMBOL_I 0b00000110
#define SEGMENT_DISPLAY_SYMBOL_G 0b00111101

#define SEGMENT_DISPLAY_SYMBOL_E 0b01111001
#define SEGMENT_DISPLAY_SYMBOL_r 0b01010000

#define SEGMENT_DISPLAY_SYMBOL_D 0b01011110
#define SEGMENT_DISPLAY_SYMBOL_S 0b01101101
#define SEGMENT_DISPLAY_SYMBOL_P 0b01110011

class DisplayController
{
private:
    SegmentDisplay upperDisplay;
    SegmentDisplay lowerDisplay;

    uint8_t upperDisplayOutput[4] = {0, 0, 0, 0};
    uint8_t lowerDisplayOutput[4] = {0, 0, 0, 0};

    ComponentState *componentState;

    const uint8_t digitToSymbol[10] = {
        0b00111111, // 0
        0b00000110, // 1
        0b01011011, // 2
        0b01001111, // 3
        0b01100110, // 4
        0b01101101, // 5
        0b01111101, // 6
        0b00000111, // 7
        0b01111111, // 8
        0b01101111, // 9
    };

    uint8_t displayMode = DISPLAY_CONTROLLER_MODE_RPM_AND_DELTA_T;

    void setNextDisplayMode();
    void applyTemperatureFloatToOutput(float input, uint8_t *target);
    void applyIntegerToOutput(uint16_t input, uint8_t *target);

public:
    DisplayController(ComponentState *componentState);
    void init();
    void update();
};
#endif