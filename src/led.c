#include <stddef.h>

#include "led.h"

static bool mock_gpio_led_state[kLedCount];

void LedInit(void)
{
    for (int i = 0; i < kLedCount; ++i) {
        mock_gpio_led_state[i] = false;
    }

    // TODO: Set up GPIOs based on hardware specifications
}

bool LedSet(enum Led led, bool on)
{
    if (led < 0 || led >= kLedCount) {
        return false;
    }

    mock_gpio_led_state[led] = on;
    return true;
}

bool LedGet(enum Led led, bool* on)
{
    if (on == NULL) {
        return false;
    }

    if (led < 0 || led >= kLedCount) {
        return false;
    }

    *on = mock_gpio_led_state[led];
    return true;
}