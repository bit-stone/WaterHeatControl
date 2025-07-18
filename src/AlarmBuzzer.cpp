#include <AlarmBuzzer.h>

AlarmBuzzer::AlarmBuzzer(ComponentState *componentState)
{
    this->componentState = componentState;
}

void AlarmBuzzer::init()
{
    PORTF_DIRSET = (1 << PIN4_bp);
    this->disableAlarm();
}

void AlarmBuzzer::enableAlarm()
{
    this->alarmEnabled = true;
    PORTF_OUTSET = (1 << PIN4_bp);
}

void AlarmBuzzer::disableAlarm()
{
    this->alarmEnabled = false;
    PORTF_OUTCLR = (1 << PIN4_bp);
}

void AlarmBuzzer::tick()
{
    if (componentState->lastPumpRpm == 0 && componentState->rpmInitDelayOver == true)
    {
        this->alarmEnableCount++;
        if (this->alarmEnableCount > 250)
        {
            this->enableAlarm();
        }
    }
    else
    {
        this->alarmEnableCount = 0;
    }

    if (this->alarmEnabled == false)
    {
        this->alarmTickCount = 0;
        PORTF_OUTCLR = (1 << PIN4_bp);
    }
    else
    {
        this->alarmTickCount++;
        if (this->alarmTickCount >= 16)
        {
            this->alarmTickCount = 0;
            PORTF_OUTTGL = (1 << PIN4_bp);
        }
    }
}