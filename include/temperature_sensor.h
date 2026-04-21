#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <stdbool.h>
#include "eeprom.h"

// Mock ADC temperature value depending on the hardware revision of the sensor (Rev-A / Rev-B)
static bool TemperatureSensorAdc(const enum Revision r, int* adc_value) {
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
static bool TemperatureSensorCelsius(enum Revision r, float* temperature) {
  int adc_value;

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

#endif // TEMPERATURE_SENSOR_H