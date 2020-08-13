// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "vm.h"

VM::VM(Bytecode& bcode)
{
    code = bcode;
    stack.resize(STACK_SIZE / sizeof(int32_t));
    set_symbol_map();
}

VM::VM(std::string bytecode_file)
{
    code = Bytecode(bytecode_file);
    stack.resize(STACK_SIZE / sizeof(int32_t));
    set_symbol_map();
}

VM::VM(std::vector<uint32_t> bytecode_array)
{
    code = Bytecode(bytecode_array);
    stack.resize(STACK_SIZE / sizeof(int32_t));
    set_symbol_map();
}

VM::~VM()
{
}

int32_t VM::run()
{
    if (debug)
    {
        std::cout << "bytecode size : " << code.size << std::endl;
        std::cout << "instructions : " << code.instruction_count << std::endl;
        std::cout << "-- symbol table -- " << std::endl;
        for (const auto &p : symbol_map)
            std::cout << "symb : " << std::hex << p.first << " -> " << std::dec << p.second << std::endl; 
    }
    // set PC to main function address 
    PC = get_symbol_address(MAIN_SYMBOL);

    running = true;
    while (running)
    {
        next();     // get next inst.
        decode();   // decode type and data
        execute();  // execute current inst.
    }
    return stack[SP];
}

int32_t VM::get_type(uint32_t instruction)
{
    uint32_t type = (ITYPE_DISCRIMINATOR & instruction) >> 29;
    // 0x0 .. 0x7
    if (type >= IType::PINT && type <= IType::SYMB)
    {
        if (type == IType::REG)
            type = get_reg_type(instruction);
        return (int32_t) type;
    }
    else
    {
        throw (std::string("vm error : invalid instruction type, PC:") + std::to_string(PC));
    }
}

int32_t VM::get_data(uint32_t instruction)
{
    int32_t data;

    if (i_type == SYMB)
        return instruction;
    else if (i_type >= IType::PINT && i_type <= IType::REG)
    {
        data = IDATA_DISCRIMINATOR & instruction;
        if (i_type == IType::NINT || i_type == IType::NADDR)
            data *= -1;
        return data;
    }
}

int32_t* VM::get_address(int32_t data)
{
    // decrease data by 1 when dealing with 'NADDR's ( [-1] -> [-2] )
    if (BP != 0 && data < 0)
        data -= 1;
    return &stack[BP + data];
}

uint32_t VM::get_reg_type(uint32_t instruction)
{
    uint32_t type = instruction & IDATA_DISCRIMINATOR;
    
    if (type >= Reg::r_AX && type <= Reg::r_BP)
        return IType::REG;

    else if (type >= Reg::ra_AX && type <= Reg::ra_BP)
        return IType::REGADDR;

    else
        throw (std::string("vm error : invalid register data, PC:") + std::to_string(PC));
}        

int32_t* VM::get_register(int32_t data)
{
    switch(data)
    {
        case Reg::r_AX: return &AX;
        case Reg::r_BX: return &BX;
        case Reg::r_SP: return (int32_t*) &SP;
        case Reg::r_BP: return (int32_t*) &BP;
        case Reg::ra_AX: return &stack[AX];
        case Reg::ra_BX: return &stack[BX];
        case Reg::ra_SP: return &stack[SP];
        case Reg::ra_BP: return &stack[BP];
        default:
            throw (std::string("vm error : invalid register data, PC:") + std::to_string(PC));
    }
}

void VM::set_symbol_map()
{
    for (uint32_t inst = 0; inst < code.instruction_count; inst++)
    {
        if (code.data[inst] == FUNC_DECL_INST || code.data[inst] == LABEL_DECL_INST)
        {
            symbol_map.insert(std::pair<uint32_t, uint32_t>(code.data[++inst], inst));
            //if (debug) {std::cout << "symbol : addr: " << code.data[inst-1] << std::hex << " symb : 0x" << inst << std::endl;}
        }
    }
    if (symbol_map.size() == 0)
        throw (std::string("vm error : no func/label symbols has found"));
}

uint32_t VM::get_symbol_address(uint32_t symbol_id)
{
    try
    {
        return symbol_map.at(symbol_id);
    }
    catch (std::out_of_range& exception)
    {
        throw (std::string("vm error : wrong symbol id , PC=") + std::to_string(PC));
    }
}

void VM::next()
{
    PC++;
}

void VM::decode()
{
    i_type = get_type(code.data[PC]);
    i_data = get_data(code.data[PC]);
    if (debug)
    {
        std::cout << std::hex << "inst : 0x" << code.data[PC] << std::dec
        << "[type : " << i_type << ", data : " << i_data << "]" << std::endl;
    }
}

