#include "display/SegmentDisplay.h"
#include "SegmentDisplay.h"

SegmentDisplay::SegmentDisplay()
{
}

void SegmentDisplay::setPinData(PinDescription clockPin, PinDescription dataPin)
{
    this->clockPin = clockPin;
    this->dataPin = dataPin;
}

void SegmentDisplay::init()
{
    // set pins as output
    *(this->dataPin.portDirSet) |= (1 << this->dataPin.pinIndex);
    *(this->clockPin.portDirSet) |= (1 << this->clockPin.pinIndex);

    *(this->dataPin.portOutSet) |= (1 << this->dataPin.pinIndex);
    *(this->clockPin.portOutSet) |= (1 << this->clockPin.pinIndex);

    this->output[0] = SEGMENT_DISPLAY_SYMBOL_ALL_SEGMENTS_ON;
    this->output[1] = SEGMENT_DISPLAY_SYMBOL_ALL_SEGMENTS_ON;
    this->output[2] = SEGMENT_DISPLAY_SYMBOL_ALL_SEGMENTS_ON;
    this->output[3] = SEGMENT_DISPLAY_SYMBOL_ALL_SEGMENTS_ON;

    this->show();
}

void SegmentDisplay::setClockLow()
{
    *(this->clockPin.portOutClr) = (1 << this->clockPin.pinIndex);
}

void SegmentDisplay::setClockHigh()
{
    *(this->clockPin.portOutSet) = (1 << this->clockPin.pinIndex);
}

void SegmentDisplay::setDataLow()
{
    *(this->dataPin.portOutClr) = (1 << this->dataPin.pinIndex);
}

void SegmentDisplay::setDataHigh()
{
    *(this->dataPin.portOutSet) = (1 << this->dataPin.pinIndex);
}

void SegmentDisplay::clockDelay()
{
    _delay_us(SEGMENT_DISPLAY_CLOCK_DELAY_US);
}

void SegmentDisplay::startCondition()
{
    // set data pin as output again
    *(this->dataPin.portDirSet) |= (1 << this->dataPin.pinIndex);

    // start condition: clock/data high, data goes low
    this->setClockHigh();
    this->setDataHigh();

    this->clockDelay();

    this->setDataLow();

    this->clockDelay();
}

void SegmentDisplay::stopCondition()
{
    this->setClockLow();
    this->setDataLow();

    this->clockDelay();

    this->setClockHigh();

    this->clockDelay();

    this->setDataHigh();

    this->clockDelay();
}

void SegmentDisplay::sendByte(uint8_t data)
{
    // sending stuff: change on clock low. clock should be below 250 kHz
    for (uint8_t k = 0; k < 8; k++)
    {
        this->setClockLow();
        this->clockDelay();

        if (data & (1 << k))
        {
            this->setDataHigh();
        }
        else
        {
            this->setDataLow();
        }

        this->clockDelay();
        this->setClockHigh();

        this->clockDelay();
    }

    // ACK: Data will be pulled low by target chip on falling edge of 8th clock signal
    // set as input to detect ack with pullup enabled
    this->setClockLow();
    *(this->dataPin.portDirClr) = (1 << this->dataPin.pinIndex);
    *(this->dataPin.pinControl) |= PORT_PULLUPEN_bm;

    uint8_t count = 0;
    this->clockDelay();

    while (count < 100 && (*(this->dataPin.portIn) & (1 << (this->dataPin.pinIndex))))
    {
        this->clockDelay();
        count++;
    }

    if (count >= 100)
    {
        // TODO: Maybe set some display error in componentState?
        PORTE_OUTTGL = (1 << PIN2_bp);
        return;
    }

    // data pin released after 9th clock
    this->setClockHigh();
    this->clockDelay();
    this->setClockLow();

    // set data pin as output again
    *(this->dataPin.portDirSet) = (1 << this->dataPin.pinIndex);
}

void SegmentDisplay::show()
{
    this->startCondition();
    this->sendByte(SEGMENT_DISPLAY_WRITE_DATA_COMMAND);
    this->stopCondition();

    this->clockDelay();

    for (uint8_t k = 0; k < 4; k++)
    {
        this->startCondition();
        this->sendByte(SEGMENT_DISPLAY_ADDRESS_COMMAND_MASK | k);
        this->sendByte(this->output[k]);
        this->stopCondition();
    }

    this->startCondition();
    this->sendByte(SEGMENT_DISPLAY_ON_COMMAND | SEGMENT_DISPLAY_BRIGHTNESS_MASK | 0b00000001);
    this->stopCondition();
}

void SegmentDisplay::clear()
{
    this->output[0] = 0b0;
    this->output[1] = 0b0;
    this->output[2] = 0b0;
    this->output[3] = 0b0;
}

void SegmentDisplay::setSymbol(uint8_t idx, uint8_t symbol)
{
    if (idx >= 4)
    {
        return;
    }

    this->output[idx] = symbol;
}

void SegmentDisplay::setOutput(uint8_t *input) {
    for(uint8_t k = 0; k < 4; k++) {
        this->output[k] = input[k];
    }
}