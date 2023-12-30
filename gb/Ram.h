#pragma once

#include "Common.h"
#include <array>

class Ram
{
public:
    uint8_t WRead(uint16_t address);
    void WWrite(uint16_t address, uint8_t value);

    uint8_t HRead(uint16_t address);
    void HWrite(uint16_t address, uint8_t value);
private:
    std::array<uint8_t, 0x2000> _ram;
    std::array<uint8_t, 0x80> _hram;
};