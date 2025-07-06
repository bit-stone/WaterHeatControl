#include <avr/io.h>

#ifndef _BITSTONE_COMPONENT_STATE
#define _BITSTONE_COMPONENT_STATE
struct ComponentState
{
    float waterTemperature = 0.0f;
    float airTemperature = 0.0f;
    float delta_T = 0.0f;

    uint16_t lastFanRpm = 0;
    uint16_t lastPumpRpm = 0;

    uint16_t lastPumpPWMPercentage = 0;
    uint16_t lastFanPWMPercentage = 0;

    bool displayButtonPressedFlag = false;

    bool error_noPumpRpm = false;
    bool error_highDeltaT = false;
};
#endif