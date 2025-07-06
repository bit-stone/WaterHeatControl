#include <avr/io.h>
#include "ComponentState.h"
#include "AnalogReader.h"

#ifndef _BITSTONE_TEMPERATURE_READER
#define _BITSTONE_TEMPERATURE_READER

struct ResistanceTemperatureValue
{
    uint8_t temperature; // Grad Celsius
    uint16_t resistance; // Ohm
};

class TemperatureReader
{
private:
    ComponentState *componentState;
    AnalogReader *analogReader;

    float referenceVolts = 0.0f;
    float waterTempVolts = 0.0f;
    float airTempVolts = 0.0f;

    uint16_t waterTempResistance = 0;
    uint16_t airTempResistance = 0;

    float waterTemperature = 0.0f;
    float airTemperature = 0.0f;
    float delta_T = 0.0f;

    // stuff used for interpolation of temp from resistance
    const ResistanceTemperatureValue resistanceValues[21] = {
        {.temperature = 0, .resistance = 27380},
        {.temperature = 5, .resistance = 22130},
        {.temperature = 10, .resistance = 17980},
        {.temperature = 15, .resistance = 14700},
        {.temperature = 20, .resistance = 12090},
        {.temperature = 25, .resistance = 10000},
        {.temperature = 30, .resistance = 8311},
        {.temperature = 35, .resistance = 6942},
        {.temperature = 40, .resistance = 5825},
        {.temperature = 45, .resistance = 4910},
        {.temperature = 50, .resistance = 4157},
        {.temperature = 55, .resistance = 3534},
        {.temperature = 60, .resistance = 3017},
        {.temperature = 65, .resistance = 2586},
        {.temperature = 70, .resistance = 2225},
        {.temperature = 75, .resistance = 1923},
        {.temperature = 80, .resistance = 1667},
        {.temperature = 85, .resistance = 1450},
        {.temperature = 90, .resistance = 1265},
        {.temperature = 95, .resistance = 1106},
        {.temperature = 100, .resistance = 971}};

    uint8_t arraySize = (sizeof(resistanceValues) / sizeof(resistanceValues[0]));
    uint8_t leftIndex = 0;
    uint8_t rightIndex = arraySize - 1;
    uint8_t middleIndex = 0;

    float result = 0.0f;

    float a = 0.0f;
    float b = 0.0f;
    float c = 0.0f;

    ResistanceTemperatureValue v0;
    ResistanceTemperatureValue v1;

    float interpolateTempFromResistance(uint16_t resistance);

public:
    TemperatureReader(AnalogReader *analogReader, ComponentState *componentStateInput);
    void update();
};

#endif