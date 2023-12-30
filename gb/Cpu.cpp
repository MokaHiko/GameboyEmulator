#include "Cpu.h"
#include "Gameboy.h"

#include <stdio.h>

// Swaps the lo and hi bit of n
uint16_t Reverse(uint16_t n)
{
    return ((n & 0xFF00) >> 8) | ((n & 0x00FF) << 8);
}

void Cpu::Init(Gameboy *gb)
{
    _gb = gb;

    _lookup.resize(static_cast<size_t>(InstructionType::IN_MAX));

    _lookup[IN_NONE] = &Cpu::PROC_NON;
    _lookup[IN_NOP] = &Cpu::PROC_NOP;
    _lookup[IN_DI] = &Cpu::PROC_DI;
    _lookup[IN_LD] = &Cpu::PROC_LD;
    _lookup[IN_LDH] = &Cpu::PROC_LDH;
    _lookup[IN_JP] = &Cpu::PROC_JP;
    _lookup[IN_JR] = &Cpu::PROC_JR;
    _lookup[IN_XOR] = &Cpu::PROC_XOR;

    // Reset
    _pc = 0x100;
    _a = 0x01;
}

const Instruction &Cpu::CurrentInstruction()
{
    return *_current_instruction;
}

uint16_t Cpu::ReadRegister(RegisterType type)
{
    switch (type)
    {
    case RT_A:
        return _a;
    case RT_F:
        return _f;
    case RT_B:
        return _b;
    case RT_C:
        return _c;
    case RT_D:
        return _d;
    case RT_E:
        return _e;
    case RT_H:
        return _h;
    case RT_L:
        return _l;

    case RT_AF:
        return Reverse(_a) | _f;
    case RT_BC:
        return Reverse(_b) | _c;
    case RT_DE:
        return Reverse(_d) | _e;
    case RT_HL:
        return Reverse(_h) | _l;

    case RT_PC:
        return _pc;
    case RT_SP:
        return _sp;

    default:
        return 0;
    }
}

void Cpu::SetRegister(RegisterType type, uint16_t value)
{
    switch(type)
    {
        case RT_A: _a = value & 0xFF; break;
        case RT_F: _f = value & 0xFF; break;
        case RT_B: _b = value & 0xFF; break;
        case RT_C: _c = value & 0xFF; break;
        case RT_D: _d = value & 0xFF; break;
        case RT_E: _e = value & 0xFF; break;
        case RT_H: _h = value & 0xFF; break;
        case RT_L: _l = value & 0xFF; break;

        // TODO: Figure out why reverse
        case RT_AF: *(uint16_t*)(&_a) = Reverse(value); break;
        case RT_BC: *(uint16_t*)(&_b) = Reverse(value); break;
        case RT_DE: *(uint16_t*)(&_d) = Reverse(value); break;
        case RT_HL: *(uint16_t*)(&_h) = Reverse(value); break;

        case RT_SP: _sp = value; break;
        case RT_PC: _pc = value; break;

        case RT_NONE: printf("\t-Uknown register type.\n"); break;
    };
}

void Cpu::SetFlags(char z, char n, char h, char c)
{
    if (z != -1)
    {
        BIT_SET(_f, 7, z);
    }

    if (n != -1)
    {
        BIT_SET(_f, 6, n);
    }

    if (h != -1)
    {
        BIT_SET(_f, 5, h);
    }

    if (c != -1)
    {
        BIT_SET(_f, 4, c);
    }
}

void Cpu::FetchInstruction()
{
    _current_opcode = _gb->GetBus().Read(_pc++);
    _current_instruction = InstructionByOpcode(_current_opcode);

    if (!_current_instruction)
    {
        printf("[ERROR]: Uknown opcode: %02X\n", _current_opcode);
        printf("[ERROR]: Exiting...\n");
        exit(-5);
    }
}