// executes primary instructions (IType::INST)
void VM::execute()
{
    switch (i_data)
    {
        case Opcode::END:
            running = false;
            break;
        case Opcode::ADD:
            stack[SP - 1] = stack[SP - 1] + stack[SP];
            SP--;
            break;
        case Opcode::SUB:
            stack[SP - 1] = stack[SP - 1] - stack[SP];
            SP--;
            break;
        case Opcode::MUL:
            stack[SP - 1] = stack[SP - 1] * stack[SP];
            SP--;
            break;
        case Opcode::DIV:
            stack[SP - 1] = stack[SP - 1] / stack[SP];
            SP--;
            break;
        case Opcode::MOD:
            stack[SP - 1] = stack[SP - 1] % stack[SP];
            SP--;
            break;
        case Opcode::OR:
            stack[SP - 1] = stack[SP - 1] | stack[SP];
            SP--;
            break;
        case Opcode::XOR:
            stack[SP - 1] = stack[SP - 1] ^ stack[SP];
            SP--;
            break;
        case Opcode::AND:
            stack[SP - 1] = stack[SP - 1] & stack[SP];
            SP--;
            break;
        case Opcode::EQ:
            stack[SP - 1] = stack[SP - 1] == stack[SP];
            SP--;
            break;
        case Opcode::NE:
            stack[SP - 1] = stack[SP - 1] != stack[SP];
            SP--;
            break;
        case Opcode::LT:
            stack[SP - 1] = stack[SP - 1] < stack[SP];
            SP--;
            break;
        case Opcode::LE:
            stack[SP - 1] = stack[SP - 1] <= stack[SP];
            SP--;
            break;
        case Opcode::GT:
            stack[SP - 1] = stack[SP - 1] > stack[SP];
            SP--;
            break;
        case Opcode::GE:
            stack[SP - 1] = stack[SP - 1] >= stack[SP];
            SP--;
            break;
        case Opcode::SHL:
            stack[SP - 1] = stack[SP - 1] << stack[SP];
            SP--;
            break;
        case Opcode::SHR:
            stack[SP - 1] = stack[SP - 1] >> stack[SP];
            SP--;
            break;
        case Opcode::INC:
            next();
            decode();
            if (i_type == IType::REG || i_type == IType::REGADDR)
            {
                auto reg = get_register(i_data);
                (*reg)++;
            }
            else if (i_type == IType::ADDR)
            {
                auto addr = get_address(i_data);
                (*addr)++;
            }
            else { throw(std::string("vm error : invalid inc argument")); }
            break;
        case Opcode::DEC:
            next();
            decode();
            if (i_type == IType::REG || i_type == IType::REGADDR)
            {
                auto reg = get_register(i_data);
                (*reg)--;
            }
            else if (i_type == IType::ADDR)
            {
                auto addr = get_address(i_data);
                (*addr)--;
            }
            else { throw(std::string("vm error : invalid dec argument")); }
            break;
        // ===================
        
        case Opcode::PUSH:
            next();
            decode();
            if (i_type == IType::PINT || i_type == IType::NINT)
                stack[++SP] = i_data;
            else if (i_type == IType::REG || i_type == IType::REGADDR)
                stack[++SP] = *get_register(i_data);
            else if (i_type == IType::ADDR || i_type == IType::NADDR)
                stack[++SP] = *get_address(i_data);
            else { throw(std::string("vm error : invalid push argument")); }
            break;

        case Opcode::POP:
            SP--;
            break;

        case Opcode::LOAD:
            next();
            decode();
            if (i_type == IType::PINT || i_type == IType::NINT)
                AX = i_data;
            else if (i_type == IType::REG || i_type == IType::REGADDR)
                AX = *get_register(i_data);
            else if (i_type == IType::ADDR || i_type == IType::NADDR)
                AX = *get_address(i_data);
            else { throw(std::string("vm error : invalid push argument")); }
            break;

        case Opcode::MOV:   // mov (dest) (src)
            next();
            decode();
            {
                int32_t* dest;
                int32_t dest_type = i_type;
                int32_t dest_data = i_data;

                if (dest_type == IType::REG || dest_type == IType::REGADDR)
                    dest = get_register(dest_data);
                else if (dest_type == IType::ADDR || dest_type == IType:: NADDR)
                    dest = get_address(dest_data); 
                else { throw(std::string("vm error : invalid mov argument (dest must be a register or address)")); }

                next();
                decode();
                int32_t src_type = i_type;
                int32_t src_data = i_data;

                if (src_type == IType::PINT || src_type == IType::NINT)
                    *dest = src_data;
                else if (src_type == IType::REG || src_type == IType::REGADDR)
                    *dest = *get_register(src_data);
                else if (src_type == IType::ADDR || src_type == IType::NADDR)
                    *dest = *get_address(src_data);
                else { throw(std::string("vm error : invalid mov argument (src must be a register, address or an immediate value)")); }
            }
            break;

        case Opcode::JMP:
            next();
            decode();
            if (i_type == IType::PINT)
                PC = i_data - 1; 
            else if (i_type == IType::REG || i_type == IType::REGADDR)
                PC = *get_register(i_data) - 1;
            else if (i_type == IType::ADDR || i_type == IType::NADDR)
                PC = *get_address(i_data) - 1;
            else if (i_type == IType::SYMB)
                PC = get_symbol_address(i_data);
            else { throw(std::string("vm error : invalid jmp argument")); }
            break; 
            
        case Opcode::JZ:
            next();
            decode();
            if (stack[SP] == 0)
            {
                if (i_type == IType::PINT)
                    PC = i_data - 1; 
                else if (i_type == IType::REG || i_type == IType::REGADDR)
                    PC = *get_register(i_data) - 1;
                else if (i_type == IType::ADDR || i_type == IType::NADDR)
                    PC = *get_address(i_data) - 1;
                else if (i_type == IType::SYMB)
                    PC = get_symbol_address(i_data);
                else { throw(std::string("vm error : invalid jz argument")); }
            }
            SP--;
            break; 

        case Opcode::JNZ:
            next();
            decode();
            if (stack[SP] != 0)
            {
                if (i_type == IType::PINT)
                    PC = i_data - 1; 
                else if (i_type == IType::REG || i_type == IType::REGADDR)
                    PC = *get_register(i_data) - 1;
                else if (i_type == IType::ADDR || i_type == IType::NADDR)
                    PC = *get_address(i_data) - 1;
                else if (i_type == IType::SYMB)
                    PC = get_symbol_address(i_data);
                else { throw(std::string("vm error : invalid jnz argument")); }
            }
            SP--;
            break; 

        case Opcode::CMP:
            next();
            decode();
            if (i_type == IType::PINT || i_type == IType::NINT)
                stack[SP] = (stack[SP] == i_data);
            else if (i_type == IType::REG || i_type == IType::REGADDR)
                stack[SP] = (stack[SP] == *get_register(i_data));
            else if (i_type == IType::ADDR || i_type == IType::NADDR)
                stack[SP] = (stack[SP] == *get_address(i_data));
            else { throw(std::string("vm error : invalid cmp argument")); }
            break;

        case Opcode::FUNC: // function declaration
            next();
            decode();
            break;
        
        case Opcode::RET: // function return from stack frame
            if (func_depth > 0)
            {
                BP = stack[SP--];       // set old BP
                PC = stack[SP--] - 1;   // set old PC
                func_depth--;
            }
            else { throw(std::string("vm error : invalid return from main function")); }
            break;
        
        case Opcode::CALL:
            next();
            decode();
            if (i_type == IType::SYMB) 
            {
                stack[++SP] = PC + 1;
                stack[++SP] = BP;
                PC = get_symbol_address(i_data);
                BP = SP;

                func_depth++;
            }
            else { throw(std::string("vm error : invalid call argument")); }
            break;

        case Opcode::LABEL:
            next();
            decode();
            if (!i_type == IType::SYMB)
            { throw(std::string("vm error : invalid label argument")); }
            break;
        default:
            { throw(std::string("vm error : invalid instruction")); }
            break;
    }           
    if (debug)
        std::cout << "[PC=" << PC << ",SP="  << SP << "] : " << stack[SP] << std::endl << "------" << std::endl;
}

// ====== shared library funcs =======

extern "C" int32_t VM::_run_step()
{
    if (!_init_step)
    {
        PC = get_symbol_address(MAIN_SYMBOL);
        _init_step = true;
    }

    running = true;
    if (running)
    {
        next();
        decode();
        execute();
    }
    return stack[SP];
}

extern "C" uint32_t VM::_get_func_depth()
{
    return func_depth;
}

extern "C" int32_t VM::_get_register_value(Reg id)
{
    switch(id)
    {
        case Reg::r_AX: return AX;
        case Reg::r_BX: return BX;
        case Reg::r_SP: return (int32_t) SP;
        case Reg::r_BP: return (int32_t) BP;
        case Reg::r_PC: return (int32_t) PC;
        default: return 0xFFFFFFFF;
    }
}

extern "C" int32_t VM::_get_stack_element_value(uint32_t addr)
{
    return stack[addr];
}