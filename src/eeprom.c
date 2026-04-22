#include "eeprom.h"

#include <string.h>

// Mock hardware: fake EEPROM contents in memory
static const uint8_t kMockEeprom[EEPROM_SIZE] = {
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
bool EepromRead(uint16_t addr, uint8_t *buf, size_t len)
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
bool ReadDeviceInfo(DeviceInfo *info)
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