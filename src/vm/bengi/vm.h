// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma once

#include "bytecode.h"
#include <iostream>
#include <map>

#define STACK_SIZE          500000 // 500 kB stack

#define MAIN_SYMBOL         0xE0000000
#define FUNC_DECL_INST      0x80000090
#define LABEL_DECL_INST     0x80000093

#define ITYPE_DISCRIMINATOR 0xE0000000
#define IDATA_DISCRIMINATOR 0x1FFFFFFF

// instruction types
enum IType
{
    PINT,	    // positive integer    (eg. 5)
    NINT,	    // negative integer    (eg. -11)
    NADDR,	    // negative address    (eg. [-7])
    ADDR,	    // address             (eg. [13])
    INST,	    // primary instruction (eg. push, add)
    REGADDR,    // register address    (eg. [ax])
    REG,        // register            (eg. ax, sp)
    SYMB,	    // symbol              (function or label symbol)

};

// opcode (INST) instruction data section
enum Opcode
{
    END = 0x00, ADD, SUB, MUL, DIV, MOD, OR, XOR, 
    AND, EQ, NE, LT, LE, GT, GE, SHL, SHR, INC, DEC,
    PUSH = 0x50, POP, LOAD, MOV, JMP, JZ, JNZ, CMP, 
    FUNC = 0x90, RET, CALL, LABEL,
};
        
// register (REG,REGADDR) instruction data section
enum Reg    
{
    r_AX = 0x01, r_BX, r_SP, r_BP, r_PC,                    // register data
    ra_AX = 0xf1, ra_BX = 0xf2, ra_SP = 0xf3, ra_BP = 0xf4, // registeraddress data
};

class VM
{
    Bytecode code;
    std::vector<int32_t> stack;
    std::map<uint32_t, uint32_t> symbol_map;       

    // registers
    uint32_t PC = 0;  // program counter
    uint32_t SP = 0;  // stack pointer
    uint32_t BP = 0;  // base pointer
    int32_t  AX = 0;  // general register
    int32_t  BX = 0;  // general register

    int32_t i_type = 0;  // current instruction type
    int32_t i_data = 0;  // current instruction data
    
    // misc for debugging
    uint32_t func_depth = 0;
    bool _init_step = false;

    int32_t  get_type(uint32_t instruction);
    int32_t  get_data(uint32_t instruction);
    int32_t* get_register(int32_t data);
    uint32_t get_reg_type(uint32_t instruction);
    int32_t* get_address(int32_t data);
    int32_t* get_reg_address(int32_t data);
    std::string get_register_name(int32_t* reg);
    uint32_t get_symbol_address(uint32_t symbol_id);

    void set_symbol_map();
    
    void next();        // get next instruction (PC++)
    void decode();      // get type and data of current instruction
    void execute();     // execute current instruction

public:
    bool debug = false;
    bool running = false;

    VM(Bytecode& code);
    VM(std::string bytecode_file);
    VM(std::vector<uint32_t> bytecode_array);
    ~VM();

    int32_t run();

    // debugging related
    int32_t _run_step();               // runs 1 instruction at a time
    uint32_t _get_func_depth();
    int32_t _get_register_value(Reg reg_id);
    int32_t _get_stack_element_value(uint32_t address);
};