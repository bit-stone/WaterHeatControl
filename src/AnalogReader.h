#include <Arduino.h>
#include <avr/interrupt.h>
#include <avr/io.h>

// #define REF_R1 10.0f
// #define REF_R2 10.0f
// #define REF_FACTOR (REF_R2 / (REF_R1 + REF_R2))

#define VOLTAGE_FACTOR 5.0f

#define MAX_CHANNEL 1

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
