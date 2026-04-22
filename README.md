# Temperature Monitor

This project implements a simple temperature monitor in C with mocked hardware interfaces, intended to run on a PC for demonstration purposes.

## Overview

The software simulates an embedded temperature monitoring application with the following mocked peripherals:

- **EEPROM**: stores hardware revision and serial number
- **Temperature sensor via ADC**: provides a mock temperature reading
- **GPIO-controlled LEDs**: indicate the current temperature range

The application reads the hardware revision from EEPROM, interprets the ADC value accordingly, and updates the LED states.

## Hardware Model

Two hardware revisions are supported:

- **Rev-A**: ADC resolution is **1.0 °C per digit**
- **Rev-B**: ADC resolution is **0.1 °C per digit**

The mocked EEPROM contains:

- revision: `0` or `1`
- serial number: `"ABC1234"`

## LED Behavior

The LEDs indicate the temperature range:

- **Green**: operation temperature (5°C - 85°C)
- **Yellow**: warning temperature (85°C - 105°C)
- **Red**: critical temperature (<5°C or >105°C)

## Project Structure

- `src/main.c` - main control loop
- `src/eeprom.c` - mocked EEPROM interface
- `src/temperature_sensor.c` - mocked ADC temperature sensor
- `src/led.c` - mocked GPIO LED control
- `include/` - public header files

## Build

Example with CMake:

```sh
cmake -S . -B build
cmake --build build