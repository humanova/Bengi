// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "assembler.h"

#include "cxxopts.hpp"
#include <iostream>


cxxopts::ParseResult parse_args(int argc, char* argv[])
{
    try{
        cxxopts::Options options(argv[0], "bengi assembler");
        options
            .positional_help("[optional args]")
            .show_positional_help();

        options.add_options("General")
            ("h,help", "display help");
        options.add_options("Assembler")
            ("c,compile", "compile basm file", cxxopts::value<std::string>()->default_value("def"))
            ("o,output", "output file", cxxopts::value<std::string>()->default_value("def"))
            ("d,debug", "run in debugging mode", cxxopts::value<bool>()->default_value("false"));
            
        options.parse_positional({"compile"});
        auto res = options.parse(argc, argv); 

        if (res.count("help"))
        {
            std::cout << options.help({"General", "Assembler"}) << std::endl;
            exit(0);
        }

        if (res["compile"].as<std::string>() == "def")
        {
            std::cout << "no input files" << std::endl;
            exit(1);
        }

        return res;
    }
    catch (const cxxopts::OptionException& e)
    {
        std::cout << "couldn't parse options : " << e.what() << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[])
{
    cxxopts::ParseResult args = parse_args(argc, argv);

    bool DEBUG_MODE = args["debug"].as<bool>(); 
    std::string basm_file = args["compile"].as<std::string>();
    std::string output_file = (args["output"].as<std::string>() == "def") ? (basm_file) : (args["output"].as<std::string>());
    
    std::vector<uint32_t> instructions;
    try{
        Assembler assembler;
        instructions = assembler.read_and_compile(basm_file);
        assembler.write_file(output_file, instructions);
    }
    catch(std::string& exception){
        std::cout << "error : " << exception << std::endl;
        exit(1);
    }
    
    if (DEBUG_MODE)
    {
        uint32_t idx = 1;
        for (uint32_t& inst : instructions)
        {
            std::cout << std::dec << idx << std::hex << " : 0x" <<  inst << std::endl; idx++;
        }
    }

    return 0;
}