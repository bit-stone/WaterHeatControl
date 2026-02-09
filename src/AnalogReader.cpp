#include <AnalogReader.h>

AnalogReader::AnalogReader()
{
}

void AnalogReader::initReader()
{
    // configure pins: set PIN0, PIN1 and PIN2 as inputs
    PORTD_DIRCLR = (1 << PIN0_bp) | (1 << PIN1_bp) | (1 << PIN2_bp);

    // set internal voltage reference to 2.5V
    VREF.CTRLA = 0b00000000;
    VREF.CTRLA |= VREF_ADC0REFSEL_2V5_gc;

    ADC0.CTRLC = 0b00000000;
    ADC0.CTRLC |= ADC_PRESC_DIV256_gc; // prescaler 256
    // Update: Using internal bandgap since input voltage is not known (between 4.5 and 5.0 V)
    // not setting REFSEL sets to internal reference set above.
    // ADC0.CTRLC |= ADC_REFSEL_VREFA_gc; // Use VRef as reference

    ADC0.CTRLB = 0b00000000;
    ADC0.CTRLB = ADC_SAMPNUM_ACC8_gc; // 8 samples per conversion

    ADC0.CTRLA = 0b00000000;
    ADC0.CTRLA &= ~(1 << ADC_RESSEL_bp); // set 10 bit mode
    ADC0.CTRLA |= (1 << ADC_ENABLE_bp);  // enable ADC0

    ADC0.MUXPOS = ADC_MUXPOS_AIN0_gc;     // select AIN0 as source
    ADC0.INTCTRL |= (1 << ADC_RESRDY_bp); // enable result ready interrupt

    ADC0.COMMAND |= (1 << ADC_STCONV_bp); // start first conversion
}

uint16_t AnalogReader::getValue(uint8_t channel)
{
    return this->analogValues[channel];
}

float AnalogReader::getVoltage(uint8_t channel)
{
    float result = 0.0f;
    result = this->analogValues[channel];
    result = result / 1023.0;
    result *= ANALOG_READER_VOLTAGE_FACTOR;
    return result;
}

void AnalogReader::handleResult()
{
    // set new analog channel here because reading the value will trigger a
    // new conversion (I think)
    this->lastChannel = (this->currentChannel);

    // increment current channel for next handle
    this->currentChannel++;
    if (this->currentChannel > ANALOG_READER_MAX_CHANNEL)
    {
        this->currentChannel = 0;
    }

    // reading result, divided by the number of accumulated values
    this->analogValues[this->lastChannel] = ADC0_RES / 8;

    // setting up next measurement
    ADC0.MUXPOS = (this->currentChannel);
    ADC0.COMMAND |= (1 << ADC_STCONV_bp);
}