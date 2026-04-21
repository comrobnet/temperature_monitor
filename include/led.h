// Header file defining all relevant functions for the mocked LEDs

#ifndef LED_H
#define LED_H

#include <stdbool.h>

enum Led {
    kLedG = 0,
    kLedY,
    kLedR,
    kLedCount
};

void LedInit(void);
bool LedSet(enum Led led, bool on);
bool LedGet(enum Led led, bool* on);

#endif // LED_H