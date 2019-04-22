/*
Emir Erbasan (humanova) 2019

	Instruction Format

	header : 3 bits
	data : 29 bits

	header format :
	100 : Primitive Instruction
	011 : Addressing ([10] etc.)
	010 : Negative Addressing ([-10] etc.)
	110 : Register
	111 : Symbol
	000 : Positive Integer
	001 : Negative Integer

	reg			data
	AX			0001
	BX			0002
	SP			0003
	BP			0004
	PC			0005

	Bengi calling convention:

	push arg2
	push arg1
	caller :	push arg	//	push function args
				call func	//	(push PC on stack,push BP on stack, set PC to func address, set BP to BP address)
				pop arg		//	remove arguments

	callee :	push[-1]	//	get arg
				mov ax [sp] //	set return value
				pop			//	remove locals
				ret			//	return (BP = old BP, pop, PC = old PC, pop)

*/

#include "VM.h"
#pragma warning(disable : 4996)


VM::VM()
{
	Memory.resize(1000000);
}

i32 VM::getType(ui32 instruction)
{
	ui32 type = 0xe0000000;
	type = (instruction & type) >> 29;
	i32 data;
	switch (type)
	{
		// primitive inst
		case 4:
			type =  INST;
			break;
		case 0:
			type = PINT;
			break;
		case 1:
			type = NINT;
			break;
		case 3:
			type = ADDR;
			break;
		case 2:
			type = NADDR;
			break;
		case 6:
			type = getRegType(instruction);
			break;
		case 7:
			type = SYMB;
			break;

		default:
			printf("bengivm error : [PC=%d] invalid instruction type", PC);
			exit(1);
	}
	return type;
}

