
/* 
	2019 Emir Erbasan(humanova)
	GPL v2 License, see LICENSE for more details

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

	caller :	push arg	//	push function args
			call func	//	(push PC on stack, push BP on stack, set PC to func address, set BP to BP address)
			pop arg		//	remove arguments

	callee :	push[-1]	//	get arg
			mov ax [sp] 	//	set return value
			pop		//	remove locals
			ret		//	return (BP = old BP, pop, PC = old PC, pop)

*/

#include "VM.h"
#include <string>
#pragma warning(disable : 4996)

#if DEBUG
	#define DEBUG_PRINT_SINGLE_INST(inst_str) cout << inst_str << endl
	#define DEBUG_PRINT_ARITHMETIC_INST(inst_str, val1, val2) cout << inst_str << " " << val1 << " " << val2 << endl
	#define DEBUG_PRINT_BASIC_INST(inst_str, param_str) cout << inst_str << " " << param_str << endl
	#define DEBUG_PRINT_MOV_INST(dest, src) cout << "mov " << dest << ", " << src << endl
	#define DEBUG_PRINT_SYMB_PC_PARAM(inst_str, symb, pc) cout << inst_str << " " << hex << symb << " " << dec << pc << endl
#else
	#define DEBUG_PRINT_SINGLE_INST(inst_str)
	#define DEBUG_PRINT_ARITHMETIC_INST(inst_str, val1, val2)
	#define DEBUG_PRINT_BASIC_INST(inst_str, param_str)
	#define DEBUG_PRINT_MOV_INST(dest, src)
	#define DEBUG_PRINT_SYMB_PC_PARAM(inst_str, symb, pc)
#endif

VM::VM()
{
	Memory.resize(MEMORY_BUFFER);
}

VM::~VM()
{
}

