// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#define CATCH_CONFIG_MAIN 
#include "catch.hpp"
#include "bengi/vm.h"
#include "bengi/bytecode.h"
#include "basm/assembler.h"

#include "data.h"

TEST_CASE("Bytecode 1", "[Bytecode T1]")
{
    {
        Bytecode b(test1);
        REQUIRE(b.size == 252);
        REQUIRE(b.instruction_count == 63);
        REQUIRE(b.data[b.instruction_count-1] == 0x80000000);
        REQUIRE(b.data[3] == 0x2);
    }
}

TEST_CASE("VM Constructor Test", "[VMC T1]")
{
    // VM(std::vector<uint32_t> bytecode_array)
    {
        VM v(test1);
        REQUIRE(v.run() == 40);
    }

    // Bytecode(std::vector<uint32_t> bytecode_array) --> VM(Bytecode code)
    {
        Bytecode b(test1);
        VM v(b); 
        REQUIRE(v.run() == 40);
    }
}


TEST_CASE("Arithmetic and Logical operations " "[ALO]")
{
    {
        VM t(test1);
        REQUIRE(t.run() == 40);
    }
}

TEST_CASE("Register and address referencing operations" "[RAFO]")
{
    {
        VM t(test2);
        REQUIRE(t.run() == 60);
    }
    {
        VM t(test3);
        REQUIRE(t.run() == 11);
    }
}

TEST_CASE("Conditinonal operations", "[CO]")
{
    {
        VM t(test4);
        REQUIRE(t.run() == 46368);
    }
    {
        VM t(test5);
        REQUIRE(t.run() == 100);
    }
}

TEST_CASE("Function-Label operations", "[FLO]")
{
    {
        VM t(test6);
        REQUIRE(t.run() == 10);
    }
    {
        VM t(test7);
        REQUIRE(t.run() == 46656);
    }
}

TEST_CASE("Assembler compiling test", "[AC]")
{
    {
        Assembler asmb;
        REQUIRE(asmb.compile(test1_asm) == test1);
    }
    {
        Assembler asmb;
        REQUIRE(asmb.compile(test2_asm) == test2);
    }
    {
        Assembler asmb;
        REQUIRE(asmb.compile(test3_asm) == test3);
    }
    {
        Assembler asmb;
        REQUIRE(asmb.compile(test4_asm) == test4);
    }
    {
        Assembler asmb;
        REQUIRE(asmb.compile(test5_asm) == test5);
    }
    {
        Assembler asmb;
        REQUIRE(asmb.compile(test6_asm) == test6);
    }
    {
        Assembler asmb;
        REQUIRE(asmb.compile(test7_asm) == test7);
    }
}