i32 VM::getData(ui32 instruction)
{
	ui32 buff = 0x1fffffff;
	i32 data;
	if (typ == PINT)
	{
		data = buff & instruction;
		return data;
	}
	else if (typ == NINT)
	{
		data = -1 * (buff & instruction);
		return data;
	}
	else if (typ == INST)
	{
		data = buff & instruction;
		return data;
	}
	else if (typ == REG) 
	{
		data = buff & instruction;
		return data;
	}
	else if (typ == REGADDR)
	{
		data = buff & instruction;
		return data;
	}
	else if (typ == ADDR)
	{
		data = buff & instruction;
		return data;
	}
	else if (typ == NADDR)
	{
		data = -1 * (buff & instruction);
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

i32* VM::getRegisterAddress(i32 data)
{
	if (data == 0xf1)
	{
		curr_addr = "[ax]";
		return &Memory[AX];
	}
	else if (data == 0xf2)
	{
		curr_addr = "[bx]";
		return &Memory[BX];
	}
	else if (data == 0xf3)
	{
		curr_addr = "[sp]";
		return &Memory[SP];
	}
	else if (data == 0xf4)
	{
		curr_addr = "[bp]";
		return &Memory[BP];
	}
}
i32* VM::getAddress(i32 data)
{

	// Stack frame icindeysek negatif degerlerden 1 cikaricaz 
	// orn : [-1] olan her NADDR [-2] olacak 
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

i32 VM::getRegType(ui32 instruction)
{
	ui32 type = instruction & 0x1fffffff;
	if (type >= 0xf1 && type <= 0xf4)
		return REGADDR;
	else return REG;

}

i32* VM::getRegister(i32 data)
{
	i32 type = getRegType(data);
	if (type == REG)
	{
		if (data == 0x01)
		{
			return &AX;
		}
		else if (data == 0x02)
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
	}
	else if (type == REGADDR)
	{
		return getRegisterAddress(data);
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
		if (instructions[i] == 0x80000090) // Func decleration
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
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			if (debug) cout << "push " << curr_addr.c_str() << endl;
			SP++;
			Memory[SP] = *reg;
		}
		else if (typ == ADDR || typ == NADDR)
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
		if (debug) cout << "pop" << endl;
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
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			if (debug) cout << "load " << curr_addr.c_str()<< endl;
			AX = *reg;
		}
		else if (typ == ADDR || typ == NADDR)
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
			else if (src_type == REGADDR)
			{
				
				i32* src_reg = getRegister(src);
				if (debug) cout << "mov " << getRegisterName(reg) << ", " << curr_addr.c_str() << endl;
				*reg = *src_reg;
			}
			else if (src_type == ADDR || src_type == NADDR)
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
		else if (dest_type == ADDR || dest_type == NADDR)
		{
			i32* dest_addr = getAddress(dest);
			string dest_name = curr_addr;
			if (src_type == PINT || src_type == NINT)
			{
				if (debug) cout << "mov " << dest_name.c_str() << ", " << src << endl;
				*dest_addr = src;
			}
			else if (src_type == REG)
			{
				i32* src_reg = getRegister(src);
				if (debug) cout << "mov " << dest_name.c_str() << ", " << src_reg << endl;
				*dest_addr = *src_reg;
			}
			else if (src_type == REGADDR)
			{
				i32* src_reg = getRegister(src);
				if (debug) cout << "mov " << dest_name.c_str() << ", " << curr_addr.c_str() << endl;
				*dest_addr = *src_reg;
			}
			else if (src_type == ADDR || src_type == NADDR)
			{
				i32* src_addr = getAddress(src);
				if (debug) cout << "mov " << dest_name.c_str() << ", " << curr_addr.c_str() << endl;
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
		else if (typ == REGADDR)
		{
			i32* dest_reg = getRegister(dat);
			if (debug) cout << "jmp " << curr_addr.c_str() << endl;
			PC = *dest_reg - 1;
		}
		else if (typ == ADDR || typ == NADDR)
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
		else if (typ == REGADDR)
		{
			i32* dest_reg = getRegister(dat);
			if (debug) cout << "jz " << curr_addr.c_str() << endl;
			if (Memory[SP] == 0) { PC = *dest_reg - 1; }

		}
		else if (typ == ADDR || typ == NADDR)
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
		else if (typ == REG)
		{
			i32* dest_reg = getRegister(dat);
			if (debug) cout << "jnz " << curr_addr.c_str() << endl;
			if (Memory[SP] != 0) { PC = *dest_reg - 1; }
		}
		else if (typ == ADDR || typ == NADDR)
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
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			if (debug) cout << "cmp " << Memory[SP] << " " << curr_addr.c_str() << endl;
			Memory[SP] = (Memory[SP] == *reg);
		}
		else if (typ == ADDR || typ == NADDR)
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
		decode();
		break;

	case 0x91: // Func Return
		BP = Memory[SP--];
		PC = Memory[SP--] - 1;
		if (debug) printf("ret (%d)\n", PC);
		break;

	// Func Call:
	// push PC
	// push BP
	// PC = new PC (jump)
	// BP = BP indis
	case 0x92: 
		fetch();
		decode();
		//push PC
		Memory[++SP] = PC + 1;
		//push BP
		Memory[++SP] = BP;

		//set PC 
		Symbol symb = GetSymbol(dat);
		PC = symb.address;
		//set BP
		BP = SP;

		if (debug) printf("call %x (%d)\n", symb.symbol, PC );

		break;
	}
}

void VM::PrintStack()
{
	printf("[%d] || STACK ||\n",PC);
	printf("Regs : PC : %d, BP : %d, SP %d, AX : %d, BX : %d\n", PC, BP, SP, AX, BX);
	for (i32 i = 0; i <= SP; i++)
	{
		printf("[%d] : %d\n", i, Memory[i]);
	}
}

i32 VM::run()
{
	//PC -= 1;
	// Get main function address
	PC = GetSymbol(0xE0000000).address;
	
	while (running)
	{
		fetch();
		decode();
		execute();
	}
	printf("tos : %d  SP : %d\n", Memory[SP], SP);
	//PrintStack();
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
