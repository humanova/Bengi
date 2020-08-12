// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "vm.h"
#include "bytecode.h"
#include "version.h"
#include "basm/assembler.h"

#include "cxxopts.hpp"
#include <iostream>

cxxopts::ParseResult parse_args(int argc, char* argv[])
{
    try{
        cxxopts::Options options(argv[0], "bengi assembly language virtual machine");
        options
            .positional_help("[optional args]")
            .show_positional_help();

        options.add_options("General")
            ("h,help", "display help")
            ("v,version", "display version information");
        options.add_options("VM")
            ("r,run", "run bytecode", cxxopts::value<std::string>()->default_value("def"))
            ("d,debug", "run in debugging mode", cxxopts::value<bool>()->default_value("false"));
        options.add_options("Assembler")
            ("c,compile", "compile basm file", cxxopts::value<std::string>())
            ("o,output", "output file", cxxopts::value<std::string>()->default_value("def"));

        options.parse_positional({"run"});
        auto res = options.parse(argc, argv); 

        if (res.count("help"))
        {
            std::cout << options.help({"General", "VM", "Assembler"}) << std::endl;
            exit(0);
        }
        
        if (res.count("version"))
        {
            std::cout << "bengi virtual machine version "<< VM_VERSION_STR(VM_VERSION_MAJOR, VM_VERSION_MINOR, VM_VERSION_REVISION) << std::endl;
            exit(0);
        }

        if (!res.count("compile") && res["run"].as<std::string>() == "def")
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

    if (args.count("compile"))
    {   
        // compile then exit
        auto input_file = args["compile"].as<std::string>();
        auto output_file = (args["output"].as<std::string>() == "def") ? (input_file) : (args["output"].as<std::string>());
        try
        {
            Assembler* assembler = new Assembler();
            std::vector<uint32_t> instructions = assembler->read_and_compile(input_file);
            assembler->write_file(output_file, instructions);
            delete assembler;

            if (DEBUG_MODE)
            {
                uint32_t idx = 1;
                for (uint32_t& inst : instructions)
                {
                    std::cout << std::dec << idx << std::hex << " : 0x" <<  inst << std::endl; idx++;
                }
            }
            exit(0);
        } 
        catch(std::string& exception)
        {
            std::cout << exception << std::endl;
            exit(1);
        }
    }

    std::string bytecode_file = args["run"].as<std::string>();  
    int32_t vm_result;

    try{
        //Bytecode bcode(bytecode_file);
        VM vm(bytecode_file);
        vm.debug = DEBUG_MODE;
        
        vm_result = vm.run();
        std::cout << "vm result : " << vm_result << std::endl;
    }
    catch(std::exception& exception){
        std::cout << "std::exception : " << exception.what() << std::endl;
        exit(1);
    }
    catch(std::string& exception){
        std::cout << "error : " << exception << std::endl;
        exit(1);
    }
    
    return 0;
}