#include "Cpu.h"
#include "Gameboy.h"

#include <stdio.h>

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

    case AM_R:
    {
        _fetched_data = ReadRegister(_current_instruction->reg1_type);
    }
    break;

    case AM_R_D8:
    {
        _fetched_data = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);
    }
    break;

    case AM_D16:
    {
        uint16_t lo = _gb->GetBus().Read(_pc++);
        _gb->Cycle(1);

        uint16_t hi = _gb->GetBus().Read(_pc++) << 8;
        _gb->Cycle(1);

        _fetched_data = hi | lo;
    }
    break;

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
    // TODO:
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

bool Cpu::Step()
{
    uint16_t pc = _pc;

    FetchInstruction();
    FetchData();

    printf("PC: %04X | %3s (%02X %02X %02X) | A: %02X F: %02X B: %02X C: %02X D: %02X E: %02X\n",
           pc,

           Instruction::StringLookUp[_current_instruction->type],
           _current_opcode,
           _gb->GetBus().Read(pc + 1),
           _gb->GetBus().Read(pc + 2),

           _a,
           _f,
           _b,
           _c,
           _d,
           _e
            );

    Execute();

    return false;
}