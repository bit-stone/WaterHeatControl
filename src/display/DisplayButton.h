#include <avr/io.h>

#include <ComponentState.h>

#ifndef _BITSTONE_DISPLAY_BUTTON
#define _BITSTONE_DISPLAY_BUTTON

#define DISPLAY_BUTTON_STATE_OPEN 0b0
#define DISPLAY_BUTTON_STATE_CLOSED 0b1
#define DISPLAY_BUTTON_STATE_CHANGE_TICK_COUNT 3

class DisplayButton
{
private:
    ComponentState *componentState;

    uint8_t lastButtonState = DISPLAY_BUTTON_STATE_OPEN;
    volatile uint8_t currentButtonState = DISPLAY_BUTTON_STATE_OPEN;
    uint8_t buttonTickCount = 0;

public:
    DisplayButton(ComponentState *componentState);
    void init();
    void handleButtonPressed();
    void handleButtonReleased();
    void tick();
};
#endif