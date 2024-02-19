#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define CLOCK_DELAY_US 5

// command to set data write to the chosen address register
#define WRITE_DATA_COMMAND 0b01000000

// last three bits used for actual address
#define ADDRESS_COMMAND_MASK 0b11000000

#define DISPLAY_ON_COMMAND 0b10001000
#define DISPLAY_OFF_COMMAND 0b10000000

#define DISPLAY_BRIGHTNESS_MASK 0b10000000

#define WITH_DECIMAL_POINT 0b10000000
#define SYMBOL_DEGREE 0b01100011
#define SYMBOL_L 0b00111000
#define SYMBOL_O 0b00111111
#define SYMBOL_W_LEFT 0b10111100
#define SYMBOL_W_RIGHT 0b00011110

#define SYMBOL_H 0b01110110
#define SYMBOL_I 0b00000110
#define SYMBOL_G 0b00111101

const uint8_t digitToSymbol[] = {
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

class Display
{
private:
    uint8_t digits[4] = {0, 0, 0, 0};

public:
    Display();
    void initDisplay();
    void clockDelay();
    void setClockLow();
    void setClockHigh();
    void setDataHigh();
    void setDataLow();
    void updateNumber(float input);
    void startCondition();
    void stopCondition();
    void show();
    void sendByte(uint8_t data);
    void sendDigit(uint8_t address, uint8_t data);
};