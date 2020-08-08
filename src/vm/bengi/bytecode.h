// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma once

#include <iostream>
#include <vector>
#include <fstream>

class Bytecode
{
public:
    std::vector<uint32_t> data;
    uint32_t size;
    uint32_t instruction_count;
    Bytecode();
    Bytecode(std::string filename);
    Bytecode(std::vector<uint32_t> bytecode_array);
    ~Bytecode();

private:
    bool check_file(std::string filename);
    std::streampos get_file_size(std::string filename);
    void read_from_binary(std::string filename);
    void read_from_vector(std::vector<uint32_t> bytecode_arr);
};