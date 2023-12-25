#include "Instruction.h"
#include "Common.h"

const char* Instruction::StringLookUp[] = {
        "<NONE>",
        "NOP",
        "LD",
        "INC",
        "DEC",
        "RLCA",
        "ADD",
        "RRCA",
        "STOP",
        "RLA",
        "JR",
        "RRA",
        "DAA",
        "CPL",
        "SCF",
        "CCF",
        "HALT",
        "ADC",
        "SUB",
        "SBC",
        "AND",
        "XOR",
        "OR",
        "CP",
        "POP",
        "JP",
        "PUSH",
        "RET",
        "CB",
        "CALL",
        "RETI",
        "LDH",
        "JPHL",
        "DI",
        "EI",
        "RST",
        "IN_ERR",
        "IN_RLC",
        "IN_RRC",
        "IN_RL",
        "IN_RR",
        "IN_SLA",
        "IN_SRA",
        "IN_SWAP",
        "IN_SRL",
        "IN_BIT",
        "IN_RES",
        "IN_SET"};

Instruction instructions[0x100] = 
{
    [0x00] = {IN_NOP, AM_IMP},

    [0x05] = {IN_DEC, AM_R, RT_B},

    [0x0E] = {IN_LD, AM_R_D8, RT_C},

    [0x21] = {IN_JR, AM_IMP, RT_NONE, RT_NONE, ConditionType::CT_NZ},

    [0xAF] = {IN_XOR, AM_R, RT_A},

    [0xC3] = {IN_JP, AM_D16 },

    [0xF3] = {IN_DI},

    [0xFF] = {IN_RST, AM_IMP}
};

Instruction* InstructionByOpcode(uint8_t opcode)
{
    if(instructions[opcode].type == IN_NONE)
    {
        return nullptr;
    }

    return &instructions[opcode];
}
