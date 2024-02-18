#include "ResistanceTemperatureConverter.h"

ResistanceTemperatureValue resistanceValues[21] = {
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

float interpolateTempFromResistance(uint16_t resistance)
{
    if (resistance <= resistanceValues[arraySize - 1].resistance)
    {
        return 100.0f;
    }

    if (resistance >= resistanceValues[0].resistance)
    {
        return 0.0f;
    }

    result = 0.0f;

    // find two nearest indices for resistance
    leftIndex = 0;
    rightIndex = arraySize - 1;

    while (leftIndex < rightIndex - 1)
    {
        middleIndex = ((rightIndex - leftIndex) / 2) + leftIndex;
        if (resistance >= resistanceValues[middleIndex].resistance)
        {
            rightIndex = middleIndex;
        }
        else
        {
            leftIndex = middleIndex;
        }
    }

    // interpolate
    v0 = resistanceValues[leftIndex];
    v1 = resistanceValues[rightIndex];

    a = ((float)v0.temperature) * ((float)v1.resistance - (float)resistance);
    b = ((float)v1.temperature) * ((float)resistance - (float)v0.resistance);
    c = ((float)v1.resistance - (float)v0.resistance);

    result = (a + b) / c;

    // return result
    return result;
}