void Cpu::FetchData()
{
    _memory_dest = 0;
    _dest_is_memory = false;

    switch (_current_instruction->address_mode)
    {
    case AM_IMP:
    {
    }
    break;

    // Register Addressing Mode 
    case AM_R:
    {
        _fetched_data = ReadRegister(_current_instruction->reg1_type);
    }
    break;

    // 2nd Register Addressing Mode 
    case AM_R_R:
    {
        _fetched_data = ReadRegister(_current_instruction->reg2_type);
    }
    break;

    // Immediate Mode Byte
    case AM_R_D8:
    {
        _fetched_data = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);
    }
    break;

    // Immediate Mode (Extended) Byte
    case AM_D16:
    case AM_R_D16:
    {
        uint16_t lo = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);

        uint16_t hi = _gb->GetBus().Read(_pc++) << 8;
        _gb->Cycle(1);

        _fetched_data = hi | lo;
    }
    break;

    // Register Indirect Addressing Mode (Loading register to memory region)
    case AM_MR_R:
    {
        _fetched_data = ReadRegister(_current_instruction->reg2_type);
        _memory_dest = ReadRegister(_current_instruction->reg1_type);
        _dest_is_memory = true;

        // Only write to most significant byte on c register
        if(_current_instruction->reg1_type == RT_C)
        {
            _memory_dest |= 0xFF00;
        }
    }
    break;

    case AM_R_MR:
    {
        uint16_t addr = ReadRegister(_current_instruction->reg2_type);

        // Only write to most significant byte on c register
        if(_current_instruction->reg1_type == RT_C)
        {
            _memory_dest |= 0xFF00;
        }

        _fetched_data = _gb->GetBus().Read(addr);
        _gb->Cycle(1);
    }
    break;

    case AM_R_HLD:
    {
        _fetched_data = _gb->GetBus().Read(ReadRegister(_current_instruction->reg2_type));
        _gb->Cycle(1);

        SetRegister(RegisterType::RT_HL, ReadRegister(RT_HL) - 1);
    }
    break;

    case AM_HLI_R:
    {
        _fetched_data = ReadRegister(_current_instruction->reg2_type);
        _memory_dest =  ReadRegister(_current_instruction->reg1_type);
        _dest_is_memory = true;
        SetRegister(RegisterType::RT_HL, ReadRegister(RT_HL) + 1);
    }
    break;

    case AM_HLD_R:
    {
        _fetched_data = ReadRegister(_current_instruction->reg2_type);
        _memory_dest =  ReadRegister(_current_instruction->reg1_type);
        _dest_is_memory = true;
        SetRegister(RegisterType::RT_HL, ReadRegister(RT_HL) - 1);
    }
    break;

    case AM_R_A8:
    {
        _fetched_data = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1); 
    }break;

    case AM_A8_R:
    {
        _memory_dest = _gb->GetBus().Read(_pc++) | 0xFF00;
        _dest_is_memory = true;
        _gb->Cycle(1);
    }break;

    case AM_HL_SPR:
    {
        _fetched_data = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);
    }break;

    case AM_D8:
    {
        _fetched_data = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);
    }break;

    case AM_A16_R:
    case AM_D16_R:
    {
        uint16_t lo = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);

        uint16_t hi = _gb->GetBus().Read(_pc++) << 8;
        _gb->Cycle(1);

        _memory_dest = hi | lo;
        _dest_is_memory = true;

        _fetched_data = ReadRegister(_current_instruction->reg2_type);
    }break;

    case AM_MR_D8:
    {
        _fetched_data = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);

        _memory_dest = ReadRegister(_current_instruction->reg1_type);
        _memory_dest = true;
    } break;

    case AM_MR:
    {
        _memory_dest = ReadRegister(_current_instruction->reg1_type);
        _memory_dest = true;

        _fetched_data = _gb->GetBus().Read(_memory_dest);
        _gb->Cycle(1);
    } break;


    case AM_R_A16 : 
    {
        uint16_t lo = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);

        uint16_t hi = _gb->GetBus().Read(_pc++) << 8;
        _gb->Cycle(1);

        uint16_t adrr = hi | lo;    
        _fetched_data = _gb->GetBus().Read(adrr);
    }break;


    default:
    {
        printf("\t- Uknown addressing mode %d!\n", _current_instruction->address_mode);
        exit(-7);
    }
    break;
    }
}

