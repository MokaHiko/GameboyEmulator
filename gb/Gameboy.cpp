#include "Gameboy.h"

void Gameboy::Run(const std::string& rom_path)
{
    _cartridge.LoadCartridge(rom_path);

    _running = true;
    while(_running)
    {
    }
}

bool Gameboy::LoadCartridge(const std::string& rom_path)
{
    return false; 
}

void Gameboy::Delay(uint32_t ms)
{
    // TODO: Delay by ms
}