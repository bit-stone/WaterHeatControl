#include <display/LedArray.h>
#include "LedArray.h"

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

    // turn all on briefly
    LedArray::setAllLed();
    LedArray::show();
    _delay_ms(400);
    LedArray::clearAllLed();
    LedArray::show();
  
    // make little bottom to top animation
    this->ledState = 0b10010000;
    LedArray::show();
    _delay_ms(250);

    this->ledState = 0b00000001;
    LedArray::show();
    _delay_ms(250);

    this->ledState = 0b01001000;
    LedArray::show();
    _delay_ms(250);

    this->ledState = 0b00000010;
    LedArray::show();
    _delay_ms(250);

    this->ledState = 0b00100100;
    LedArray::show();
    _delay_ms(250);

    LedArray::clearAllLed();
    LedArray::show();

    // ledState is used like this:
    // LED STATE
    // 7  6  5  4  3  2  1  0
    // QH QG QF QE QD QC QB QA
}

void LedArray::clearAllLed()
{
    this->ledState = 0b0;
}

void LedArray::setAllLed()
{
    this->ledState = 0b11111111;
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
        if (this->ledState & (1 << (7 - k)))
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

uint8_t LedArray::getLevelForDeltaT(float delta_T)
{
    // always reset to off first
    if (delta_T < LED_LEVEL_1_DELTA_T)
    {
        return 1; // all off
    }
    else if (delta_T < LED_LEVEL_2_DELTA_T)
    {
        return 2; // first level shown
    }
    else if (delta_T < LED_LEVEL_3_DELTA_T)
    {
        return 3; // second level shown
    }
    else
    {
        return 4; // all level shown
    }
}

void LedArray::update()
{
    this->lastLedState = this->ledState;
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
    this->deltaTLevel = this->getLevelForDeltaT(delta_T);

    // initial (empty) value, set the values according to current reading
    if (this->lastDeltaTLevel == 0)
    {
        this->lastDeltaTLevel = this->deltaTLevel;
    }

    if (this->lastDeltaTLevel != this->deltaTLevel)
    {
        switch (this->lastDeltaTLevel)
        {
        case 1:
            if (delta_T > (LED_LEVEL_1_DELTA_T + DELTA_T_LEVEL_SWITCH_DIFF))
            {
                this->deltaTLevel = 2;
                this->lastDeltaTLevel = this->deltaTLevel;
            }
            break;
        case 2:
            if (delta_T > (LED_LEVEL_2_DELTA_T + DELTA_T_LEVEL_SWITCH_DIFF))
            {
                this->deltaTLevel = 3;
                this->lastDeltaTLevel = this->deltaTLevel;
            }
            if (delta_T < (LED_LEVEL_1_DELTA_T - DELTA_T_LEVEL_SWITCH_DIFF))
            {
                this->deltaTLevel = 1;
                this->lastDeltaTLevel = this->deltaTLevel;
            }
            break;
        case 3:
            if (delta_T > (LED_LEVEL_3_DELTA_T + DELTA_T_LEVEL_SWITCH_DIFF))
            {
                this->deltaTLevel = 4;
                this->lastDeltaTLevel = this->deltaTLevel;
            }
            if (delta_T < (LED_LEVEL_2_DELTA_T - DELTA_T_LEVEL_SWITCH_DIFF))
            {
                this->deltaTLevel = 2;
                this->lastDeltaTLevel = this->deltaTLevel;
            }
            break;
        default:
            if (delta_T < (LED_LEVEL_3_DELTA_T - DELTA_T_LEVEL_SWITCH_DIFF))
            {
                this->deltaTLevel = 3;
                this->lastDeltaTLevel = this->deltaTLevel;
            }
            break;
        }
    }

    switch (this->lastDeltaTLevel)
    {
    case 1:
        this->ledState |= (0b00000000);
        break;
    case 2:
        this->ledState |= (0b10000000);
        break;
    case 3:
        this->ledState |= (0b11000000);
        break;
    default:
        this->ledState |= (0b11100000);
        break;
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
        this->ledState |= (1 << (2 + this->componentState->displayMode));
        break;
    default:
        break;
    }

    // last, two error LEDs. If their flag is set, set them to blinking,
    // otherwise off.
    if (this->componentState->error_noPumpRpm == true)
    {
        this->ledState |= (1 << 0);
    }

    if (this->componentState->error_generic == true)
    {
        this->ledState |= (1 << 1);
    }

    // shift them out (but only on update or when forced)
    if (this->lastLedState != this->ledState)
    {
        this->forceUpdateTicks = 0;
        this->show();
    }
    else
    {
        this->forceUpdateTicks++;

        // there should be a forced update every so often
        // otherwise if no changes are detected, glitches might solidify forever
        if (this->forceUpdateTicks >= FORCE_UPDATE_TICK_COUNT)
        {
            this->forceUpdateTicks = 0;
            this->show();
        }
    }
}
