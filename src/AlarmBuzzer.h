#include <avr/io.h>

#include <ComponentState.h>

#ifndef _BITSTONE_ALARM_BUZZER
#define _BITSTONE_ALARM_BUZZER

class AlarmBuzzer
{
private:
    ComponentState *componentState;

    uint8_t alarmEnableCount = 0;

    uint8_t alarmTickCount = 0;
    bool alarmEnabled = false;

public:
    AlarmBuzzer(ComponentState *componentState);
    void init();
    void enableAlarm();
    void disableAlarm();
    void tick();
};

#endif