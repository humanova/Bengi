// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma once

#include <fstream>
#include <regex>
#include <map>
#include <vector>
#include <string>

// include vm architecture information
#include "bengi/vm.h"
#include "lexer.h"

#define POSITIVEINT_INST_BASE   0x00000000
#define NEGATIVEINT_INST_BASE   0x20000000
#define NEGATIVEADDR_INST_BASE  0x40000000
#define POSITIVEADDR_INST_BASE  0x60000000
#define PRIMITIVE_INST_BASE     0x80000000
#define REGISTER_INST_BASE      0xC0000000
#define SYMBOL_INST_BASE        0xE0000000 // aka: main symbol

#define DATA_MAX 0x1FFFFFFF

class Assembler
{
    Lexer lexer;
    std::map<std::string, uint32_t> symbol_map;
    std::map<std::string, uint32_t> instruction_map;

    uint32_t add_symbol(std::string symbol_name);
    uint32_t cast_instruction(uint32_t base, uint32_t data);
    uint32_t map_to_binary(std::string s);
    std::map<std::string, uint32_t> create_instruction_map();

    bool is_main_defined = false;
    bool is_integer(std::string s);
    bool is_function(std::string s);
    bool is_label(std::string s);
    bool is_addr(std::string s);
public:
    Assembler();
    std::vector<uint32_t> compile(std::string assembly);
    std::vector<uint32_t> read_and_compile(std::string filename);
    std::string read_file(std::string filename);
    void write_file(std::string filename, std::vector<uint32_t> instructions);
};