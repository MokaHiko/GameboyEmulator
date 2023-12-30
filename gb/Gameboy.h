#pragma once

#include <string> 

#include "Common.h"
#include "Cpu.h"
#include "Cartridge.h"
#include "Bus.h"
#include "Ram.h"

// The Game Boy is an 8-bit fourth generation handheld game console developed and manufactured by Nintendo. It was first released in Japan on April 21, 1989, in North America later the same year, and in Europe in late 1990.
class Gameboy
{
public:
    void Run(const std::string& rom_path);
    void Shutdown();

    Bus& GetBus() {return *_bus;}
    Cartridge& GetCartridge() {return *_cartridge;}

    Cpu& GetCpu() {return *_cpu;}

    Ram& GetRam() {return *_ram;}

    // Emulate n cycles
    void Cycle(int n = 1);
private:
    bool LoadCartridge(const std::string& rom_path);
    void Delay(uint32_t ms);
private:
    Cartridge* _cartridge;
    Bus* _bus;
    Cpu* _cpu;
    Ram* _ram;

    uint32_t _ticks;
    bool _running;
};