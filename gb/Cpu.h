#pragma once

#include "Common.h"

#include "Instruction.h"
#include "vector"

class Gameboy;
class Cpu
{
public:
    // Instruction set fn pointer
    typedef void (Cpu::*IN_PROC)(void);

    void Init(Gameboy* gb);
    bool Step();

    const Instruction& CurrentInstruction();
    uint16_t ReadRegister(RegisterType type);
    void SetRegister(RegisterType type, uint16_t value);

    void SetIERegister(uint8_t value) {_ie = value;}
    uint8_t GetIERegister() {return _ie;}

    void SetFlags(char z = -1, char n = -1, char h = -1, char c = -1);
private:
    // Stack functions
    void Push(uint8_t data);
    void Push16(uint16_t data);

    uint8_t Pop();
    uint8_t Pop16();
private:
    // Instruction Context
    void FetchInstruction();
    void FetchData();
    void Execute();

    bool CheckCondition() const;

    uint8_t _current_opcode;
    Instruction* _current_instruction;
    uint16_t _fetched_data;
    uint16_t _memory_dest;

    bool _dest_is_memory;
    bool _int_master_enabled;
private:
    // Flags
    int Z() const {return BIT(_f, 7);}
    int N() const {return BIT(_f, 6);}
    int H() const {return BIT(_f, 5);}
    int C() const {return BIT(_f, 4);}

    void GOTO(uint16_t address, bool push_pc);

    // Instruciton Set
    void PROC_NON();

    void PROC_NOP(); void PROC_LD(); void PROC_DI();
    void PROC_JP();  void PROC_JR(); void PROC_XOR();
    void PROC_LDH(); void PROC_POP(); void PROC_PUSH(); 
    void PROC_CALL(); void PROC_RET(); void PROC_RETI();
    void PROC_RST();
    std::vector<Cpu::IN_PROC> _lookup;
private:
    uint8_t _a, _f;
    uint8_t _b, _c;
    uint8_t _d, _e;
    uint8_t _h, _l;

    uint16_t _sp;
    uint16_t _pc;

    uint8_t _ie;
private:
    Gameboy* _gb;
};