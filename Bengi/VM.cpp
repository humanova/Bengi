#include "VM.h"
#pragma warning(disable : 4996)
/*
	Instruction Format

	header : 2 bits (3 bits for registers)
	data : 30 bits (29 bits for register data)
	
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

	bengi calling convention:

	push arg2
	push arg1
	call func (push PC on stack, jmp func address)
	add 

	
*/

VM::VM()
{
	Memory.resize(1000000);
}

i32 VM::getType(ui32 instruction)
{
	ui32 type = 0xc0000000;
	type = instruction & type;
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

i32 VM::getData(ui32 instruction)
{
	if (typ == PINT)
	{
		i32 data = 0x3fffffff;
		data = data & instruction;
		return data;
	}
	else if (typ == NINT)
	{
		i32 data = 0x3fffffff;
		data = -1 * (data & instruction);
		return data;
	}
	else if (typ == INST)
	{
		i32 data = 0x3fffffff;
		data = data & instruction;
		return data;
	}
	else if (typ == REG) 
	{
		i32 data = 0x1fffffff;
		data = data & instruction;
		return data;
	}
	else if (typ == ADDR)
	{
		i32 data = 0x3fffffff;
		data = data & instruction;
		return data;
	}
	else if (typ == SYMB)
	{
		return instruction;
	}
	else
	{
		printf("bengivm error : unknown instruction type : %d", typ);
		exit(1);
	}
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
		// Stack frame icindeysek negatif degerlerden 1 cikaricaz [-1] demek [-2] demek
		char buffer[33];
		string data_str = string((char *)(itoa(data, buffer, 10)));
		data_str = "[" + data_str + "]";
		curr_addr = data_str;
		if (BP != 0 && data < 0)
		{
			data -= 1;
		}
		return &Memory[BP + data];
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

vector<VM::Symbol> VM::GetSymbolTable(vector<ui32> instructions)
{
	vector<VM::Symbol> symbols;
	for (int i = 0; i < instructions.size(); i++)
	{
		if (instructions[i] == 0x40000090) // Func decleration
		{
			Symbol symbol;
			symbol.symbol = instructions[++i];
			symbol.address = 1000 + i;
			symbols.push_back(symbol);
			if (debug)
				printf("Func found : address -> %d | symbol - > %x\n", symbol.address, symbol.symbol);
		}
	}
	return symbols;
}

VM::Symbol VM::GetSymbol(i32 symbol_id)
{
	for (int i = 0; i < SymbolTable.size(); i++)
	{
		//printf("Symbol[%d] : symbol_id -> %d | address -> %x", i, SymbolTable[i].symbol, SymbolTable[i].address);
		if (SymbolTable[i].symbol == symbol_id)
		{
			return SymbolTable[i];
		}
	}

	printf("bengivm error : [PC=%d] wrong symbol id : 0x%x\n",PC, symbol_id);
	exit(1);
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

		// == Basic Instructions == 


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

	case 0x53: // mov dest src
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
		break;
	//TODOS :
	case 0x90:	// Function Decleration
		fetch();
		typ = SYMB;
		getData(Memory[PC]);
		break;

	case 0x91: // Func Return
		BP = Memory[SP--];
		PC = Memory[SP--];
		if (debug) printf("ret (%d)\n", PC);
		break;

	// Func Call:
	// push PC
	// push BP
	// PC = new PC (jump)
	// BP = BP indis
	case 0x92: 
		fetch();
		typ = SYMB;
		//push PC
		Memory[++SP] = PC + 1;
		//push BP
		Memory[++SP] = BP;

		//set PC 
		dat = getData(Memory[PC]);
		Symbol symb = GetSymbol(dat);
		PC = symb.address;
		//set BP
		BP = SP;

		if (debug) printf("call %x (%d)\n", symb.symbol, PC );

		break;
	}
}

i32 VM::run()
{
	//PC -= 1;
	// Get main function address
	PC = GetSymbol(0x10000000).address; 
	
	while (running)
	{
		printf("BengiVM[PC=%d] : \n", PC);
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


void VM::loadProgram(vector<ui32> prog)
{
	for (i32 i = 0; i < prog.size(); i++)
	{
		Memory[PC + i] = prog[i];
	}
}

void VM::loadBinary(string path)
{
	i32 i;
	vector<ui32> prog;
	ifstream bin_file(path.c_str(), ios::binary);
	while (bin_file.read((char *)&i, sizeof(i)))
	{
		prog.push_back(i);
	}
	if (debug) cout << "instructions : " << prog.size() << endl;
	this->SymbolTable = GetSymbolTable(prog);
	loadProgram(prog);
}
