#include "display/DisplayButton.h"

DisplayButton::DisplayButton(ComponentState *componentState)
{
    this->componentState = componentState;
}

void DisplayButton::init()
{
    // use PC6 with pullup as input
    PORTC_DIRCLR = (1 << PIN6_bp);
    PORTC_PIN6CTRL |= (1 << PORT_PULLUPEN_bp);
}

void DisplayButton::handleButtonPressed()
{
    this->componentState->displayButtonPressedFlag = true;
}

void DisplayButton::handleButtonReleased()
{
    this->componentState->displayButtonReleasedFlag = true;
}

void DisplayButton::handleTick()
{
    currentButtonState = !(PORTC_IN & PIN6_bm);
    if (currentButtonState != lastButtonState)
    {
        buttonTickCount++;
        if (buttonTickCount >= DISPLAY_BUTTON_STATE_CHANGE_TICK_COUNT)
        {
            if (currentButtonState == DISPLAY_BUTTON_STATE_CLOSED)
            {
                DisplayButton::handleButtonPressed();
                lastButtonState = DISPLAY_BUTTON_STATE_CLOSED;
            }
            else
            {
                DisplayButton::handleButtonReleased();
                lastButtonState = DISPLAY_BUTTON_STATE_OPEN;
            }
        }
    }
    else
    {
        buttonTickCount = 0;
    }
}