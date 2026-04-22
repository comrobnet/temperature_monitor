#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>

#define EEPROM_SIZE        256
#define EEPROM_REV_ADDR    0x00
#define EEPROM_SERIAL_ADDR 0x01
#define EEPROM_SERIAL_LEN  7

// Define hardware revisions for the temperature sensor (Rev-A / Rev-B)
enum Revision {
    kRevA = 0,
    kRevB = 1
};

// Define struct that is returned when the eeprom is read, including revision and serial numbers
typedef struct {
    enum Revision revision;
    char serial[EEPROM_SERIAL_LEN + 1];
} DeviceInfo;

// Copy the eeprom content at the desired address to the buffer
bool EepromRead(uint16_t addr, uint8_t *buf, size_t len);

// Read and store the eeprom info
bool ReadDeviceInfo(DeviceInfo *info);

#endif // EEPROM_H