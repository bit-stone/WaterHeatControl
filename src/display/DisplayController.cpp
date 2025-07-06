#include <display/DisplayController.h>
#include "DisplayController.h"

DisplayController::DisplayController(ComponentState *componentState)
{
    this->componentState = componentState;

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

void DisplayController::setNextDisplayMode()
{
    if (this->displayMode + 1 > DISPLAY_CONTROLLER_MODE_MAX_VALUE)
    {
        this->displayMode = 0;
    }
    else
    {
        this->displayMode++;
    }

    this->componentState->displayMode = this->displayMode;
}

void DisplayController::applyTemperatureFloatToOutput(float input, uint8_t *target)
{
    if (input > 99)
    {
        target[0] = SEGMENT_DISPLAY_SYMBOL_H;
        target[1] = SEGMENT_DISPLAY_SYMBOL_I;
        target[2] = SEGMENT_DISPLAY_SYMBOL_G;
        target[3] = SEGMENT_DISPLAY_SYMBOL_H;
        return;
    }

    if (input < 1)
    {
        target[0] = SEGMENT_DISPLAY_SYMBOL_L;
        target[1] = SEGMENT_DISPLAY_SYMBOL_O;
        target[2] = SEGMENT_DISPLAY_SYMBOL_W_LEFT;
        target[3] = SEGMENT_DISPLAY_SYMBOL_W_RIGHT;
        return;
    }

    uint16_t value = (int)(input * 10);
    target[0] = digitToSymbol[(int)(value / 100)];
    target[1] = digitToSymbol[(int)((value % 100) / 10)] | SEGMENT_DISPLAY_WITH_DECIMAL_POINT;
    target[2] = digitToSymbol[(int)(value % 10)];
    target[3] = SEGMENT_DISPLAY_SYMBOL_DEGREE;

    // clear leading zeros
    for (uint8_t k = 0; k < 1; k++)
    {
        if (target[k] != digitToSymbol[0])
        {
            break;
        }

        if (target[k] == digitToSymbol[0])
        {
            target[k] = 0b0;
        }
    }
}

void DisplayController::applyIntegerToOutput(uint16_t input, uint8_t *target)
{
    if (input > 9999)
    {
        target[0] = SEGMENT_DISPLAY_SYMBOL_H;
        target[1] = SEGMENT_DISPLAY_SYMBOL_I;
        target[2] = SEGMENT_DISPLAY_SYMBOL_G;
        target[3] = SEGMENT_DISPLAY_SYMBOL_H;
        return;
    }

    target[0] = digitToSymbol[(int)(input / 1000)];
    target[1] = digitToSymbol[(int)((input % 1000) / 100)];
    target[2] = digitToSymbol[(int)((input % 100) / 10)];
    target[3] = digitToSymbol[(int)(input % 10)];

    // clear leading zeros
    for (uint8_t k = 0; k < 3; k++)
    {
        if (target[k] != digitToSymbol[0])
        {
            break;
        }

        if (target[k] == digitToSymbol[0])
        {
            target[k] = 0b0;
        }
    }
}

void DisplayController::update()
{
    if (this->componentState->displayButtonPressedFlag == true)
    {
        this->componentState->displayButtonPressedFlag = false;
        this->setNextDisplayMode();
    }

    switch (this->displayMode)
    {
    case DISPLAY_CONTROLLER_MODE_TEMPERATURE:
        this->applyTemperatureFloatToOutput(this->componentState->waterTemperature, this->upperDisplayOutput);
        this->applyTemperatureFloatToOutput(this->componentState->airTemperature, this->lowerDisplayOutput);

        this->upperDisplay.setOutput(this->upperDisplayOutput);
        this->lowerDisplay.setOutput(this->lowerDisplayOutput);
        break;
    case DISPLAY_CONTROLLER_MODE_PWM:
        this->applyIntegerToOutput(this->componentState->lastPumpPWMPercentage, this->upperDisplayOutput);
        this->applyIntegerToOutput(this->componentState->lastFanPWMPercentage, this->lowerDisplayOutput);

        this->upperDisplay.setOutput(this->upperDisplayOutput);
        this->lowerDisplay.setOutput(this->lowerDisplayOutput);
        break;
    case DISPLAY_CONTROLLER_MODE_RPM_AND_DELTA_T:
        this->applyIntegerToOutput(this->componentState->lastPumpRpm, this->upperDisplayOutput);
        this->applyTemperatureFloatToOutput(this->componentState->delta_T, this->lowerDisplayOutput);

        this->upperDisplay.setOutput(this->upperDisplayOutput);
        this->lowerDisplay.setOutput(this->lowerDisplayOutput);
        break;
    default:
        this->upperDisplay.setSymbol(0, SEGMENT_DISPLAY_SYMBOL_D);
        this->upperDisplay.setSymbol(1, SEGMENT_DISPLAY_SYMBOL_I);
        this->upperDisplay.setSymbol(2, SEGMENT_DISPLAY_SYMBOL_S);
        this->upperDisplay.setSymbol(3, SEGMENT_DISPLAY_SYMBOL_P);

        this->lowerDisplay.setSymbol(0, 0b00000000);
        this->lowerDisplay.setSymbol(1, SEGMENT_DISPLAY_SYMBOL_E);
        this->lowerDisplay.setSymbol(2, SEGMENT_DISPLAY_SYMBOL_r);
        this->lowerDisplay.setSymbol(3, SEGMENT_DISPLAY_SYMBOL_r);
        break;
    }

    this->lowerDisplay.show();
    this->upperDisplay.show();
}