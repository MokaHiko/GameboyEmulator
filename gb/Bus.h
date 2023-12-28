#pragma once

 #include "Common.h"

class Gameboy;
class Bus
{
public:
    void Init(Gameboy* gb);

    uint8_t Read(uint16_t address);
    uint16_t Read16(uint16_t address);

    void Write(uint16_t address, uint8_t value);
    void Write16(uint16_t address, uint16_t value);
private:
    Gameboy* _gb;
};