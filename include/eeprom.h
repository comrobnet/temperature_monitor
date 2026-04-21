#ifndef EEPROM_H
#define EEPROM_H

#include <stdint.h>
#include <stdbool.h>
#include <string.h>

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

// Mock hardware: fake EEPROM contents in memory
const static uint8_t kMockEeprom[EEPROM_SIZE] = {
    [EEPROM_REV_ADDR] = 1,
    [0x01] = 'A',
    [0x02] = 'B',
    [0x03] = 'C',
    [0x04] = '1',
    [0x05] = '2',
    [0x06] = '3',
    [0x07] = '4',
};


// Copy the eeprom content at the desired address to the buffer
static bool EepromRead(const uint16_t addr, uint8_t *buf, const size_t len)
{
    if (buf == NULL) {
        return false;
    }

    if ((size_t)addr + len > EEPROM_SIZE) {
        return false;
    }

    memcpy(buf, &kMockEeprom[addr], len);
    return true;
}

// Read and store the eeprom info
static bool ReadDeviceInfo(DeviceInfo *info)
{
    uint8_t raw[1 + EEPROM_SERIAL_LEN];

    if (info == NULL) {
        return false;
    }

    if (!EepromRead(EEPROM_REV_ADDR, raw, sizeof(raw))) {
        return false;
    }

    if (raw[0] != kRevA && raw[0] != kRevB) {
        return false;
    }

    info->revision = (enum Revision)raw[0];
    memcpy(info->serial, &raw[1], EEPROM_SERIAL_LEN);
    info->serial[EEPROM_SERIAL_LEN] = '\0';

    return true;
}

#endif // EEPROM_H