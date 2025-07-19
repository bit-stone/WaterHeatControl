#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "ComponentState.h"

#ifndef _BITSTONE_PWM_OUTPUT
#define _BITSTONE_PWM_OUTPUT

#define PWM_OUTPUT_MAX_VALUE 640

#define PWM_OUTPUT_PUMP_LEVEL_1_PWM 400
#define PWM_OUTPUT_PUMP_LEVEL_2_PWM 450
#define PWM_OUTPUT_PUMP_LEVEL_3_PWM 500 // no max value needed, pump is quite powerful

#define PWM_OUTPUT_FAN_MIN_VALUE 130
#define PWM_OUTPUT_FAN_MAX_VALUE PWM_OUTPUT_MAX_VALUE
#define PWM_OUTPUT_FAN_PWM_DELTA (PWM_OUTPUT_FAN_MAX_VALUE - PWM_OUTPUT_FAN_MIN_VALUE)

#define PWM_OUTPUT_DELTA_T_MINIMUM 5.0f
#define PWM_OUTPUT_DELTA_T_MAXIMUM 20.0f
#define PWM_OUTPUT_DELTA_T_DELTA PWM_OUTPUT_DELTA_T_MAXIMUM - PWM_OUTPUT_DELTA_T_MINIMUM

class PwmOutput
{
private:
    ComponentState *componentState;

    uint16_t pumpPwmLevelValues[3] = {PWM_OUTPUT_PUMP_LEVEL_1_PWM, PWM_OUTPUT_PUMP_LEVEL_2_PWM, PWM_OUTPUT_PUMP_LEVEL_3_PWM};

    uint16_t nextValue;
    float delta_T;

public:
    PwmOutput(ComponentState *componentState);
    void initTimer();
    void updatePumpLevel(uint8_t value);
    void updatePumpPercentage(uint16_t pwmValue);
    void updateFanPwm(uint16_t value);
    void updateFanPercentage(uint16_t pwmValue);
    void update();
};

#endif