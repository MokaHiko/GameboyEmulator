#include "Ram.h"

uint8_t Ram::WRead(uint16_t address)
{
    uint16_t rel = address - 0xC000;
    assert(rel >= 0x2000 && "Invalid working ram address!");

    return _ram[rel];
}

void Ram::WWrite(uint16_t address, uint8_t value)
{
    uint16_t rel = address - 0xC000;
    assert(rel >= 0x2000 && "Invalid working ram address!");

    _ram[rel] = value;
}

uint8_t Ram::HRead(uint16_t address)
{
    uint16_t rel = address - 0xFF80;
    assert(rel >= 0x80 && "Invalid high ram address!");

    return _hram[rel];
}

void Ram::HWrite(uint16_t address, uint8_t value)
{
    uint16_t rel = address - 0xFF80;
    assert(rel >= 0x80 && "Invalid high ram address!");

    _hram[rel] = value;
}