i32 VM::getType(ui32 instruction)
{
	ui32 type = 0xe0000000;
	type = (instruction & type) >> 29;
	i32 data;
	switch (type)
	{
		case 0x0:
			type = PINT;
			break;

		case 0x1:
			type = NINT;
			break;

		case 0x2:
			type = NADDR;
			break;

		case 0x3:
			type = ADDR;
			break;

		case 0x4:
			type =  INST;
			break;

		case 0x6:
			// Check if its REG or REGADDR
			type = getRegType(instruction);
			break;

		case 0x7:
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
	if (typ == PINT || typ == INST || typ == REG || typ == REGADDR || typ == ADDR)
	{
		data = buff & instruction;
		return data;
	}
	else if (typ == NINT || typ == NADDR)
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
	i32* addr;
	if (data == 0xf1)
	{
		curr_addr = "[ax]";
		addr = &Memory[AX];
	}
	else if (data == 0xf2)
	{
		curr_addr = "[bx]";
		addr = &Memory[BX];
	}
	else if (data == 0xf3)
	{
		curr_addr = "[sp]";
		addr = &Memory[SP];
	}
	else if (data == 0xf4)
	{
		curr_addr = "[bp]";
		addr = &Memory[BP];
	}
	else
	{
		printf("bengivm error : [PC=%d] invalid register instruction : %x", PC, data);
		exit(1);
	}

	return addr;
}

i32* VM::getAddress(i32 data)
{
	// Stack frame icindeysek negatif degerlerden 1 cikaricaz 
	// orn : [-1] olan her NADDR [-2] olacak 
	curr_addr = "[" + to_string(data) + "]";
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

vector<VM::Symbol> VM::SetSymbolTable(vector<ui32> instructions)
{
	vector<VM::Symbol> symbols;
	for (int i = 0; i < instructions.size(); i++)
	{
		// check func/label decls.
		if (instructions[i] == FUNC_DECL_INST || instructions[i] == LABEL_DECL_INST) 
		{
			Symbol symbol;
			symbol.symbol = instructions[++i];
			symbol.address = 1000 + i;
			symbols.push_back(symbol);
			#if DEBUG
			printf("Func symbol found : address -> %d | symbol - > %x\n", symbol.address, symbol.symbol);
			#endif
		}
	}
	return symbols;
}

VM::Symbol VM::getSymbol(i32 symbol_id)
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
		running = 0;
		DEBUG_PRINT_SINGLE_INST("end");
		break;

		// == Arithmetic Instructions ==
	case 0x1: // add
		DEBUG_PRINT_ARITHMETIC_INST("add", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] + Memory[SP];
		SP--;
		break;

	case 0x2: // sub
		DEBUG_PRINT_ARITHMETIC_INST("sub", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] - Memory[SP];
		SP--;
		break;

	case 0x3: // mul
		DEBUG_PRINT_ARITHMETIC_INST("mul", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] * Memory[SP];
		SP--;
		break;

	case 0x4: // div
		DEBUG_PRINT_ARITHMETIC_INST("div", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] / Memory[SP];
		SP--;
		break;

	case 0x5: // mod
		DEBUG_PRINT_ARITHMETIC_INST("mod", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] % Memory[SP];
		SP--;
		break;

	case 0x6: // or
		DEBUG_PRINT_ARITHMETIC_INST("or", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] | Memory[SP];
		SP--;
		break;

	case 0x7: // xor
		DEBUG_PRINT_ARITHMETIC_INST("xor", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] ^ Memory[SP];
		SP--;
		break;
	case 0x8: // and
		DEBUG_PRINT_ARITHMETIC_INST("and", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] & Memory[SP];
		SP--;
		break;

	case 0x9: // equal
		DEBUG_PRINT_ARITHMETIC_INST("eq", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] == Memory[SP];
		SP--;
		break;

	case 0xA: // not eq
		DEBUG_PRINT_ARITHMETIC_INST("ne", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] != Memory[SP];
		SP--;
		break;

	case 0xB: // lt
		DEBUG_PRINT_ARITHMETIC_INST("lt", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] < Memory[SP];
		SP--;
		break;

	case 0xC: // lt eq
		DEBUG_PRINT_ARITHMETIC_INST("le", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] <= Memory[SP];
		SP--;
		break;

	case 0xD: // gt
		DEBUG_PRINT_ARITHMETIC_INST("gt", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] > Memory[SP];
		SP--;
		break;

	case 0xE: // gt eq
		DEBUG_PRINT_ARITHMETIC_INST("ge", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] >= Memory[SP];
		SP--;
		break;

	case 0xF: // sh left
		DEBUG_PRINT_ARITHMETIC_INST("shl", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] << Memory[SP];
		SP--;
		break;

	case 0x10: // sh right
		DEBUG_PRINT_ARITHMETIC_INST("shr", Memory[SP - 1], Memory[SP]);
		Memory[SP - 1] = Memory[SP - 1] >> Memory[SP];
		SP--;
		break;

	case 0x11:
		fetch();
		decode();
		if (typ == REG)
		{
			i32* reg = getRegister(dat);
			*reg += 1;
			DEBUG_PRINT_BASIC_INST("inc", getRegisterName(reg));
		}
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			*reg += 1;
			DEBUG_PRINT_BASIC_INST("inc", curr_addr.c_str());
		}
		else if (typ == ADDR)
		{
			i32* addr = getAddress(dat);
			*addr += 1;
			DEBUG_PRINT_BASIC_INST("inc", curr_addr.c_str());
		}
		else
		{
			cerr << "bengivm error : invalid 'inc' argument" << endl;
			exit(1);
		}
		break;

	case 0x12:
		fetch();
		decode();
		if (typ == REG)
		{
			i32* reg = getRegister(dat);
			*reg -= 1;
			DEBUG_PRINT_BASIC_INST("dec", getRegisterName(reg));
		}
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			*reg -= 1;
			DEBUG_PRINT_BASIC_INST("dec", curr_addr.c_str());
		}
		else if (typ == ADDR)
		{
			i32* addr = getAddress(dat);
			*addr -= 1;
			DEBUG_PRINT_BASIC_INST("dec", curr_addr.c_str());
		}
		else
		{
			cerr << "bengivm error : invalid 'dec' argument" << endl;
			exit(1);
		}
		break;

		// == Basic Instructions == 

	case 0x50: // push
		//get next argument
		fetch();
		decode();
		if (typ == PINT || typ == NINT)
		{
			DEBUG_PRINT_BASIC_INST("push", dat);
			SP++;
			Memory[SP] = dat;
		}
		else if (typ == REG)
		{
			i32* reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("push", getRegisterName(reg));
			SP++;
			Memory[SP] = *reg;
		}
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("push", curr_addr.c_str());
			SP++;
			Memory[SP] = *reg;
		}
		else if (typ == ADDR || typ == NADDR)
		{
			i32* addr = getAddress(dat);
			DEBUG_PRINT_BASIC_INST("push", curr_addr.c_str());
			SP++;
			Memory[SP] = *addr;
		}
		else
		{
			cerr << "bengivm error : invalid 'push' argument" << endl;
			exit(1);
		}
		break;

	case 0x51: // pop
		SP--;
		DEBUG_PRINT_SINGLE_INST("pop");
		break;

	case 0x52: // load (mov ax, ?) 
		fetch();
		decode();
		if (typ == PINT || typ == NINT)
		{
			DEBUG_PRINT_BASIC_INST("load", dat);
			AX = dat;
		}
		else if (typ == REG)
		{
			i32* reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("load", getRegisterName(reg));
			AX = *reg;
		}
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("load", curr_addr.c_str());
			AX = *reg;
		}
		else if (typ == ADDR || typ == NADDR)
		{
			i32* addr = getAddress(dat);
			DEBUG_PRINT_BASIC_INST("load", curr_addr.c_str());
			AX = *addr;
		}
		else
		{
			cerr << "bengivm error : invalid 'load' argument" << endl;
			exit(1);
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

			if (dest_type == REG || dest_type == REGADDR)
			{
				i32 *reg = getRegister(dest);

				#if DEBUG
				const char* dest_reg_name;
				if (dest_type == REG)
					dest_reg_name = getRegisterName(reg);
				else
					dest_reg_name = curr_addr.c_str();;
				#endif

				if (src_type == PINT || src_type == NINT)
				{
					DEBUG_PRINT_MOV_INST(dest_reg_name, src);
					*reg = dat;
				}
				else if (src_type == REG)
				{
					i32* src_reg = getRegister(src);
					DEBUG_PRINT_MOV_INST(dest_reg_name, getRegisterName(src_reg));
					*reg = *src_reg;
				}
				else if (src_type == REGADDR)
				{

					i32* src_reg = getRegister(src);
					DEBUG_PRINT_MOV_INST(dest_reg_name, curr_addr.c_str());
					*reg = *src_reg;
				}
				else if (src_type == ADDR || src_type == NADDR)
				{
					i32* src_addr = getAddress(src);
					DEBUG_PRINT_MOV_INST(dest_reg_name, curr_addr.c_str());
					*reg = *src_addr;
				}
				else
				{
					cout << "bengivm error : invalid 'mov' argument (src must be a register, address or an immediate value)" << endl;
					exit(1);
				}
			}
			else if (dest_type == ADDR || dest_type == NADDR)
			{
				i32* dest_addr = getAddress(dest);
				string dest_name = curr_addr;
				if (src_type == PINT || src_type == NINT)
				{
					DEBUG_PRINT_MOV_INST(dest_name.c_str(), src);
					*dest_addr = src;
				}
				else if (src_type == REG)
				{
					i32* src_reg = getRegister(src);
					DEBUG_PRINT_MOV_INST(dest_name.c_str(), getRegisterName(src_reg));
					*dest_addr = *src_reg;
				}
				else if (src_type == REGADDR)
				{
					i32* src_reg = getRegister(src);
					DEBUG_PRINT_MOV_INST(dest_name.c_str(), curr_addr.c_str());
					*dest_addr = *src_reg;
				}
				else if (src_type == ADDR || src_type == NADDR)
				{
					i32* src_addr = getAddress(src);
					DEBUG_PRINT_MOV_INST(dest_name.c_str(), curr_addr.c_str());
					*dest_addr = *src_addr;
				}
				else
				{
					cout << "bengivm error : invalid 'mov' argument (src must be a register, address or an immediate value)" << endl;
					exit(1);
				}
			}
			else
			{
				cout << "bengivm error : invalid 'mov' argument (dest must be a register or address)" << endl;
				cout << "dest type was : " << dest_type << endl;
				exit(1);
			}
		}
		break;

	case 0x54: // jmp
		fetch();
		decode();
		{
			if (typ == PINT)
			{
				DEBUG_PRINT_BASIC_INST("jmp", dat);
				PC = dat - 1;
			}
			else if (typ == REG)
			{
				i32* dest_reg = getRegister(dat);
				DEBUG_PRINT_BASIC_INST("jmp", getRegisterName(dest_reg));
				PC = *dest_reg - 1;
			}
			else if (typ == REGADDR)
			{
				i32* dest_reg = getRegister(dat);
				DEBUG_PRINT_BASIC_INST("jmp", curr_addr.c_str());
				PC = *dest_reg - 1;
			}
			else if (typ == ADDR || typ == NADDR)
			{
				i32* dest_addr = getAddress(dat);
				DEBUG_PRINT_BASIC_INST("jmp", curr_addr.c_str());
				PC = *dest_addr - 1;
			}
			else
			{
				cout << "bengivm error : invalid 'jmp' argument (dest must be a register, address or imm value)" << endl;
			}
		}
		break;

	case 0x55: // jz  (jump if zero)

		fetch();
		decode();
		{
		if (typ == PINT)
		{
			DEBUG_PRINT_BASIC_INST("jz", dat);
			if (Memory[SP] == 0) { PC = dat - 1; }
		}
		else if (typ == REG)
		{
			i32* dest_reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("jz", getRegisterName(dest_reg));
			if (Memory[SP] == 0) { PC = *dest_reg - 1; }

		}
		else if (typ == REGADDR)
		{
			i32* dest_reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("jz", curr_addr.c_str());
			if (Memory[SP] == 0) { PC = *dest_reg - 1; }

		}
		else if (typ == ADDR || typ == NADDR)
		{
			i32 *dest_addr = getAddress(dat);
			DEBUG_PRINT_BASIC_INST("jz", curr_addr.c_str());
			if (Memory[SP] == 0) { PC = *dest_addr - 1; }

		}
		else if (typ == SYMB)
		{
			Symbol symb = getSymbol(Memory[PC]);
			if (Memory[SP] == 0)
			{
				PC = symb.address;
			}
			DEBUG_PRINT_SYMB_PC_PARAM("jz", symb.symbol, PC);
		}
		else
		{
			cout << "bengivm error : invalid 'jz' argument (dest must be a register,address or imm value)" << endl;
		}
		}
		SP--;
		break;

	case 0x56: // jnz
		fetch();
		decode();
		{
		if (typ == PINT)
		{
			DEBUG_PRINT_BASIC_INST("jnz", dat);
			if (Memory[SP] != 0)
			{
				PC = dat - 1;
			}
		}
		else if (typ == REG)
		{
			i32* dest_reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("jnz", getRegisterName(dest_reg));
			if (Memory[SP] != 0) { PC = *dest_reg - 1; }
		}
		else if (typ == REG)
		{
			i32* dest_reg = getRegister(dat);
			DEBUG_PRINT_BASIC_INST("jnz", curr_addr.c_str());
			if (Memory[SP] != 0) { PC = *dest_reg - 1; }
		}
		else if (typ == ADDR || typ == NADDR)
		{
			i32* dest_addr = getAddress(dat);
			DEBUG_PRINT_BASIC_INST("jnz", curr_addr.c_str());
			if (Memory[SP] != 0) { PC = *dest_addr - 1; }
			else
			{
				cout << "bengivm error : invalid 'jnz' argument (dest must be a register,address or imm value)" << endl;
			}
			SP--;
		}
		else if (typ == SYMB)
		{
			Symbol symb = getSymbol(Memory[PC]);
			if (Memory[SP] != 0)
			{
				PC = symb.address;
			}
			DEBUG_PRINT_SYMB_PC_PARAM("jnz", symb.symbol, PC);
		}
		}
		SP--;
		
		break;

	case 0x57: // cmp
		fetch();
		decode();
		if (typ == PINT || typ == NINT)
		{
			DEBUG_PRINT_ARITHMETIC_INST("cmp", Memory[SP], dat);
			Memory[SP] = (Memory[SP] == dat);
		}
		else if (typ == REG)
		{
			i32* reg = getRegister(dat);
			DEBUG_PRINT_ARITHMETIC_INST("cmp", Memory[SP], getRegisterName(reg));
			Memory[SP] = (Memory[SP] == *reg);
		}
		else if (typ == REGADDR)
		{
			i32* reg = getRegister(dat);
			DEBUG_PRINT_ARITHMETIC_INST("cmp", Memory[SP], curr_addr.c_str());
			Memory[SP] = (Memory[SP] == *reg);
		}
		else if (typ == ADDR || typ == NADDR)
		{
			i32* addr = getAddress(dat);
			DEBUG_PRINT_ARITHMETIC_INST("cmp", Memory[SP], curr_addr.c_str());
			Memory[SP] = (Memory[SP] == *addr);
		}
		else
		{
			cout << "bengivm error : invalid 'cmp' argument" << endl;
		}
		break;

	case 0x90:	// Function Decleration
		fetch();
		decode();
		break;

	case 0x91: // Func Return

		BP = Memory[SP--];
		PC = Memory[SP--] - 1;
		
		funcDepth--;
		currFunctionAddr = PC;

		DEBUG_PRINT_BASIC_INST("ret", PC);
		break;

	// Func Call:
	// push PC
	// push BP
	// PC = new PC (jump)
	// BP = BP indis
	case 0x92:
	{
		fetch();
		decode();

		//push PC-BP
		Memory[++SP] = PC + 1;
		Memory[++SP] = BP;

		//set PC and BP
		Symbol symb = getSymbol(dat);
		PC = symb.address;
		BP = SP;

		funcDepth++;
		currFunctionAddr = symb.address;

		DEBUG_PRINT_SYMB_PC_PARAM("call", symb.symbol, PC);
	}
		break;
	
	case 0x93:	// Label Decleration
	{
		fetch();
		decode();
		Symbol symbol = getSymbol(dat);

		DEBUG_PRINT_SYMB_PC_PARAM("label", symbol.symbol, PC);
	}
		break;
	
	}
}

