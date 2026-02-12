#include <TemperatureReader.h>

/**
 * This module reads the analog values for air/water volts as well as reference volts
 * and calculates water/air temperature from it as well as delta_T
 * 
 * errors
 * E02 - air temperature: voltage too low
 * E03 - water temperature: voltage too low
 * E04 - reference voltage too low
 */

TemperatureReader::TemperatureReader(AnalogReader *analogReader, ComponentState *componentState)
{
    this->componentState = componentState;
    this->analogReader = analogReader;
}

void TemperatureReader::update()
{
    this->airTempVolts = this->analogReader->getVoltage(2);
    this->waterTempVolts = this->analogReader->getVoltage(1);
    this->referenceVolts = this->analogReader->getVoltage(0);

    const float eps = 0.01f;
    if (this->airTempVolts < eps || this->waterTempVolts < eps)
    {
        // avoid division by zero, set to some default value
        this->waterTemperature = 1.0f;
        this->airTemperature = 1.0f;
    } else {
        waterTempResistance = ((referenceVolts - waterTempVolts) * 10000) / (waterTempVolts);
        airTempResistance = ((referenceVolts - airTempVolts) * 10000) / (airTempVolts);

        this->waterTemperature = interpolateTempFromResistance(waterTempResistance);
        this->airTemperature = interpolateTempFromResistance(airTempResistance);
    }

    this->delta_T = this->waterTemperature - this->airTemperature;

    // TODO add warnings and errors about delta_T

    this->componentState->waterTemperature = this->waterTemperature;
    this->componentState->airTemperature = this->airTemperature;
    this->componentState->delta_T = this->delta_T;
}

float TemperatureReader::interpolateTempFromResistance(uint16_t resistance)
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