#include <display/LedArray.h>

LedArray::LedArray() {}

void LedArray::init()
{
    // 3 pins are used:
    // PD 3 - Latch
    // PD 4 - Data
    // PD 5 - Clock

    PORTD_DIRSET = PIN5_bm | PIN4_bm | PIN3_bm;
    PORTD_OUTCLR = PIN5_bm | PIN4_bm | PIN3_bm;

    // turn all on
    ledState = 0b11111111;
    LedArray::show();

    // ledState is used like this:
    // LED STATE
    // 7  6  5  4  3  2  1  0
    // QH QG QF QE QD QC QB QA
}

void LedArray::clearAllLed()
{
    ledState = 0b0;
}

void LedArray::setErrorLed()
{
    ledState |= (1 << LED_ARRAY_ERROR_PIN);
}

void LedArray::show()
{
    for (k = 0; k < 8; k++)
    {
        if (ledState & (1 << (7 - k)))
        {
            LedArray::shiftHighOut();
        }
        else
        {
            LedArray::shiftLowOut();
        }
    }

    LedArray::pulseEnable();
}

void LedArray::shiftHighOut()
{
    LedArray::setDataHigh();
    LedArray::pulseClock();
}

void LedArray::shiftLowOut()
{
    LedArray::setDataLow();
    LedArray::pulseClock();
}

void LedArray::setDataLow()
{
    PORTD_OUTCLR = (1 << PIN4_bp);
}

void LedArray::setDataHigh()
{
    PORTD_OUTSET = (1 << PIN4_bp);
}

void LedArray::pulseClock()
{
    PORTD_OUTCLR = (1 << PIN5_bp);
    _delay_us(2);
    PORTD_OUTSET = (1 << PIN5_bp);
    _delay_us(2);
    PORTD_OUTCLR = (1 << PIN5_bp);
    _delay_us(2);
}

void LedArray::pulseEnable()
{
    PORTD_OUTCLR = (1 << PIN3_bp);
    _delay_us(2);
    PORTD_OUTSET = (1 << PIN3_bp);
    _delay_us(2);
    PORTD_OUTCLR = (1 << PIN3_bp);
    _delay_us(2);
}
