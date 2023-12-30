#include "Gameboy.h"

void Gameboy::Run(const std::string& rom_path)
{
    // TODO: Move to init 
    // TODO: Add Shutdown memory free
    _cartridge = new Cartridge();
    _cartridge->LoadCartridge(rom_path);

    _bus = new Bus();
    _bus->Init(this);

    _cpu = new Cpu();
    _cpu->Init(this);

    _ram = new Ram();
    //_wram->Init(this);

    _running = true;
    while(_running)
    {
        _cpu->Step();
    }
}

void Gameboy::Shutdown()
{
    delete _cpu;
    delete _bus;
    delete _ram;
}

bool Gameboy::LoadCartridge(const std::string& rom_path)
{
    return false; 
}

void Gameboy::Cycle(int n)
{
    _ticks++;
}

void Gameboy::Delay(uint32_t ms)
{
    // TODO: Delay by ms
}