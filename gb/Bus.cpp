#include "Bus.h"
#include "Gameboy.h"

// 0x0000 - 0x3FFF : ROM Bank 0
// 0x4000 - 0x7FFF : ROM Bank 1 - Switchable
// 0x8000 - 0x97FF : CHR RAM
// 0x9800 - 0x9BFF : BG Map 1
// 0x9C00 - 0x9FFF : BG Map 2
// 0xA000 - 0xBFFF : Cartridge RAM
// 0xC000 - 0xCFFF : RAM Bank 0
// 0xD000 - 0xDFFF : RAM Bank 1-7 - switchable - Color only
// 0xE000 - 0xFDFF : Reserved - Echo RAM
// 0xFE00 - 0xFE9F : Object Attribute Memory
// 0xFEA0 - 0xFEFF : Reserved - Unusable
// 0xFF00 - 0xFF7F : I/O Registers
// 0xFF80 - 0xFFFE : Zero Page

void Bus::Init(Gameboy* gb)
{
    _gb = gb;
}

uint8_t Bus::Read(uint16_t address)
{
    if(address < 0x8000)
    {
        return _gb->GetCartridge().Read(address);
    }
    else if(address < 0xA000)
    {
        // Char/Map data
        NO_IMPL;
    }
    else if(address < 0xC000)
    {
        // Cartridge Ram
        return _gb->GetCartridge().Read(address);
    }
    else if(address < 0xE000)
    {
        // WRAM (Working Ram)
        return _gb->GetRam().WRead(address);
    }
    else if(address < 0xFE00)
    {
        // Reserved echo ram unusable
        return 0;
    }
    else if(address < 0xFEA0)
    {
        // TODO: Add PPU memory access
        NO_IMPL;
    }
    else if(address < 0xFF00)
    {
        // Reserved unusable... 
        return 0;
    }
    else if(address < 0xFF80)
    {
        // TODO: Io registers
        NO_IMPL;
    }
    else if(address == 0xFFFF)
    {
        // Cpu enable interrupt register  
        _gb->GetCpu().GetIERegister();
    }

    // Return high ram read / zero page 
    return _gb->GetRam().HRead(address);
}

uint16_t Bus::Read16(uint16_t address)
{
    uint16_t lo = Read(address);
    int16_t hi = Read(address + 1) << 8;

    return hi | lo;
}

void Bus::Write(uint16_t address, uint8_t value)
{
    if(address < 0x8000)
    {
        return _gb->GetCartridge().Write(address, value);
    }
    else if(address < 0xA000)
    {
        // Char/Map data
        NO_IMPL;
    }
    else if(address < 0xC000)
    {
        // Cartridge Ram
        _gb->GetCartridge().Write(address, value);
    }
    else if(address < 0xE000)
    {
        // WRAM (Working Ram)
        _gb->GetRam().WWrite(address, value);
    }
    else if(address < 0xFE00)
    {
        // Reserved echo ram unusable
    }
    else if(address < 0xFEA0)
    {
        // TODO: Add PPU memory access
        NO_IMPL;
    }
    else if(address < 0xFF00)
    {
        // Reserved unusable... 
    }
    else if(address < 0xFF80)
    {
        // TODO: Io registers
        NO_IMPL;
    }
    else if(address == 0xFFFF)
    {
        //  Cpu enable interrupt register  
        _gb->GetCpu().SetIERegister(value);
    }
    else
    {
        // Return high ram  write/ zero page 
        _gb->GetRam().HWrite(address, value);
    }
}

void Bus::Write16(uint16_t address, uint16_t value)
{
    Write(address + 1, (value >> 8) & 0XFF);
    Write(address, value  & 0XFF);
}
