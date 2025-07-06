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

#define SEGMENT_DISPLAY_CLOCK_DELAY_US 2

#define SEGMENT_DISPLAY_ON_COMMAND 0b10001000
#define SEGMENT_DISPLAY_OFF_COMMAND 0b10000000

#define SEGMENT_DISPLAY_BRIGHTNESS_MASK 0b10000000

#define SEGMENT_DISPLAY_SYMBOL_ALL_SEGMENTS_ON 0b11111111

class SegmentDisplay
{
private:
    uint8_t output[4] = {0, 0, 0, 0};

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
    void setOutput(uint8_t *input);
};

#endif