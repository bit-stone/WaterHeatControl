#include "PwmOutput.h"

PwmOutput::PwmOutput(ComponentState *componentState)
{
    this->componentState = componentState;
}

void PwmOutput::initTimer()
{
    // target: 25kHz signal on two output channels
    // configure pins as output
    PORTA_DIR |= (PIN0_bm | PIN1_bm);
    PORTA_OUT |= (PIN0_bm | PIN1_bm);

    // set to output timer on port a (A0 and A1)
    PORTMUX.TCAROUTEA = PORTMUX_TCA0_PORTA_gc;

    // set period and clock div
    TCA0_SINGLE_PER = PWM_OUTPUT_MAX_VALUE - 1;
    TCA0_SINGLE_CTRLA = 0;

    // set default cmp to 100%
    TCA0_SINGLE_CMP0 = PWM_OUTPUT_MAX_VALUE;
    TCA0_SINGLE_CMP1 = PWM_OUTPUT_MAX_VALUE;

    // enable two compare channel
    TCA0_SINGLE_CTRLB |= TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1_bm;

    // set waveform generation mode
    TCA0_SINGLE_CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    // enable timer
    TCA0_SINGLE_CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void PwmOutput::updatePumpLevel(uint8_t level)
{
    if (level <= 1)
    {
        level = 1;
    }

    if (level > 3)
    {
        level = 3;
    }

    TCA0_SINGLE_CMP0 = this->pumpPwmLevelValues[level - 1];
    this->updatePumpPercentage(this->pumpPwmLevelValues[level - 1]);
}

void PwmOutput::updatePumpPercentage(uint16_t pwmValue)
{
    this->componentState->lastPumpPWMPercentage = (int)(((float)pwmValue / PWM_OUTPUT_MAX_VALUE) * 100);
}

void PwmOutput::updateFanPwm(uint16_t value)
{
    if (value < PWM_OUTPUT_FAN_MIN_VALUE)
    {
        value = PWM_OUTPUT_FAN_MIN_VALUE;
    }

    if (value > PWM_OUTPUT_FAN_MAX_VALUE)
    {
        value = PWM_OUTPUT_FAN_MAX_VALUE;
    }
    TCA0_SINGLE_CMP1 = value;
    this->updateFanPercentage(value);
}

void PwmOutput::updateFanPercentage(uint16_t pwmValue)
{
    this->componentState->lastFanPWMPercentage = (int)(((float)pwmValue / PWM_OUTPUT_MAX_VALUE) * 100);
}

void PwmOutput::update()
{
    this->delta_T = this->componentState->delta_T;

    if (this->delta_T < PWM_OUTPUT_DELTA_T_MINIMUM)
    {
        // go minimal values
        updatePumpLevel(1);
        updateFanPwm(PWM_OUTPUT_FAN_MIN_VALUE);
    }
    else if (this->delta_T > PWM_OUTPUT_DELTA_T_MAXIMUM)
    {
        // go highest level on pump
        updatePumpLevel(3);
        // go full blast on fans
        updateFanPwm(PWM_OUTPUT_FAN_MAX_VALUE);
    }
    else
    {
        // single level for pump
        updatePumpLevel(2);
        // linear interpolation
        nextValue = (((this->delta_T - PWM_OUTPUT_DELTA_T_MINIMUM) * PWM_OUTPUT_FAN_PWM_DELTA) / PWM_OUTPUT_DELTA_T_DELTA) + PWM_OUTPUT_FAN_MIN_VALUE;
        updateFanPwm(nextValue);
    }
}