#pragma once

#include <cstdint>
#include <string> 

#include "Cartridge.h"

// The Game Boy is an 8-bit fourth generation handheld game console developed and manufactured by Nintendo. It was first released in Japan on April 21, 1989, in North America later the same year, and in Europe in late 1990.
class Gameboy
{
public:
    void Run(const std::string& rom_path);
private:
    bool LoadCartridge(const std::string& rom_path);
    void Delay(uint32_t ms);
private:
    uint32_t _ticks;
    bool _running;

    Cartridge _cartridge;
};