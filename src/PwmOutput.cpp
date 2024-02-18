#include "PwmOutput.h"

PwmOutput::PwmOutput()
{
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
    TCA0_SINGLE_PER = PWM_MAX_VALUE - 1;
    TCA0_SINGLE_CTRLA = 0;

    // set default cmp to 100%
    TCA0_SINGLE_CMP0 = PWM_MAX_VALUE;
    TCA0_SINGLE_CMP1 = PWM_MAX_VALUE;

    // enable two compare channel
    TCA0_SINGLE_CTRLB |= TCA_SINGLE_CMP0EN_bm | TCA_SINGLE_CMP1_bm;

    // set waveform generation mode
    TCA0_SINGLE_CTRLB |= TCA_SINGLE_WGMODE_SINGLESLOPE_gc;

    // enable timer
    TCA0_SINGLE_CTRLA |= TCA_SINGLE_ENABLE_bm;
}

void PwmOutput::updatePumpPwm(uint16_t value)
{
    if (value < PUMP_PWM_MINIMAL_VALUE)
    {
        value = PUMP_PWM_MINIMAL_VALUE;
    }

    if (value > PWM_MAX_VALUE)
    {
        value = PWM_MAX_VALUE;
    }

    TCA0_SINGLE_CMP0 = value;
}

void PwmOutput::updateFanPwm(uint16_t value)
{
    if (value < FAN_PWM_MINIMAL_VALUE)
    {
        value = FAN_PWM_MINIMAL_VALUE;
    }

    if (value > PWM_MAX_VALUE)
    {
        value = PWM_MAX_VALUE;
    }
    TCA0_SINGLE_CMP1 = value;
}

void PwmOutput::updateTemperature(float temperature)
{
    if (temperature < TEMPERATURE_MIN)
    {
        // go minimal values
        updateFanPwm(FAN_PWM_MINIMAL_VALUE);
        updatePumpPwm(PUMP_PWM_MINIMAL_VALUE);
    }
    else if (temperature < TEMPERATURE_MAX)
    {
        // linear interpolation
        nextValue = (((temperature - TEMPERATURE_MIN) * PUMP_PWM_DELTA) / TEMPERATURE_DELTA) + PUMP_PWM_MINIMAL_VALUE;
        updatePumpPwm(nextValue);
        Serial.println(nextValue);

        nextValue = (((temperature - TEMPERATURE_MIN) * FAN_PWM_DELTA) / TEMPERATURE_DELTA) + FAN_PWM_MINIMAL_VALUE;
        updateFanPwm(nextValue);
        Serial.println(nextValue);
    }
    else
    {
        // go full blast on fans
        updateFanPwm(FAN_PWM_MAXIMAL_VALUE);

        // go half power on pump
        updatePumpPwm(PUMP_PWM_MAXIMAL_VALUE);
    }
}