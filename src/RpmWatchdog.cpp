#include <RpmWatchdog.h>

/**
 * The RpmWatchdog is responsible for gathering the RPM data of the 
 * pump as well as the additional flow sensor within the loop.
 * Important:
 * This will only set visible lights on error. The acustic alarm is 
 * handled by AlarmBuzzer, which is responsible for reading the data
 * from this.
 * 
 * error codes
 * E01 - no rpm after initialization period is over - count: number of cycles with 0 rpm
 * 
 */

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

        if (componentState->rpmInitDelayCount > _BITSTONE_RPM_WATCHDOG_INITAL_STARTUP_TICKS)
        {
            componentState->rpmInitDelayOver = true;
        }
    }

    if (tickCount >= _BITSTONE_RPM_WATCHDOG_TICK_WINDOW)
    {
        tickCount = 0;
        lastCount = currentCount;
        currentCount = 0;
        lastRpm = (uint32_t) ((lastCount * 60)) >> 1;

        componentState->lastPumpRpm = lastRpm;

        if (lastRpm == 0 && componentState->rpmInitDelayOver == true)
        {
            this->componentState->error_noPumpRpm = true;
        }
    }
}

uint16_t RpmWatchdog::getLastRpm()
{
    return lastRpm;
}