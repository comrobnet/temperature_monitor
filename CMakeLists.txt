cmake_minimum_required(VERSION 3.30)
project(temperature_monitor C)

set(CMAKE_C_STANDARD 11)

add_executable(temperature_monitor
        src/main.c
        #src/eeprom.c
        #src/temperature_sensor.c
        src/led.c
)

target_include_directories(temperature_monitor PRIVATE include)
