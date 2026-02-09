#include <avr/io.h>

#include <ComponentState.h>

#ifndef _BITSTONE_RPM_WATCHDOG
#define _BITSTONE_RPM_WATCHDOG

#define _BITSTONE_RPM_WATCHDOG_INITAL_STARTUP_TICKS 10 * 128
#define _BITSTONE_RPM_WATCHDOG_TICK_WINDOW 128  // 1 second at 128Hz

class RpmWatchdog
{
private:
    ComponentState *componentState;

    volatile uint16_t currentCount = 0;  // Modified by ISR
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