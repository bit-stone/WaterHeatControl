#include <display/LedArray.h>

LedArray::LedArray(ComponentState *componentState)
{
    this->componentState = componentState;
}

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

void LedArray::update()
{
    // clear all led so nothing "lingers"
    this->clearAllLed();

    // set leds based on component state

    // first, deal with delta_T
    // has three LEDs.
    // zero leds on - <2.0 degress
    // one led on - <6.0 degrees
    // two led on - <10.0 degrees
    // three led on - > 10.0
    float delta_T = this->componentState->delta_T;

    // always reset to off first
    if (delta_T < 2.0)
    {
        ledState |= (0b00000000);
    }
    else if (delta_T < 6.0)
    {
        ledState |= (0b10000000);
    }
    else if (delta_T < 10.0)
    {
        ledState |= (0b11000000);
    }
    else
    {
        ledState |= (0b11100000);
    }

    // next, display mode. is easy, just show the led for the mode
    // first led - rpm/delta_T
    // second led - absolute temps
    // third led - pwm percentages
    // reset first

    switch (this->componentState->displayMode)
    {
    case 0:
    case 1:
    case 2:
        ledState |= (1 << (2 + this->componentState->displayMode));
        break;
    default:
        break;
    }

    Serial.println(ledState, BIN);

    // last, two error LEDs. If their flag is set, set them to blinking,
    // otherwise off.
    if(this->componentState->error_noPumpRpm == true) {
        ledState |= (1 << 0);
    }

    if(this->componentState->error_generic == true) {
        ledState |= (1 << 1);
    }

    // shift them out
    this->show();
}
