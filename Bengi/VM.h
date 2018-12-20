#pragma once
#include "IOFile.h"
#include "Utils.h"
#include "Error.h"

class VM
{
public:

	VM(IOFile &iofile);
	void AllocateSegmentMemory();
	void GetSrc(FILE* &src_file);
	void Eval();
	
	void Test();

	int token;
	uint32_t poolsize = 64 * 1024;
	int line = 0;
	int lineIndex = 0;
	char *src;
	char *old_src;

	
	//uint8_t *IP;
	/* VM REGISTERS */
	uint32_t *IP, // Instruction Pointer (PC - program counter)
			 *BP, // Base Pointer
			 *SP, // Stack Pointer
			 AX,  // General Register
			 CYCLE; 

	enum Instructions
	{
		LEA, IMM, JMP, CALL, JZ, JNZ, ENT, ADJ, LEV, LI, LC, SI, SC, PUSH,
		OR, XOR, AND, EQ, NE, LT, GT, LE, GE, SHL, SHR, ADD, SUB, MUL, DIV, MOD,
		OPEN, READ, CLOS, PRTF, MALC, MSET, MCMP, EXIT
	};

private:

	// VM Memory Segments
	uint32_t *text,
			 *dump_text,
			 *stack;

	char *data;

	uint8_t op, *tmp_op;

};

