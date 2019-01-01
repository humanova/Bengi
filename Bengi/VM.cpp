#include "VM.h"
#pragma warning(disable : 4996)
/*
	Instruction Format

	header : 2 bits (3 bits for registers)
	data : 30 bits
	
	header format :
	0 (00) => positive integer
	1 (01) => primitive instruction
	2 (10) => negative integer
	3 (11) => (111) register
	4 (11) => (110) address
	// 1 -> ax
	// 2 -> sp
	// 3 -> bp
	// 4 -> pc
*/

VM::VM()
{
	Memory.resize(1000000);
}

i32 VM::getType(i32 instruction)
{
	i32 type = 0xc0000000;
	type = (instruction & type);
	if (type == 0xc0000000) {
		type = type = (0xe0000000 & instruction) >> 29;
		if (type == 7) return 3;
		if (type == 6) return 4;
		return type;
	}
	else
	{
		type = type >> 30;
		return type;
	}
}

i32 VM::getData(i32 instruction)
{
	i32 data = 0x3fffffff;
	if (typ == REG) {
		data = 0x1fffffff;
	}
	data = data & instruction;
	return data;
}

i32* VM::getAddress(i32 data)
{
	if (data == 0xffffff1)
	{
		curr_addr = "[ax]";
		return &Memory[AX];
	}
	else if (data == 0xffffff2)
	{
		curr_addr = "[bx]";
		return &Memory[BX];
	}
	else if (data == 0xffffff3)
	{
		curr_addr = "[sp]";
		return &Memory[SP];
	}
	else if (data == 0xffffff4)
	{
		curr_addr = "[bp]";
		return &Memory[BP];
	}
	else
	{
		
		char buffer[33];
		string data_str = string((char *)(itoa(data, buffer, 10)));
		data_str = "[" + data_str + "]";
		curr_addr = data_str;
		return &Memory[data];
	}
}

i32* VM::getRegister(i32 data)
{
	if (data == 0x01)
	{
		return &AX;
	}
	if (data == 0x02)
	{
		return &BX;
	}
	else if (data == 0x03)
	{
		return &SP;
	}
	else if (data == 0x04)
	{
		return &BP;
	}
	else if (data == 0x05)
	{
		return &PC;
	}
	else
	{
		cout << "bengivm error(getRegister) : invalid register identifier" << endl;
		exit(1);
	}
}

