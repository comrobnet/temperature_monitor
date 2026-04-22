#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include <stdbool.h>
#include "eeprom.h"

// Mock ADC initialization
void TemperatureSensorInit(void);

// Mock ADC temperature value depending on the hardware revision of the sensor (Rev-A / Rev-B)
bool TemperatureSensorAdc(enum Revision r, uint16_t* adc_value);

// Convert ADC temperature values to values in degrees Celsius
// Rev-A has a resolution of 1 degree Celsius per digit. For example, a value of 10 corresponds to 10 °C
// Rev-B has a resolution of 0.1 degrees Celsius per digit. For example, a value of 100 corresponds to 10 °C
// TODO: Deprecated - used raw ADC values instead
bool TemperatureSensorCelsius(enum Revision r, float* temperature);

#endif // TEMPERATURE_SENSOR_H