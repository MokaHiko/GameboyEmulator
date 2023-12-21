cmake_minimum_required(VERSION 3.0.0)
project(GBEmu VERSION 0.1.0 LANGUAGES C CXX)


set(CXX_STANDARD 17)
include(CTest)
enable_testing()

add_executable(GBEmu main.cpp 
                     gb/Timer.cpp 
                     gb/Cpu.cpp 
                     gb/Ppu.cpp 
                     gb/Cartridge.cpp 
                     gb/Gameboy.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
