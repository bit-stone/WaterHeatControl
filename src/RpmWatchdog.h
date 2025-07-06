#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#ifndef _BITSTONE_RPM_WATCHDOG
#define _BITSTONE_RPM_WATCHDOG

class RpmWatchdog
{
private:
    uint16_t currentCount = 0;
    uint16_t lastCount = 0;
    uint16_t lastRpm = 0;
    uint16_t tickCount = 0;

public:
    RpmWatchdog();
    void init();
    void incrementCount();
    void tick();
    uint16_t getLastRpm();
};

#endif