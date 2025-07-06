#include <display/DisplayController.h>

DisplayController::DisplayController()
{
    PinDescription upperDataPin = {
        .portOutSet = &PORTB_OUTSET,
        .portOutClr = &PORTB_OUTCLR,
        .portDirSet = &PORTB_DIRSET,
        .portDirClr = &PORTB_DIRCLR,
        .pinControl = &PORTB_PIN0CTRL,
        .portIn = &PORTB_IN,
        .pinIndex = PIN0_bp};

    PinDescription upperClockPin = {
        .portOutSet = &PORTB_OUTSET,
        .portOutClr = &PORTB_OUTCLR,
        .portDirSet = &PORTB_DIRSET,
        .portDirClr = &PORTB_DIRCLR,
        .pinControl = &PORTB_PIN1CTRL,
        .portIn = &PORTB_IN,
        .pinIndex = PIN1_bp};

    this->upperDisplay.setPinData(upperClockPin, upperDataPin);

    PinDescription lowerDataPin = {
        .portOutSet = &PORTE_OUTSET,
        .portOutClr = &PORTE_OUTCLR,
        .portDirSet = &PORTE_DIRSET,
        .portDirClr = &PORTE_DIRCLR,
        .pinControl = &PORTE_PIN0CTRL,
        .portIn = &PORTE_IN,
        .pinIndex = PIN0_bp};

    PinDescription lowerClockPin = {
        .portOutSet = &PORTE_OUTSET,
        .portOutClr = &PORTE_OUTCLR,
        .portDirSet = &PORTE_DIRSET,
        .portDirClr = &PORTE_DIRCLR,
        .pinControl = &PORTE_PIN1CTRL,
        .portIn = &PORTE_IN,
        .pinIndex = PIN1_bp};

    this->lowerDisplay.setPinData(lowerClockPin, lowerDataPin);
}

void DisplayController::init()
{
    this->upperDisplay.init();
    this->lowerDisplay.init();
}