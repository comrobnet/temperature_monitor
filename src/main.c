#include <stdio.h>
#include <time.h>

#include "eeprom.h"
#include "led.h"
#include "temperature_sensor.h"

#define LOOP_PERIOD_US 100L
#define NS_PER_US      1000L
#define NS_PER_SEC     1000000000L

// Calculate difference in start and end time in nanoseconds
static int64_t timespec_diff_ns(const struct timespec* start,
                                const struct timespec* end)
{
    return ((end->tv_sec - start->tv_sec) * NS_PER_SEC) +
           (end->tv_nsec - start->tv_nsec);
}

int main(void)
{
    // Setup
    bool previous_led_state[kLedCount] = {false, false, false};
    const int64_t loop_period_ns = LOOP_PERIOD_US * NS_PER_US;

    //
    // EEPROM
    //

    DeviceInfo dev;

    if (!ReadDeviceInfo(&dev)) {
        fprintf(stderr, "Failed to read EEPROM\n");
        return 1;
    }

    printf("Revision: %u\n", dev.revision);
    printf("Serial:   %s\n", dev.serial);


    while (true)
    {
        struct timespec t_start;
        struct timespec t_end;

        clock_gettime(CLOCK_MONOTONIC, &t_start);
        //
        // Temperature
        //

        float temperature = 0;
        if (!TemperatureSensorCelsius(dev.revision, &temperature))
        {
            fprintf(stderr, "Failed to read temperature\n");
            return 1;
        }

        ///
        /// LEDs
        ///

        // set leds based on temperature

        bool current_led_state[kLedCount] = {false, false, false};

        // nominal temperature
        if (temperature >= 5 && temperature < 85)
        {
            current_led_state[kLedG] = true;
        }
        else if (temperature >= 85 && temperature < 105)
        {
            current_led_state[kLedY] = true;
        }
        else
        {
            current_led_state[kLedR] = true;
        }

        // Track if LEDs changed for debugging
        bool changed = false;

        // switch LEDs if state changes
        for (int led = 0; led < kLedCount; ++led) {
            if (previous_led_state[led] != current_led_state[led]) {
                if (!LedSet((enum Led)led, current_led_state[led]))
                {
                    fprintf(stderr, "Failed to set LED\n");
                };
                previous_led_state[led] = current_led_state[led];
                changed = true;
            }
        }

        // If the status changed, print the current temperature and LED state
        if (changed)
        {
            printf("%f", temperature);
            printf("\n");

            printf("LED state: G=%s Y=%s R=%s\n",
                current_led_state[kLedG] ? "ON" : "OFF",
                current_led_state[kLedY] ? "ON" : "OFF",
                current_led_state[kLedR] ? "ON" : "OFF");
        }

        ///
        /// Loop timing
        ///

        clock_gettime(CLOCK_MONOTONIC, &t_end);

        const int64_t elapsed_ns = timespec_diff_ns(&t_start, &t_end);
        const int64_t remaining_ns = loop_period_ns - elapsed_ns;

        if (remaining_ns > 0) {
            struct timespec sleep_time;
            sleep_time.tv_sec = remaining_ns / NS_PER_SEC;
            sleep_time.tv_nsec = remaining_ns % NS_PER_SEC;
            nanosleep(&sleep_time, NULL);
        } else {
            // loop overran 100 us, so do not sleep
            // printf("Loop overtime\n");
        }
    }

    return 0;
}