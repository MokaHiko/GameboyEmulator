#include <iostream>
#include "gb/Gameboy.h"

int main(int, char**){
    Gameboy* gb = new Gameboy();
    gb->Run("roms/cpu_instrs.gb");
    delete gb;
}
