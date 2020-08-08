// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "bytecode.h"

Bytecode::Bytecode()
{
}

Bytecode::Bytecode(std::string filename)
{
    if (Bytecode::check_file(filename))
    {
	    Bytecode::size = get_file_size(filename) * sizeof(char);
    	Bytecode::instruction_count = size / sizeof(uint32_t);

    	Bytecode::data.resize(instruction_count);
    	Bytecode::read_from_binary(filename);
    }
}

Bytecode::Bytecode(std::vector<uint32_t> bytecode_arr)
{
    uint32_t inst_count = bytecode_arr.size();
    if (inst_count >= 3)
    {
        Bytecode::size = inst_count * sizeof(uint32_t);
        Bytecode::instruction_count = inst_count;

        Bytecode::data.resize(instruction_count);
        Bytecode::data = bytecode_arr;
    }  
    else
        throw std::string("invalid bytecode vector size");
}

Bytecode::~Bytecode()
{
}

bool Bytecode::check_file(std::string filename)
{	
    if (!(filename.substr(filename.find_last_of(".") + 1) == "cben"))
        throw std::string("wrong file extension");

    std::ifstream file(filename);
    if (!file.is_open())
        throw std::string("error while opening bytecode file");
    file.close();

    return true;
}

std::streampos Bytecode::get_file_size(std::string filename)
{
    std::streampos fsize = 0;
    std::ifstream file(filename, std::ios::binary );

    fsize = file.tellg();
    file.seekg( 0, std::ios::end );
    fsize = file.tellg() - fsize;
    file.close();

    return fsize;
}

void Bytecode::read_from_binary(std::string filename)
{
    uint32_t inst;
    uint32_t inst_idx = 0;
    std::ifstream binary_file(filename.c_str(), std::ios::binary);

    while(binary_file.read((char *)&inst, sizeof(inst)))
    {
        Bytecode::data[inst_idx] = inst;
        inst_idx++;
    }
    binary_file.close();
}