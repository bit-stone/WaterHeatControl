#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

// #define REF_R1 10.0f
// #define REF_R2 10.0f
// #define REF_FACTOR (REF_R2 / (REF_R1 + REF_R2))

#define VOLTAGE_FACTOR 5.0f

// the number of adc channels that are being scanned. 
// Currently, the channels ares used for:

// Channel 0: Reference Voltage - measuring the supply voltage
// Channel 1: Water Temperature
// Channel 2: Input Air Temperature
// Channel 3: NOT USED
// Channel 4: NOT USED
// Channel 5: NOT USED
// Channel 6: NOT USED
// Channel 7: NOT USED
// Channel 8: NOT USED
// Channel 9: NOT USED
#define MAX_CHANNEL 2

class AnalogReader
{
private:
    uint16_t analogValues[16] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    uint8_t currentChannel = 0;
    uint8_t lastChannel = 0;
public:
    AnalogReader();
    void initReader();
    uint16_t getValue(uint8_t channel);
    void handleResult();
    float getVoltage(uint8_t channel);
};
