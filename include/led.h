#ifndef LED_H
#define LED_H

#include <stdbool.h>

enum Led {
    kLedG = 0,
    kLedY,
    kLedR,
    kLedCount
};

// Initialized three mocked LEDs (green, yellow, red)
void LedInit(void);

// Set the state of a LED to on (true) or off (false)
bool LedSet(enum Led led, bool on);

// Get the state of an LED
bool LedGet(enum Led led, bool* on);

#endif // LED_H