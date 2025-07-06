#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <PinDescription.h>

#ifndef _BITSTONE_SEGMENT_DISPLAY
#define _BITSTONE_SEGMENT_DISPLAY

/**
 * This section controls a 4 digit 7 segment display
 */

#define SEGMENT_DISPLAY_WRITE_DATA_COMMAND 0b01000000
#define SEGMENT_DISPLAY_ADDRESS_COMMAND_MASK 0b11000000

#define SEGMENT_DISPLAY_CLOCK_DELAY_US 5

#define SEGMENT_DISPLAY_ON_COMMAND 0b10001000
#define SEGMENT_DISPLAY_OFF_COMMAND 0b10000000

#define SEGMENT_DISPLAY_BRIGHTNESS_MASK 0b10000000

#define SEGMENT_DISPLAY_WITH_DECIMAL_POINT 0b10000000
#define SEGMENT_DISPLAY_SYMBOL_ALL_SEGMENTS_ON 0b11111111
#define SEGMENT_DISPLAY_SYMBOL_DEGREE 0b01100011
#define SEGMENT_DISPLAY_SYMBOL_L 0b00111000
#define SEGMENT_DISPLAY_SYMBOL_O 0b00111111
#define SEGMENT_DISPLAY_SYMBOL_W_LEFT 0b10111100
#define SEGMENT_DISPLAY_SYMBOL_W_RIGHT 0b00011110

#define SEGMENT_DISPLAY_SYMBOL_H 0b01110110
#define SEGMENT_DISPLAY_SYMBOL_I 0b00000110
#define SEGMENT_DISPLAY_SYMBOL_G 0b00111101

class SegmentDisplay
{
private:
    uint8_t output[4] = {0, 0, 0, 0};

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

    PinDescription clockPin;
    PinDescription dataPin;

    void setClockLow();
    void setClockHigh();
    void setDataLow();
    void setDataHigh();
    void clockDelay();
    void startCondition();
    void stopCondition();
    void sendByte(uint8_t data);

public:
    SegmentDisplay();
    void setPinData(PinDescription clockPin, PinDescription dataPin);
    void init();
    void show();
    void clear();
    void setSymbol(uint8_t idx, uint8_t symbol);
};

#endif