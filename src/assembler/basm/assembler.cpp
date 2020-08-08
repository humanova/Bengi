// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "assembler.h"

Assembler::Assembler()
{
    instruction_map = create_instruction_map();
    symbol_map.insert(std::pair<std::string, uint32_t>("main", SYMBOL_INST_BASE));
}

std::vector<uint32_t> Assembler::compile(std::string assembly)
{
    std::vector<uint32_t> instructions;
    std::vector<std::string> lexemes = lexer.lex(assembly);

    for (std::string s : lexemes)
    {
        if (is_function(s))
        {
            instructions.push_back(map_to_binary("func"));
            if (s != ".main:")
            {
                if (symbol_map.count(s) == 0)
                {
                    std::string func_name = s.substr(1, s.length()-2);
                    uint32_t func_symbol = add_symbol(func_name); // add symbol to table

                    instructions.push_back(func_symbol);
                } else {throw (std::string("basm error : invalid function declaration (already defined) : ") + s);}
            }
            else
            {
                if (!is_main_defined)
                {
                    is_main_defined = true;
                    instructions.push_back(symbol_map.at("main"));
                } else {throw (std::string("basm error : invalid function declaration, 'main' is already defined"));}
            }
        }
        else if (is_label(s))
        {
            if (symbol_map.count(s) == 0)
            {
                std::string label_name = s.substr(0, s.length()-1);
                uint32_t label_symbol = add_symbol(label_name); // add symbol to table

                instructions.push_back(map_to_binary("label"));
                instructions.push_back(label_symbol);
            } else {throw (std::string("basm error : invalid label declaration (already defined) : ") + s);}
        }
        else 
        {
            instructions.push_back(map_to_binary(s));
        }
    }

    if (is_main_defined)
        return instructions;
    else {throw (std::string("basm error : 'main' function isn't defined"));}
}

std::vector<uint32_t> Assembler::read_and_compile(std::string filename)
{
    return compile(read_file(filename));
}

uint32_t Assembler::add_symbol(std::string symbol_name)
{
    uint32_t symbol = symbol_map.at(symbol_map.rbegin()->first) + 0x10; // last symbol + 0x10
    symbol_map.insert(std::pair<std::string, uint32_t>(symbol_name, symbol));  
    return symbol;
}

uint32_t Assembler::cast_instruction(uint32_t base, uint32_t data)
{
    if (!(data > DATA_MAX))
        return (base | data);
    else {throw (std::string("basm error : invalid instruction (overflows data section): "));}
}

uint32_t Assembler::map_to_binary(std::string s)
{
    if (instruction_map.count(s) > 0)
        return instruction_map.at(s);
    else if (symbol_map.count(s) > 0)
        return symbol_map.at(s);
    else if (is_integer(s))
    {
        int32_t int_data;
        try {
            int_data = std::stoi(s);
        } catch(std::exception& exception){throw (std::string("basm error : invalid instruction : ") + s);}

        if (int_data >= 0)
            return cast_instruction(POSITIVEINT_INST_BASE, (uint32_t)int_data);
        else
            return cast_instruction(NEGATIVEINT_INST_BASE, (uint32_t)(-1*int_data));
    }
    else if (is_addr(s))
	{	
        int32_t addr_data;
        try {
            addr_data = std::stoi(s.substr(1, s.length()-2));
        } catch(std::exception& exception){throw (std::string("basm error : invalid instruction : ") + s);}

        if (addr_data >= 0)
		    return cast_instruction(POSITIVEADDR_INST_BASE, (uint32_t)addr_data);
        else
            return cast_instruction(NEGATIVEADDR_INST_BASE, (uint32_t)(-1*addr_data));
	}
    else {throw (std::string("basm error : invalid instruction (unknown) : ") + s);}
}

bool Assembler::is_integer(std::string s)
{
    if (s.front() == '-')
        s.erase(0);
    for (char& c : s)
    {
        if (!isdigit(c))
            return false;
    }
    return true;
}

bool Assembler::is_function(std::string s)
{
    if (s.front() == '.' && s.back() == ':' && s.length() > 2)
    {
        if (!is_integer(s.substr(1, s.length() - 2)))
            return true;
    }
    return false;
}

bool Assembler::is_label(std::string s)
{
    if (s.back() == ':' && s.length() > 1)
    {
        if (!is_integer(s.substr(0, s.length() - 2)))
            return true;
    }
    return false;
}

bool Assembler::is_addr(std::string s)
{
    if (s.front() == '[' && s.back() == ']' && s.length() > 2)
    {
        if (is_integer(s.substr(1, s.length() - 2)))
            return true;
    }
    return false;
}

