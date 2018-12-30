#include "VM.h"

/*
	Instruction Format

	header : 2 bits
	data : 30 bits
	
	header format :
	0 (00) => positive integer
	1 (01) => primitive instruction
	2 (10) => negative integer
	3 (11) => undefined
*/

VM::VM()
{
	Memory.resize(1000000);
}

i32 VM::getType(i32 instruction)
{
	i32 type = 0xc0000000;
	type = (type & instruction) >> 30;
	return type;
}

i32 VM::getData(i32 instruction)
{
	i32 data = 0x3fffffff;
	data = data & instruction;
	return data;
}

void VM::fetch()
{
	// Get next instruction
	PC++;	
}

void VM::decode()
{
	typ = getType(Memory[PC]);
	dat = getData(Memory[PC]);

}

void VM::execute()
{
	doPrimitive();
}

void VM::doPrimitive()
{
	switch (dat)
	{
	case 0x0: // halt
		if(debug) cout << "end" << endl;
		running = 0;
		break;

	// == Arithmetic Instructions ==
	case 0x1: // add
		if (debug) cout << "add " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] + Memory[SP];
		SP--;
		break;
	case 0x2: // sub
		if (debug) cout << "sub " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] - Memory[SP];
		SP--;
		break;
	case 0x3: // mul
		if (debug) cout << "mul " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] * Memory[SP];
		SP--;
		break;
	case 0x4: // div
		if (debug) cout << "div " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] / Memory[SP];
		SP--;
		break;
	case 0x5: // mod
		if (debug) cout << "mod " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] % Memory[SP];
		SP--;
		break;
	case 0x6: // or
		if (debug) cout << "or " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] | Memory[SP];
		SP--;
		break;
	case 0x7: // xor
		if (debug) cout << "xor " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] ^ Memory[SP];
		SP--;
		break;
	case 0x8: // and
		if (debug) cout << "and " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] & Memory[SP];
		SP--;
		break;
	case 0x9: // equal
		if (debug) cout << "eq " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] == Memory[SP];
		SP--;
		break;
	case 0xA: // not eq
		if (debug) cout << "ne " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] != Memory[SP];
		SP--;
		break;
	case 0xB: // lt
		if (debug) cout << "lt " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] < Memory[SP];
		SP--;
		break;
	case 0xC: // lt eq
		if (debug) cout << "le " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] <= Memory[SP];
		SP--;
		break;
	case 0xD: // gt
		if (debug) cout << "gt " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] > Memory[SP];
		SP--;
		break;
	case 0xE: // gt eq
		if (debug) cout << "ge " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] >= Memory[SP];
		SP--;
		break;
	case 0xF: // sh left
		if (debug) cout << "shl " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] << Memory[SP];
		SP--;
		break;
	case 0x10: // sh right
		if (debug) cout << "shr " << Memory[SP - 1] << " " << Memory[SP] << endl;
		Memory[SP - 1] = Memory[SP - 1] >> Memory[SP];
		SP--;
		break;

	// == Classic Instructions == 

	case 0x50: // push
		//get next argument
		fetch();
		decode();
		if (debug) cout << "push " << dat <<endl;
		if (typ == 0 || typ == 2)
		{
			SP++;
			Memory[SP] = dat;
		}
		else
		{
			cout << "bengivm error : invalid push argument" << endl;
			exit(1);
		}
		break;

	case 0x51: // jump
		fetch();
		decode();
		if (debug) cout << "jmp " << dat << endl;
		PC = dat;
		break;
	
	case 0x52: // jz  (jump if zero)
		if (Memory[SP] == 0)
		{
			fetch();
			decode();
			PC = dat;
		}
		break;
		
	case 0x53: // jnz
		if (Memory[SP] != 0)
		{
			fetch();
			decode();
			PC = dat;
		}
		break;

	}
	
	
		
	
}

void VM::run()
{
	PC -= 1;
	while (running)
	{
		fetch();
		decode();
		execute();
	}
	cout << "tos : " << Memory[SP] << endl;
}

void VM::loadProgram(vector<i32> prog)
{
	
	for (i32 i = 0; i < prog.size(); i++)
	{
		Memory[PC + i] = prog[i];
	}
}