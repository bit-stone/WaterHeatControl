#include <AlarmBuzzer.h>

AlarmBuzzer::AlarmBuzzer(ComponentState *componentState)
{
    this->componentState = componentState;
}

void AlarmBuzzer::init()
{
    PORTF_DIRSET = (1 << PIN4_bp);
    this->clearAlarmBeeping();
}

void AlarmBuzzer::setAlarmBeeping()
{
    this->alarmIsBeeping = true;
    PORTF_OUTSET = (1 << PIN4_bp);
}

void AlarmBuzzer::clearAlarmBeeping()
{
    this->alarmIsBeeping = false;
    PORTF_OUTCLR = (1 << PIN4_bp);
}

void AlarmBuzzer::tick()
{
    
    if (!this->alarmIsBeeping)
    {
        // check if the alarm should be beeping
        if (componentState->lastPumpRpm == 0 && componentState->rpmInitDelayOver == true)
        {
            this->alarmEnableCount++;
            if (this->alarmEnableCount > 500)
            {
                this->setAlarmBeeping();
            }
        }
        else
        {
            this->alarmEnableCount = 0;
        }
    }
    else
    {
        // handle beeping pattern
        this->alarmTickCount++;
        if (this->alarmTickCount >= 16)
        {
            this->alarmTickCount = 0;
            PORTF_OUTTGL = (1 << PIN4_bp);
        }
    }
}