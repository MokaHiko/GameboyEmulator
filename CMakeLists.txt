cmake_minimum_required(VERSION 3.0.0)
project(GBEmu VERSION 0.1.0 LANGUAGES C CXX)


set(CMAKE_CXX_STANDARD 17)
include(CTest)
enable_testing()

add_executable(GBEmu main.cpp 
                     gb/Common.h
                     gb/Timer.h
                     gb/Timer.cpp 
                     gb/Bus.h
                     gb/Bus.cpp 
                     gb/Instruction.h 
                     gb/Instruction.cpp
                     gb/Cpu.h 
                     gb/Cpu.cpp 
                     gb/Ppu.h
                     gb/Ppu.cpp 
                     gb/Cartridge.h
                     gb/Cartridge.cpp 
                     gb/Gameboy.h
                     gb/Gameboy.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
