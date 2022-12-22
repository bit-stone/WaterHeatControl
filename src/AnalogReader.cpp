#include <AnalogReader.h>

AnalogReader::AnalogReader()
{
}

void AnalogReader::initReader()
{
    // configure pins
    PORTD_DIR &= ~((1 << PIN0_bp) | (1 << PIN1_bp));

    // configure ADC
    VREF.CTRLA |= (VREF_ADC0REFSEL_2V5_gc); // set 2.5v internal ref

    ADC0.CTRLA &= ~(1 << ADC_RESSEL_bp); // set 10 bit mode
    ADC0.CTRLB = ADC_SAMPNUM_ACC8_gc; // 8 samples per conversion
    ADC0.CTRLC = ADC_PRESC_DIV256_gc; // prescaler 32

    ADC0.MUXPOS = ADC_MUXPOS_AIN0_gc; // select AIN0 as source
    ADC0.CTRLA |= (1 << ADC_ENABLE_bp); // enable ADC0
    ADC0.INTCTRL |= (1 << ADC_RESRDY_bp); // enable result ready interrupt

    ADC0.COMMAND |= (1 << ADC_STCONV_bp); // start first conversion
}

uint16_t AnalogReader::getValue(uint8_t channel)
{
    return this->analogValues[channel];
}

float AnalogReader::getVoltage(uint8_t channel) {
    float result = 0.0f;
    result = this->analogValues[channel];
    result = result / 1023.0;
    result *= VOLTAGE_FACTOR;
    return result;
}

void AnalogReader::handleResult()
{
    // set new analog channel here because reading the value will trigger a 
    // new conversion (I think)
    this->lastChannel = (this->currentChannel);

    // increment current channel for next handle
    this->currentChannel++;
    if (this->currentChannel > MAX_CHANNEL)
    {
        this->currentChannel = 0;
    }

    ADC0.MUXPOS = (this->currentChannel);
    ADC0.COMMAND |= (1 << ADC_STCONV_bp);
    
    // reading result, divided by the number of accumulated values
    this->analogValues[this->lastChannel] = ADC0_RES / 8;
}