std::string Assembler::read_file(std::string filename)
{
    if (!(filename.substr(filename.find_last_of(".") + 1) == "basm"))
        throw(std::string("basm error : wrong file extension"));

    std::ifstream file(filename);
    if (!file.is_open())
	{
		throw(std::string("basm error : error while opening basm file"));
	}

	std::string line, basm_content;
	while(getline(file, line))
		basm_content += line + "\n";

    file.close();
	return basm_content;
}

void Assembler::write_file(std::string filename, std::vector<uint32_t> instructions)
{
    std::ofstream ofile;
    std::string fn = std::regex_replace(filename, std::regex("\\.[^.]+$"), "") + ".cben";

    try{
        ofile.open(fn.c_str(), std::ios::binary);
        for (uint32_t i = 0; i < instructions.size(); i++)
        {
            ofile.write(reinterpret_cast<char *>(&instructions[i]), sizeof(uint32_t));
        }
	    ofile.close();
    }catch(std::ofstream::failure& err) { throw(std::string("basm error : error while writing instructions to file : ") + err.what());}
}

std::map<std::string, uint32_t> Assembler::create_instruction_map()
{
    return std::map<std::string, uint32_t>
    {
        // 0x80000000 -> 0x80000012 
        // base bits : 100 | data bits : ..0000 0000 -> ..0001 0010
        { "end",	PRIMITIVE_INST_BASE | Opcode::END},
        { "add",	PRIMITIVE_INST_BASE | Opcode::ADD },
        { "sub",	PRIMITIVE_INST_BASE | Opcode::SUB },
        { "mul",	PRIMITIVE_INST_BASE | Opcode::MUL },
        { "div",	PRIMITIVE_INST_BASE | Opcode::DIV },
        { "mod",	PRIMITIVE_INST_BASE | Opcode::MOD },
        { "or",		PRIMITIVE_INST_BASE | Opcode::OR },
        { "xor",	PRIMITIVE_INST_BASE | Opcode::XOR },
        { "and",	PRIMITIVE_INST_BASE | Opcode::AND },
        { "eq",		PRIMITIVE_INST_BASE | Opcode::EQ },
        { "ne",		PRIMITIVE_INST_BASE | Opcode::NE },
        { "lt",		PRIMITIVE_INST_BASE | Opcode::LT },
        { "le",		PRIMITIVE_INST_BASE | Opcode::LE },
        { "gt",		PRIMITIVE_INST_BASE | Opcode::GT },
        { "ge",		PRIMITIVE_INST_BASE | Opcode::GE },
        { "shl",	PRIMITIVE_INST_BASE | Opcode::SHL },
        { "shr",	PRIMITIVE_INST_BASE | Opcode::SHR },
        { "inc",	PRIMITIVE_INST_BASE | Opcode::INC },
        { "dec",	PRIMITIVE_INST_BASE | Opcode::DEC },
        // 0x80000050 -> 0x80000057
        // base bits : 100 | data bits : ..0101 0000 -> ..0101 0111
        { "push",	PRIMITIVE_INST_BASE | Opcode::PUSH },
        { "pop",	PRIMITIVE_INST_BASE | Opcode::POP },
        { "load",	PRIMITIVE_INST_BASE | Opcode::LOAD },
        { "mov",	PRIMITIVE_INST_BASE | Opcode::MOV },
        { "jmp",	PRIMITIVE_INST_BASE | Opcode::JMP },
        { "jz",		PRIMITIVE_INST_BASE | Opcode::JZ },
        { "jnz",	PRIMITIVE_INST_BASE | Opcode::JNZ },
        { "cmp",	PRIMITIVE_INST_BASE | Opcode::CMP },
        // 0x80000090 -> 0x80000093
        // base bits : 100 | data bits : ..1001 0000 -> ..1001 0011
        { "func",	PRIMITIVE_INST_BASE | Opcode::FUNC },
        { "ret",	PRIMITIVE_INST_BASE | Opcode::RET },
        { "call",	PRIMITIVE_INST_BASE | Opcode::CALL },
        { "label",	PRIMITIVE_INST_BASE | Opcode::LABEL },
        // 0xc0000001 -> 0xc0000005
        // base bits : 110 | data bits : ..0001 -> ..0101
        { "ax",		REGISTER_INST_BASE | Reg::r_AX },
        { "bx",		REGISTER_INST_BASE | Reg::r_BX },
        { "sp",		REGISTER_INST_BASE | Reg::r_SP },
        { "bp",		REGISTER_INST_BASE | Reg::r_BP },
        { "pc",		REGISTER_INST_BASE | Reg::r_PC },
        // 0xc00000f1 -> 0xc00000f5
        // base bits : 110 | data bits : ..1111 0001 -> ..1111 0005
        { "[ax]",	REGISTER_INST_BASE | Reg::ra_AX },
        { "[bx]",	REGISTER_INST_BASE | Reg::ra_BX },
        { "[sp]",	REGISTER_INST_BASE | Reg::ra_SP },
        { "[bp]",	REGISTER_INST_BASE | Reg::ra_BP }
    };
}