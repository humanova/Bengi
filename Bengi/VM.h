//Emir Erbasan (humanova) 2019

#pragma once
#include <iostream>
#include <vector>
#include <fstream>

#define MAIN_SYMBOL 0xE0000000 
#define MEMORY_BUFFER 1000000

#define DEBUG 0

typedef int32_t i32;
typedef uint32_t ui32;

using namespace std;

class VM
{
	struct Symbol
	{
		i32 address;
		i32 symbol;
	};

	i32 PC = 1000;	// program counter
	i32 SP = 0;		// stack pointer
	i32 BP = 0;		// base pointer
	i32 AX = 0;		// general register
	i32 BX = 0;		// general register
	
	vector<i32> Memory;

	i32 typ = 0;
	i32 dat = 0;
	i32 running = 1;
	string curr_addr;
	vector<Symbol> SymbolTable;
	
	// private functions
	i32 getType(ui32 instruction);
	i32 getData(ui32 instruction);
	i32* getRegister(i32 data);
	i32* getAddress(i32 data);
	i32 getRegType(ui32 instruction);
	i32* getRegisterAddress(i32 data);
	const char* getRegisterName(i32* reg);
	Symbol getSymbol(i32 symbol_id);
	vector<Symbol> SetSymbolTable(vector<ui32> instructions);

	void fetch();
	void decode();
	void execute();
	void doPrimitive();
	
	enum InstructionType
	{
		INST,	// prim. instruction
		ADDR,	// address
		NADDR,	// negative address
		REG,	// register
		REGADDR,// register address
		SYMB,	// symbol
		PINT,	// positive integer
		NINT,	// negative integer
	};

public:
	//public functions
	VM();
	i32 run();
	void LoadInstructions(vector<ui32> prog);
	void LoadBinary(string path);
	void PrintStack();
};

