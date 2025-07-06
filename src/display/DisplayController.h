#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

#include <display/SegmentDisplay.h>

#ifndef _BITSTONE_DISPLAY_CONTROLLER
#define _BITSTONE_DISPLAY_CONTROLLER

class DisplayController
{
private:
    SegmentDisplay upperDisplay;
    SegmentDisplay lowerDisplay;

public:
    DisplayController();
    void init();
};
#endif