#pragma once
#include <iostream>
#include <vector>

typedef int32_t i32;

using namespace std;

class VM
{
	i32 PC = 1000; // program counter
	i32 SP = 0; // stack pointer
	vector<i32> Memory;

	i32 typ = 0;
	i32 dat = 0;
	i32 running = 1;

	// private functions
	i32 getType(i32 instruction);
	i32 getData(i32 instruction);
	void fetch();
	void decode();
	void execute();
	void doPrimitive();

public:
	//public functions
	VM();
	void run();
	void loadProgram(vector<i32> prog);
	bool debug = true;

};

