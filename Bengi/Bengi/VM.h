// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma once
#include <iostream>
#include <vector>
#include <fstream>

#define MAIN_SYMBOL 0xE0000000 
#define MEMORY_BUFFER 1000000

#define FUNC_DECL_INST 0x80000090
#define LABEL_DECL_INST 0x80000093

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
	string curr_addr;
	vector<Symbol> SymbolTable;
	int funcDepth = 0;			// main functions is 0 level depth
	bool init_step = false;
	
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
	~VM();
	i32 run();
	i32 run_step();
	void LoadInstructions(vector<ui32> prog);
	void LoadBinary(string path);
	void PrintStack();
	
	//out funcs
	int _GetFuncDepth();
	i32 _GetRegisterValue(int regId);
	i32 _GetStackElement(int addr);
	i32 _Addr2Symbol(i32 addr);

	i32 currFunctionAddr;
	i32 running = true;
};

