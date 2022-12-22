#include <Arduino.h>

struct ResistanceTemperatureValue {
    uint8_t temperature; // Grad Celsius
    uint16_t resistance; // Ohm
};

float interpolateTempFromResistance(uint16_t resistance);