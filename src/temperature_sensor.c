#include <stddef.h>

#include "temperature_sensor.h"

void TemperatureSensorInit(void)
{
    // TODO: Initialize ADC based on hardware specifications
}

bool TemperatureSensorAdc(enum Revision r, uint16_t* adc_value) {
    // TODO: Depending on sensor change data type to uint8/16/32 etc
    // TODO: In real sensor, consider ADC offset/scaling

    if (adc_value == NULL) {
        return false;
    }

    switch (r) {
    case kRevA:
        // TODO: Implement reading of real sensor via ADC readout
        // ADC value hardcoded to 10 for Rev-A
        *adc_value = 10;   // 10 digits = 10.0 °C
        return true;

    case kRevB:
        // TODO: Implement reading of real sensor via ADC readout
        // ADC value hardcoded to 100 for Rev-B
        *adc_value = 100;  // 100 digits = 10.0 °C
        return true;

    default:
        return false;
    }
}

bool TemperatureSensorCelsius(const enum Revision r, float* temperature) {
    uint16_t adc_value;

    if (temperature == NULL) {
        return false;
    }

    if (!TemperatureSensorAdc(r, &adc_value)) {
        return false;
    }

    switch (r) {
    case kRevA:
        *temperature = (float)adc_value;
        return true;

    case kRevB:
        *temperature = (float)adc_value * 0.1f;
        return true;

    default:
        return false;
    }
}