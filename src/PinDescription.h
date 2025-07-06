#include <avr/io.h>

#ifndef _BITSTONE_PIN_DESCRIPTION
#define _BITSTONE_PIN_DESCRIPTION

struct PinDescription
{
    volatile uint8_t *portOutSet;
    volatile uint8_t *portOutClr;
    volatile uint8_t *portDirSet;
    volatile uint8_t *portDirClr;
    volatile uint8_t *pinControl;
    volatile uint8_t *portIn;
    volatile uint8_t pinIndex;
};

#endif