const char* VM::getRegisterName(i32* reg)
{
	if (reg == &AX)
	{
		return "ax";
	}
	else if (reg == &BX)
	{
		return "bx";
	}
	else if (reg == &SP)
	{
		return "sp";
	}
	else if (reg == &BP)
	{
		return "bp";
	}
	else if (reg == &PC)
	{
		return "pc";
	}
	else
	{
		cout << "bengivm error(getRegisterName) : invalid register reference";
		exit(1);
	}
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
		if (debug) cout << "end" << endl;
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
		if (typ == PINT || typ == NINT)
		{
			if (debug) cout << "push " << dat << endl;
			SP++;
			Memory[SP] = dat;
		}
		else if (typ == REG)
		{
			i32* reg = getRegister(dat);
			if (debug) cout << "push " << getRegisterName(reg) << endl;
			SP++;
			Memory[SP] = *reg;
		}
		else if (typ == ADDR)
		{
			i32* addr = getAddress(dat);
			if (debug) cout << "push " << curr_addr.c_str() << endl;
			SP++;
			Memory[SP] = *addr;

		}
		else
		{
			cout << "bengivm error : invalid push argument" << endl;
			exit(1);
		}
		break;

	case 0x51: // pop
		SP--;
		break;

	case 0x52: // load (mov ax, ?) 
		fetch();
		decode();
		if (typ == PINT || typ == NINT)
		{
			if (debug) cout << "load " << dat << endl;
			AX = dat;
		}
		else if (typ == REG)
		{
			i32* reg = getRegister(dat);
			if (debug) cout << "load " << getRegisterName(reg) << endl;
			AX = *reg;
		}
		else if (typ == ADDR)
		{
			i32* addr = getAddress(dat);
			if (debug) cout << "load " << curr_addr.c_str() << endl;
			AX = *addr;
		}
		break;

	case 0x53: // mov
		fetch();
		decode();
		{
		i32 dest_type = typ;
		i32 dest = dat;
		fetch();
		decode();
		i32 src_type = typ;
		i32 src = dat;

		if (dest_type == REG)
		{
			i32 * reg = getRegister(dest);
			if (src_type == PINT || src_type == NINT)
			{
				if (debug) cout << "mov " << getRegisterName(reg) << ", " << src << endl;
				*reg = dat;
			}
			else if (src_type == REG)
			{
				i32* src_reg = getRegister(src);
				if (debug) cout << "mov " << getRegisterName(reg) << ", " << getRegisterName(src_reg) << endl;
				*reg = *src_reg;
			}
			else if (src_type == ADDR)
			{
				i32* src_addr = getAddress(src);
				if (debug) cout << "mov " << getRegisterName(reg) << ", " << curr_addr.c_str() << endl;
				*reg = *src_addr;
				
			}
			else
			{
				cout << "bengivm error : invalid mov argument (src must be a register, address or an immediate value)" << endl;
				exit(1);
			}
		}
		else if (dest_type == ADDR)
		{
			i32* dest_addr = getAddress(dest);
			if (src_type == PINT || src_type == NINT)
			{
				if (debug) cout << "mov " << curr_addr.c_str() << ", " << src << endl;
				*dest_addr = src;
			}
			else if (src_type == REG)
			{
				i32* src_reg = getRegister(src);
				if (debug) cout << "mov " << curr_addr.c_str() << ", " << src_reg << endl;
				*dest_addr = *src_reg;
			}
			else if (src_type == ADDR)
			{
				string _dest_addr = curr_addr;
				i32* src_addr = getAddress(src);
				if (debug) cout << "mov " << _dest_addr.c_str() << ", " << curr_addr.c_str() << endl;
				*dest_addr = *src_addr;
			}
			else
			{
				cout << "bengivm error : invalid mov argument (src must be a register, address or an immediate value)" << endl;
				exit(1);
			}
		}
		else
		{
			cout << "bengivm error : invalid mov argument (dest must be a register or address)" << endl;
			cout << "dest type was : " << dest_type << endl;
			exit(1);
		}
		}
		break;
		
	case 0x54: // jump
		fetch();
		decode();
		{
		if (typ == PINT)
		{
			if (debug) cout << "jmp " << dat << endl;
			PC = dat - 1;
		}
		else if (typ == REG)
		{
			i32* dest_reg = getRegister(dat);
			if (debug) cout << "jmp " << getRegisterName(dest_reg) << endl;
			PC = *dest_reg - 1;
		}
		else if (typ == ADDR)
		{
			i32* dest_addr = getAddress(dat);
			if (debug) cout << "jmp " << curr_addr.c_str() << endl;
			PC = *dest_addr - 1;
		}
		else
		{
			cout << "bengivm error : invalid jmp argument (dest must be a register, address or imm value)" << endl;
		}
		}
		break;
	
	case 0x55: // jz  (jump if zero)
		
		fetch();
		decode();
		if (typ == PINT)
		{
			if (debug) cout << "jz " << dat << endl;
			if (Memory[SP] == 0) { PC = dat - 1; } 
		}
		else if (typ == REG)
		{
			i32* dest_reg = getRegister(dat);
			if (debug) cout << "jz " << getRegisterName(dest_reg) << endl;
			if (Memory[SP] == 0) { PC = *dest_reg - 1; }
				
		}
		else if (typ == ADDR)
		{
			i32 *dest_addr = getAddress(dat);
			if (debug) cout << "jz " << curr_addr.c_str() << endl;
			if (Memory[SP] == 0) { PC = *dest_addr - 1; }
				
		}
		else
		{
			cout << "bengivm error : invalid jz argument (dest must be a register,address or imm value)" << endl;
		}
		SP--;
		break;
		
	case 0x56: // jnz
		fetch();
		decode();
			
		if (typ == PINT)
		{
			if (debug) cout << "jnz " << dat << endl;
			if(Memory[SP] != 0)
				PC = dat - 1;
		}
		else if (typ == REG)
		{
			i32* dest_reg = getRegister(dat);
			if (debug) cout << "jnz " << getRegisterName(dest_reg) << endl;
			if (Memory[SP] != 0) { PC = *dest_reg - 1; }
				
		}
		else if (typ == ADDR)
		{
			i32* dest_addr = getAddress(dat);
			if (debug) cout << "jnz " << getRegisterName(dest_addr) << endl;
			if (Memory[SP] != 0) { PC = *dest_addr - 1; }
		}
		else
		{
			cout << "bengivm error : invalid jnz argument (dest must be a register,address or imm value)" << endl;
		}
		SP--;
		break;
	
	case 0x57: // cmp
		fetch();
		decode();
		if (typ == PINT || typ == NINT)
		{
			if (debug) cout << "cmp " << Memory[SP] << " " <<  dat << endl;

			Memory[SP] = (Memory[SP] == dat);
		}
		else if (typ == REG)
		{
			i32* reg = getRegister(dat);
			if (debug) cout << "cmp " << Memory[SP] << " " << getRegisterName(reg) << endl;
			Memory[SP] = (Memory[SP] == *reg);
		}
		else if (typ == ADDR)
		{
			i32* addr = getAddress(dat);
			if (debug) cout << "cmp " << Memory[SP] << " " << curr_addr.c_str() << endl;
			Memory[SP] = (Memory[SP] == *addr);
		}
		else
		{
			cout << "bengivm error : invalid cmp argument" << endl;

		}
		
	}
}

i32 VM::run()
{
	PC -= 1;
	
	while (running)
	{
		fetch();
		decode();
		execute();
	}
	cout << "tos : " << Memory[SP] << endl;
	return Memory[SP];
}


/*void VM::reset()
{
	PC = 1000;
	AX = 0;
	SP = 0;
	BP = 0;
	Memory = vector<i32>();
	Memory.resize(1000000);
}*/


void VM::loadProgram(vector<i32> prog)
{
	for (i32 i = 0; i < prog.size(); i++)
	{
		Memory[PC + i] = prog[i];
	}
}

void VM::loadBinary(string path)
{
	i32 i;
	vector<i32> prog;
	ifstream bin_file(path.c_str(), ios::binary);
	while (bin_file.read((char *)&i, sizeof(i)))
	{
		prog.push_back(i);
	}
	if (debug) cout << "instructions : " << prog.size() << endl;
	loadProgram(prog);
}
