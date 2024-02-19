#include <Display.h>
#include <util/delay.h>

Display::Display()
{
}

void Display::initDisplay()
{
    // set pb0 and pb1 as output, set output high by default
    // pb0 = data, pb1 = clock
    PORTB_DIRSET = PIN0_bm;
    PORTB_DIRSET = PIN1_bm;

    PORTB_OUTSET = PIN0_bm | PIN1_bm;

    digits[0] = 0b01111111;
    digits[1] = 0b11111111;
    digits[2] = 0b01111111;
    digits[3] = SYMBOL_DEGREE;

    Display::show();
}

void Display::clockDelay()
{
    _delay_us(CLOCK_DELAY_US);
}

void Display::setClockLow()
{
    PORTB_OUTCLR = PIN1_bm;
}

void Display::setClockHigh()
{
    PORTB_OUTSET = PIN1_bm;
}

void Display::setDataLow()
{
    PORTB_OUTCLR = PIN0_bm;
}

void Display::setDataHigh()
{
    PORTB_OUTSET = PIN0_bm;
}

void Display::updateNumber(float input)
{
    if(input < 0.0) {
        digits[0] = SYMBOL_L;
        digits[1] = SYMBOL_O;
        digits[2] = SYMBOL_W_LEFT;
        digits[3] = SYMBOL_W_RIGHT;
    } else if(input > 99.9) {
        digits[0] = SYMBOL_H;
        digits[1] = SYMBOL_I;
        digits[2] = SYMBOL_G;
        digits[3] = SYMBOL_H;
    } else {
        uint16_t output = (int) (input * 10);
        digits[0] = digitToSymbol[(int) (output / 100)];
        digits[1] = (digitToSymbol[(int) ((output % 100) / 10)] | WITH_DECIMAL_POINT);
        digits[2] = digitToSymbol[(int) ((output % 10))];
        digits[3] = SYMBOL_DEGREE;
    }

    Display::show();
}

void Display::startCondition()
{
    // set data pin as output again
    PORTB_DIRSET = PIN0_bm;

    // start condition: clock/data high, data goes low
    Display::setClockHigh();
    Display::setDataHigh();

    Display::clockDelay();

    Display::setDataLow();

    Display::clockDelay();
}

void Display::stopCondition()
{
    Display::setClockLow();
    Display::setDataLow();

    Display::clockDelay();

    Display::setClockHigh();

    Display::clockDelay();

    Display::setDataHigh();

    Display::clockDelay();
}

void Display::sendByte(uint8_t data)
{
    // sending stuff: change on clock low. clock should be below 250 kHz
    for (uint8_t k = 0; k < 8; k++)
    {
        Display::setClockLow();
        Display::clockDelay();

        if (data & (1 << k))
        {
            Display::setDataHigh();
        }
        else
        {
            Display::setDataLow();
        }

        Display::clockDelay();
        Display::setClockHigh();

        Display::clockDelay();
    }

    // ACK: Data will be pulled low by target chip on falling edge of 8th clock signal
    // set as input to detect ack with pullup enabled
    PORTB_DIRCLR = PIN0_bm;
    PORTB_PIN0CTRL = PORT_PULLUPEN_bm;
    Display::setClockLow();

    uint8_t count = 0;
    Display::clockDelay();

    while (count < 100 && !(PORTB_IN && (1 << PIN0_bp)))
    {
        Display::clockDelay();
        count++;
    }

    if (count >= 100)
    {
        Serial.println("ACK not received!");
        return;
    }

    // Data pin released after 9th clock
    Display::setClockHigh();
    Display::clockDelay();
    Display::setClockLow();

    // set data pin as output again
    PORTB_DIRSET = PIN0_bm;
}

void Display::show()
{
    for (uint8_t k = 0; k < 4; k++)
    {
        Display::sendDigit(k, digits[k]);
    }
}

void Display::sendDigit(uint8_t address, uint8_t data)
{
    Display::startCondition();
    Display::sendByte(WRITE_DATA_COMMAND);
    Display::stopCondition();

    Display::clockDelay();

    Display::startCondition();
    Display::sendByte(ADDRESS_COMMAND_MASK | address);

    Display::sendByte(data);
    Display::stopCondition();

    Display::startCondition();
    Display::sendByte(DISPLAY_ON_COMMAND | DISPLAY_BRIGHTNESS_MASK | 0b00000000);
    Display::stopCondition();
}