#include <stdint.h>
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

typedef struct {
    uint16_t low;
    uint16_t high;
    uint16_t critical;
} TemperatureThresholds;

int main(void)
{
    //
    // Setup
    //
    const int64_t loop_period_ns = LOOP_PERIOD_US * NS_PER_US;

    static const TemperatureThresholds kThresholdsByRevision[] = {
        [kRevA] = { .low = 5,  .high = 85,  .critical = 105 },
        [kRevB] = { .low = 50, .high = 850, .critical = 1050 }
    };

    bool previous_led_state[kLedCount] = {false, false, false};

    // setup hardware
    // TODO: Setup microcontroller clock
    LedInit();
    TemperatureSensorInit();

    // EEPROM
    DeviceInfo dev;

    if (!ReadDeviceInfo(&dev)) {
        fprintf(stderr, "Failed to read EEPROM\n");
        return 1;
    }

    printf("Revision: %u\n", dev.revision);
    printf("Serial:   %s\n", dev.serial);

    const TemperatureThresholds thresholds = kThresholdsByRevision[dev.revision];

    //
    // Main loop
    //
    while (true)
    {
        struct timespec t_start;
        struct timespec t_end;

        // TODO: This is an OS service for PC mock; on an embedded system use hardware timer with interrupt
        clock_gettime(CLOCK_MONOTONIC, &t_start);

        //
        // Temperature
        //

        uint16_t temperature = 0;
        if (!TemperatureSensorAdc(dev.revision, &temperature))
        {
            fprintf(stderr, "Failed to read temperature\n");
            return 1;
        }
        // TODO: Depending on ADC signal characteristics, using the raw ADC value could lead to flickering LEDs
        // If this is an issue, consider options such as low pass filtering, a dead band, etc.

        ///
        /// LEDs
        ///

        // set leds based on temperature

        bool current_led_state[kLedCount] = {false, false, false};

        // nominal temperature
        if (temperature >= thresholds.low && temperature < thresholds.high)
        {
            current_led_state[kLedG] = true;
        }
        else if (temperature >= thresholds.high && temperature < thresholds.critical)
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
                    return 1;
                };
                previous_led_state[led] = current_led_state[led];
                changed = true;
            }
        }

        // If the status changed, print the current temperature and LED state
        if (changed)
        {
            printf("Temperature ADC value: ");
            printf("%u", temperature);
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
            // TODO: This is an OS service for PC mock; on an embedded system use hardware timer with interrupt
            nanosleep(&sleep_time, NULL);
        } else {
            // loop overran 100 us, so do not sleep
            // printf("Loop overtime\n");
        }
    }

    return 0;
}