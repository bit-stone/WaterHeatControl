#include <Display.h>
#include <LedOut.h>

Display::Display()
{
}

void Display::setPortData(volatile uint8_t *dataOutSetRegister,
                          volatile uint8_t *dataOutClrRegister,
                          volatile uint8_t *dataDirSetRegister,
                          volatile uint8_t *dataDirClrRegister,
                          volatile uint8_t *dataPinCtrlRegister,
                          volatile uint8_t *dataPinInputRegister,
                          uint8_t dataClockPin,
                          uint8_t dataDataPin)
{
    portOutSet = dataOutSetRegister;
    portOutClr = dataOutClrRegister;
    portDirSet = dataDirSetRegister;
    portDirClr = dataDirClrRegister;
    dataPinCtrl = dataPinCtrlRegister;
    portInputReg = dataPinInputRegister;
    clockPin = dataClockPin;
    dataPin = dataDataPin;
}

void Display::initDisplay()
{
    set_all_led();
    // set pins as output, set output high by default
    *portDirSet = (1 << dataPin) | (1 << clockPin);
    *portOutSet = (1 << dataPin) | (1 << clockPin);

    displayDigits[0] = SYMBOL_ALL_SEGMENTS_ON;
    displayDigits[1] = SYMBOL_ALL_SEGMENTS_ON;
    displayDigits[2] = SYMBOL_ALL_SEGMENTS_ON;
    displayDigits[3] = SYMBOL_ALL_SEGMENTS_ON;

    Display::show();
}

void Display::clockDelay()
{
    _delay_us(CLOCK_DELAY_US);
}

void Display::setClockLow()
{
    *portOutClr = (1 << clockPin);
}

void Display::setClockHigh()
{
    *portOutSet = (1 << clockPin);
}

void Display::setDataLow()
{
    *portOutClr = (1 << dataPin);
}

void Display::setDataHigh()
{
    *portOutSet = (1 << dataPin);
}

void Display::updateNumber(float input)
{
    if (input < 0.0)
    {
        displayDigits[0] = SYMBOL_L;
        displayDigits[1] = SYMBOL_O;
        displayDigits[2] = SYMBOL_W_LEFT;
        displayDigits[3] = SYMBOL_W_RIGHT;
    }
    else if (input > 99.9)
    {
        displayDigits[0] = SYMBOL_H;
        displayDigits[1] = SYMBOL_I;
        displayDigits[2] = SYMBOL_G;
        displayDigits[3] = SYMBOL_H;
    }
    else
    {
        uint16_t output = (int)(input * 10);
        displayDigits[0] = digitToSymbol[(int)(output / 100)];
        displayDigits[1] = (digitToSymbol[(int)((output % 100) / 10)] | WITH_DECIMAL_POINT);
        displayDigits[2] = digitToSymbol[(int)((output % 10))];
        displayDigits[3] = SYMBOL_DEGREE;
    }

    Display::show();
}

void Display::startCondition()
{
    // set data pin as output again
    *portDirSet = (1 << dataPin);

    // start condition: clock/data high, data goes low
    Display::setClockHigh();
    Display::setDataHigh();

    Display::clockDelay();

    Display::setDataLow();

    Display::clockDelay();
}

void Display::stopCondition()
{
    Display::setClockLow();
    Display::setDataLow();

    Display::clockDelay();

    Display::setClockHigh();

    Display::clockDelay();

    Display::setDataHigh();

    Display::clockDelay();
}

void Display::sendByte(uint8_t data)
{
    // sending stuff: change on clock low. clock should be below 250 kHz
    for (uint8_t k = 0; k < 8; k++)
    {
        Display::setClockLow();
        Display::clockDelay();

        if (data & (1 << k))
        {
            Display::setDataHigh();
        }
        else
        {
            Display::setDataLow();
        }

        Display::clockDelay();
        Display::setClockHigh();

        Display::clockDelay();
    }

    // ACK: Data will be pulled low by target chip on falling edge of 8th clock signal
    // set as input to detect ack with pullup enabled
    Display::setClockLow();
    *portDirClr = (1 << dataPin);
    *dataPinCtrl |= PORT_PULLUPEN_bm;

    uint8_t count = 0;
    Display::clockDelay();

    while (count < 100 && !(*portInputReg && (1 << dataPin)))
    {
        Display::clockDelay();
        count++;
    }

    if (count >= 100)
    {
        // Serial.println("ERRACK");
        return;
    }

    // Data pin released after 9th clock
    Display::setClockHigh();
    Display::clockDelay();
    Display::setClockLow();

    // set data pin as output again
    *portDirSet |= (1 << dataPin);
}

void Display::show()
{
    for (uint8_t k = 0; k < 4; k++)
    {
        Display::sendDigit(k, displayDigits[k]);
    }
}

void Display::sendDigit(uint8_t address, uint8_t data)
{
    Display::startCondition();
    Display::sendByte(WRITE_DATA_COMMAND);
    Display::stopCondition();

    Display::clockDelay();

    Display::startCondition();
    Display::sendByte(ADDRESS_COMMAND_MASK | address);

    Display::sendByte(data);
    Display::stopCondition();

    Display::startCondition();
    Display::sendByte(DISPLAY_ON_COMMAND | DISPLAY_BRIGHTNESS_MASK | 0b00000000);
    Display::stopCondition();
}