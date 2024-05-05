#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#define PUMP_PWM_MINIMAL_VALUE 400
#define PUMP_PWM_MAXIMAL_VALUE 640
#define PUMP_PWM_DELTA (PUMP_PWM_MAXIMAL_VALUE - PUMP_PWM_MINIMAL_VALUE)

#define FAN_PWM_MINIMAL_VALUE 130
#define FAN_PWM_MAXIMAL_VALUE 640
#define FAN_PWM_DELTA (FAN_PWM_MAXIMAL_VALUE - FAN_PWM_MINIMAL_VALUE)

#define PWM_MAX_VALUE 640

#define TEMPERATURE_MIN 25.0
#define TEMPERATURE_MAX 35.0
#define TEMPERATURE_DELTA (TEMPERATURE_MAX - TEMPERATURE_MIN)

class PwmOutput
{
private:
    uint16_t nextValue;

public:
    PwmOutput();
    void initTimer();
    void updatePumpPwm(uint16_t value);
    void updateFanPwm(uint16_t value);
    void updateTemperature(float tempterature);
};