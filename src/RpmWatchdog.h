#include <avr/io.h>

#include <ComponentState.h>

#ifndef _BITSTONE_RPM_WATCHDOG
#define _BITSTONE_RPM_WATCHDOG

#define _BITSTONE_RPM_WATCHDOG_INITAL_STARTUP_TICKS 10 * 128

class RpmWatchdog
{
private:
    ComponentState *componentState;

    uint8_t initialStartupDelayCompleted = false;
    uint16_t initialStartupTicks = 0;

    uint16_t currentCount = 0;
    uint16_t lastCount = 0;
    uint16_t lastRpm = 10;
    uint16_t tickCount = 0;

public:
    RpmWatchdog(ComponentState *componentState);
    void init();
    void incrementCount();
    void tick();
    uint16_t getLastRpm();
};

#endif