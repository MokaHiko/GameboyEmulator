#pragma once

#include "Common.h"
#include <string>
#include <vector>

struct CartridgeHeader
{
    uint8_t entry[0x4];
    uint8_t logo[0x30];

    uint8_t title[0x10];

    uint16_t new_lic_code;

    uint8_t sgb_flag;
    uint8_t cart_type;
    uint8_t rom_size;
    uint8_t ram_size;
    uint8_t dest_code;
    uint8_t lic_code;
    uint8_t version;
    uint8_t check_sum;

    uint16_t global_check_sum;
};

class Cartridge
{
public:
    bool LoadCartridge(const std::string& rom_path);
    std::string LicenseName() const;
    std::string CartridgeTypeName() const;

    uint8_t Read(uint16_t address) const;
    void Write(uint16_t address, uint8_t value);
private:
    std::string _file_name;
    uint32_t _rom_size;
    std::vector<uint8_t> _rom_data; 
    CartridgeHeader* _header;
};