i32 VM::run()
{
	// Get main function address
	PC = getSymbol(MAIN_SYMBOL).address;
	currFunctionAddr = PC;
	
	while (running)
	{
		fetch();
		decode();
		execute();
	}
	printf("tos : %d  SP : %d\n", Memory[SP], SP);
	return Memory[SP];
}

i32 VM::run_step()
{
	if (!init_step)
	{
		// Get main function address
		PC = getSymbol(MAIN_SYMBOL).address;
		currFunctionAddr = PC;
		init_step = true;
	}
	
	if (running)
	{
		fetch();
		decode();
		execute();
	}
	return Memory[SP];
}

void VM::LoadInstructions(vector<ui32> prog)
{
	for (i32 i = 0; i < prog.size(); i++)
	{
		Memory[PC + i] = prog[i];
	}
}

void VM::LoadBinary(string path)
{
	i32 i;
	vector<ui32> prog;
	ifstream bin_file(path.c_str(), ios::binary);
	while (bin_file.read((char *)&i, sizeof(i)))
	{
		prog.push_back(i);
	}
	#if DEBUG
		cout << "instructions : " << prog.size() << endl;
	#endif
	this->SymbolTable = SetSymbolTable(prog);
	LoadInstructions(prog);
}

void VM::PrintStack()
{
	printf("(%d)\n", PC);
	printf("Regs : PC : %d, BP : %d, SP %d, AX : %d, BX : %d\n", PC, BP, SP, AX, BX);
	for (i32 i = 0; i <= SP; i++)
	{
		printf("[%d] : %d\n", i, Memory[i]);
	}
}

// Out VM funcs
// (Called from dynamic libraries)

int VM::_GetFuncDepth()
{
	return funcDepth;
}

i32 VM::_GetRegisterValue(int regId)
{
	i32 val;
	switch (regId) {
	case 1:
		val = AX;
		break;
	case 2:
		val = BX;
		break;
	case 3:
		val = SP;
		break;
	case 4:
		val = BP;
		break;
	case 5:
		val = PC;
		break;

	default:
		val = 0xFFFFFFFF;
		break;
	}
	return val;
}

i32 VM::_GetStackElement(int addr)
{
	return Memory[addr];
}

i32 VM::_Addr2Symbol(i32 addr)
{
	for (int i = 0; i < SymbolTable.size(); i++)
	{
		if (SymbolTable[i].address == addr)
		{
			return SymbolTable[i].symbol;
		}
		else
			return 0xFFFFFFFF;
	}
}