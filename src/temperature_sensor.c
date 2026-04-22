#include <stddef.h>

#include "temperature_sensor.h"

void TemperatureSensorInit(void)
{
    // TODO: Initialize ADC based on hardware specifications
}

// Mock ADC temperature value depending on the hardware revision of the sensor (Rev-A / Rev-B)
bool TemperatureSensorAdc(enum Revision r, uint16_t* adc_value) {
    // TODO: Depending on sensor change data type to uint8/16/32 etc
    // TODO: In real sensor, consider ADC offset/scaling

    if (adc_value == NULL) {
        return false;
    }

    switch (r) {
    case kRevA:
        *adc_value = 10;   // 10 digits = 10.0 °C
        return true;

    case kRevB:
        *adc_value = 100;  // 100 digits = 10.0 °C
        return true;

    default:
        return false;
    }
}


// Convert ADC temperature values to values in degrees Celsius
// Rev-A has a resolution of 1 degree Celsius per digit. For example, a value of 10 corresponds to 10 °C
// Rev-B has a resolution of 0.1 degrees Celsius per digit. For example, a value of 100 corresponds to 10 °C
bool TemperatureSensorCelsius(enum Revision r, float* temperature) {
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