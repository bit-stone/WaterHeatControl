#include <avr/io.h>

#include <ComponentState.h>

#ifndef _BITSTONE_ALARM_BUZZER
#define _BITSTONE_ALARM_BUZZER

class AlarmBuzzer
{
private:
    ComponentState *componentState;

    uint16_t alarmEnableCount = 0;
    uint8_t alarmTickCount = 0;
    
    bool alarmIsBeeping = false;

public:
    AlarmBuzzer(ComponentState *componentState);
    void init();
    void setAlarmBeeping();
    void clearAlarmBeeping();
    void tick();
};

#endif