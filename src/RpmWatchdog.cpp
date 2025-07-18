#include <RpmWatchdog.h>

RpmWatchdog::RpmWatchdog(ComponentState *componentState)
{
    this->componentState = componentState;
}

void RpmWatchdog::init()
{
    // using PF5 to detect rpm. Needs to be input with pullup
    // also enable the pin interrupt on this pin

    // for timing, use main tick timer

    // pin as input
    PORTF_DIRCLR = (1 << PIN5_bp);

    // pin pullup
    PORTF_PIN5CTRL |= PORT_PULLUPEN_bm;

    // detect rising edge
    PORTF_PIN5CTRL |= PORT_ISC_FALLING_gc;

    PORTD_DIRSET = (1 << PIN5_bp);
}

void RpmWatchdog::incrementCount()
{
    currentCount++;
}

void RpmWatchdog::tick()
{
    tickCount++;

    if (componentState->rpmInitDelayOver == false)
    {
        componentState->rpmInitDelayCount++;

        if (componentState->rpmInitDelayCount++ > _BITSTONE_RPM_WATCHDOG_INITAL_STARTUP_TICKS)
        {
            componentState->rpmInitDelayOver = true;
        }
    }

    if (tickCount >= 128)
    {
        tickCount = 0;
        lastCount = currentCount;
        currentCount = 0;
        lastRpm = (lastCount * 60) >> 1;

        componentState->lastPumpRpm = lastRpm;

        if (lastRpm == 0 && initialStartupDelayCompleted == true)
        {
            this->componentState->error_noPumpRpm = true;
        }
    }
}

uint16_t RpmWatchdog::getLastRpm()
{
    return lastRpm;
}