void Cpu::Execute()
{
    if (!_lookup[_current_instruction->type])
    {
        printf("\t- Unimplemented %3s %2X\n", Instruction::StringLookUp[_current_instruction->type], _current_instruction->type);
        return;
    }

    // C style bind to this
    (this->*(_lookup[_current_instruction->type]))();
}

bool Cpu::CheckCondition() const
{
    bool z = Z();
    bool c = C();
    switch (_current_instruction->condition_type)
    {
    case CT_NONE:
        return true;
    case CT_C:
        return c;
    case CT_Z:
        return z;
    case CT_NC:
        return !c;
    case CT_NZ:
        return !z;

    default:
        return false;
    }
}

void Cpu::PROC_NON()
{
    printf("Invalid Instruction!\n");
}

void Cpu::PROC_NOP()
{
}

void Cpu::PROC_LD()
{
    if(_dest_is_memory)
    {
        if(_current_instruction->reg2_type >= RT_AF)
        {
            _gb->GetBus().Write16(_memory_dest, _fetched_data);
            _gb->Cycle(1);
        }
        else
        {
            _gb->GetBus().Write(_memory_dest, _fetched_data);
        }
    }

    // TODO: Special instruction Case? 
    if(_current_instruction->address_mode == AM_HL_SPR)
    {
        uint8_t h_flag = (ReadRegister(_current_instruction->reg2_type)) & 0XF + 
                    _fetched_data & 0XF >= 0x10;
        uint8_t c_flag = (ReadRegister(_current_instruction->reg1_type)) & 0XFF + 
                    _fetched_data & 0XFF >= 0x100;

        SetFlags(0, 0, h_flag, c_flag);
        SetRegister(_current_instruction->reg1_type, ReadRegister(_current_instruction->reg2_type) + (char)_fetched_data);
    }

    SetRegister(_current_instruction->reg1_type, _fetched_data);
}

void Cpu::PROC_DI()
{
    _int_master_enabled = false;
}

void Cpu::PROC_JP()
{
    if (CheckCondition())
    {
        _pc = _fetched_data;
        _gb->Cycle(1);
    }
}

void Cpu::PROC_JR()
{
    // TODO: ..
}

void Cpu::PROC_XOR()
{
    _a ^= static_cast<uint8_t>(_fetched_data & 0xFF);
    SetFlags(_a == 0, -1, -1, -1);
}

void Cpu::PROC_LDH()
{
    // High ram read/write
    if(_current_instruction->reg1_type == RegisterType::RT_A)
    {
        SetRegister(RegisterType::RT_A, _gb->GetBus().Read(_fetched_data) | 0xFF00);
    }
    else if(_current_instruction->reg2_type == RegisterType::RT_A)
    {
        _gb->GetBus().Write(_fetched_data | 0xFF00, ReadRegister(RegisterType::RT_A));
    }
}

bool Cpu::Step()
{
    uint16_t pc = _pc;

    FetchInstruction();
    FetchData();

    printf("PC: %04X SP: %04X| %3s (%02X %02X %02X) | AF: %02X%02X BC: %02X%02X DE: %02X%02X HL: %02X%02X\n",
           pc,
           _sp,

           Instruction::StringLookUp[_current_instruction->type],
           _current_opcode,
           _gb->GetBus().Read(pc + 1),
           _gb->GetBus().Read(pc + 2),

           _a,_f,
           _b,_c,
           _d,_e,
           _h,_l
            );

    Execute();

    return false;
}