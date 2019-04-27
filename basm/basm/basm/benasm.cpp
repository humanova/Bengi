//Emir Erbasan (humanova) 2019
#pragma warning(disable : 4996)
#include <iostream>
#include <fstream>
#include "Lexer.h"

typedef uint32_t ui32;

using namespace std;

// functions
vector<ui32> Compile(strings s);
bool DefineMain();
bool CheckDefined(string func_name);
ui32 mapToNumber(string s);
ui32 mapToSymbol(string s);
ui32 GetSymbol(string name);

bool isInteger(string s);
bool isAddress(string s);
bool isFunction(string s);
bool isNegative(string s);

struct Symbol 
{
	string name;
	ui32 symbol;
};

vector<Symbol> SymbolTable;
bool isMainDefined = false;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <basm-file>" << endl;
		exit(1);
	}
	
	//read input file
	ifstream input_file;
	input_file.open(argv[1]);
	if (!input_file.is_open())
	{
		cout << "basm error : could not open [" << argv[1] << "]" << endl;
		exit(1);
	}

	string line;
	string contents;
	while(getline(input_file, line))
	{
		contents += line + "\n";
	}
	input_file.close();

	// parse input file
	Lexer lexer;
	strings lexemes = lexer.lex(contents);

	//compile to binary
	DefineMain();
	vector<ui32> instructions = Compile(lexemes);
	
	// write to a bin file
	ofstream ofile;
	
	char* filename;
	string fn;
	for (ui32 i = 0; argv[1][i] != '.'; i++) 
	{
		fn[i] = argv[1][i];
	}
	filename = strcat(reinterpret_cast<char *>(&fn), ".cben");
	ofile.open(filename, ios::binary);
	for (ui32 i = 0; i < instructions.size(); i++)
	{
		ofile.write(reinterpret_cast<char *>(&instructions[i]), sizeof(ui32));
	}

	ofile.close();
	return 0;
}

vector<ui32> Compile(strings s)
{
	vector<ui32> instructions;
	for (ui32 i = 0; i < s.size(); i++)
	{
		// Get positive integers
		if (isInteger(s[i]))
		{
			instructions.push_back(stoi(s[i]));

		}
		// Get negative integers
		else if (isNegative(s[i])) // check if the first char is -
		{
			if (isInteger(s[i + 1]))
			{
				ui32 num = stoi(s[++i]);
				ui32 instruction = 0x20000000 | num;
				instructions.push_back(instruction);
			}
			else
			{
				cout << "basm error : invalid negative integer format : there is no integer after '-'" << endl;
				exit(1);
			}
		}
		else if (isAddress(s[i])) // check [
		{
			string str = s[i]; // get [
			if (isNegative(s[i + 1]))
			{
				str += s[++i]; // get '-'
				str += s[++i]; // get number
				if (s[++i] != "]") //check ]
				{
					cout << "basm error : invalid adressing format" << endl;
					exit(1);
				}
				str += s[i];
			}
			else
			{
				str += s[++i];     // middle 
				if (s[++i] != "]") //check ]
				{
					cout << "basm error : invalid adressing format" << endl;
					exit(1);
				}
				str += s[i];
			}
			
			ui32 instruction = mapToNumber(str);
			instructions.push_back(instruction);

		}
		else if (isFunction(s[i]))
		{
			string str = s[i]; // get .
			str += s[++i];     // function name
			string func_name = s[i];
			if (s[++i] != ":") //check :
			{
				cout << "basm error : invalid function decleration" << endl;
				exit(1);
			}
			str += s[i];
			// Push func instruction first eg: func .main: 
			ui32 FuncInstruction = mapToNumber("func");
			instructions.push_back(FuncInstruction);

			// Get symbol by function name

			ui32 FunctionSymbol = mapToSymbol(func_name);
			printf("symbolizing function : %s (symbol : %x)\n",func_name, FunctionSymbol);
			instructions.push_back(FunctionSymbol);

		}
		else
		{

			if (mapToNumber(s[i]) != -1)
			{
				ui32 instruction = mapToNumber(s[i]);
				instructions.push_back(instruction);
			}
			// check if it's a symbol or not (function call)
			else if (CheckDefined(s[i]))
			{
				ui32 symbol = GetSymbol(s[i]);
				instructions.push_back(symbol);
			}
			else
			{
				cout << "basm error : invalid instruction [" << s[i] << "]" << endl;
				exit(1);
			}
		}
	}
	if (isMainDefined)
	{
		return instructions;
	}
	else
	{
		cout << "basm error : 'main' function isn't defined" << endl;
		exit(1);
	}
}

bool isInteger(string s)
{
	for (ui32 i = 0; i < s.length(); i++)
	{
		if (!isdigit(s[i]))
		{
			return false;
		}
	}
	return true;
}

bool isAddress(string s)
{
	if (s == "[" || s == "]")
	{
		return true;
	}
	return false;
}

bool isFunction(string s)
{
	if (s == "." || s == ":")
	{
		return true;
	}
	return false;
}

bool isNegative(string s)
{
	if (s == "-")
	{
		return true;
	}
	return false;
}

bool CheckDefined(string func_name)
{
	for (int i = 0; i < SymbolTable.size(); i++)
	{
		if (SymbolTable[i].name != func_name)
			continue;
		else
		{
			cout << "basm error : function '" << func_name << "' is already defined" << endl;
			return true;
		}
	}
	return false;
}

bool DefineMain()
{
	Symbol symbol;
	symbol.name = "main";
	symbol.symbol = 0xE0000000;
	SymbolTable.push_back(symbol);
}

ui32 GetSymbol(string name)
{
	for (int i = 0; i < SymbolTable.size(); i++)
	{
		if (SymbolTable[i].name == name)
			return SymbolTable[i].symbol;
	}
	return -1;
}

ui32 mapToSymbol(string s)
{
	if (s == "main")
	{
		if (!isMainDefined)
		{
			isMainDefined = true;
			return 0xE0000000;
		}
		else
		{
			cout << "basm error : function 'main' is already defined" << endl;
			exit(1);
		}
	}
	else
	{
		if (CheckDefined(s))
			exit(1);
		Symbol symbol;
		symbol.name = s;
		symbol.symbol = SymbolTable.back().symbol + 0x10;
		SymbolTable.push_back(symbol);

		return symbol.symbol;
	}
}
ui32 mapToNumber(string s)
{
	if (s == "end")
	{
		return 0x80000000;
	}
	else if (s == "add")
	{
		return 0x80000001;
	}
	else if (s == "sub")
	{
		return 0x80000002;
	}
	else if (s == "mul")
	{
		return 0x80000003;
	}
	else if (s == "div")
	{
		return 0x80000004;
	}
	else if (s == "mod")
	{
		return 0x80000005;
	}
	else if (s == "or")
	{
		return 0x80000006;
	}
	else if (s == "xor")
	{
		return 0x80000007;
	}
	else if (s == "and")
	{
		return 0x80000008;
	}
	else if (s == "eq")
	{
		return 0x80000009;
	}
	else if (s == "ne")
	{
		return 0x8000000A;
	}
	else if (s == "lt")
	{
		return 0x8000000B;
	}
	else if (s == "le")
	{
		return 0x8000000C;
	}
	else if (s == "gt")
	{
		return 0x8000000D;
	}
	else if (s == "ge")
	{
		return 0x8000000E;
	}
	else if (s == "shl")
	{
		return 0x8000000F;
	}
	else if (s == "shr")
	{
		return 0x80000010;
	}

	// primitive instructions
	else if (s == "push")
	{
		return 0x80000050;
	}
	else if (s == "pop")
	{
		return 0x80000051;
	}
	else if (s == "load")
	{
		return 0x80000052;
	}
	else if (s == "mov")
	{
		return 0x80000053;
	}
	else if (s == "jmp")
	{
		return 0x80000054;
	}
	else if (s == "jz")
	{
		return 0x80000055;
	}
	else if (s == "jnz")
	{
		return 0x80000056;
	}
	else if (s == "cmp")
	{
		return 0x80000057;
	}
	else if (s == "func")
	{
		return 0x80000090;
	}
	else if (s == "ret")
	{
		return 0x80000091;
	}
	else if (s == "call")
	{
		return 0x80000092;
	}


	// registers
	else if (s == "ax")
	{
		return 0xc0000001;
	}
	else if (s == "bx")
	{
		return 0xc0000002;
	}
	else if (s == "sp")
	{
		return 0xc0000003;
	}
	else if (s == "bp")
	{
		return 0xc0000004;
	}	
	else if (s == "pc")
	{
		return 0xc0000005;
	}
	// reg addresses
	else if (s.front() == '[' && s.back() == ']') 
	{
		s.erase(0, 1); // delete [
		s.pop_back();  // delete ]
		if (s == "ax")
		{
			return 0xc00000f1;
		}
		else if (s == "bx")
		{
			return 0xc00000f2;
		}
		else if (s == "sp")
		{
			return 0xc00000f3;
		}
		else if (s == "bp")
		{
			return 0xc00000f4;
		}
		else
		{
			// Check if addr is negative or not
			ui32 instruction;
			if (s.front() == '-')
			{
				instruction = 0x40000000;
				s.erase(0, 1);
			}
			else
				instruction = 0x60000000;
			ui32 addr = stoi(s);
			instruction = instruction | addr;
			return instruction;
		}
		cout << "basm error : invalid instruction [" << s << "]" << endl;
		exit(1);

	}
	else
	{
		ui32 symbol = GetSymbol(s);
		if (symbol != -1)
			return symbol;
		else
		{
			cout << "basm error : undefined symbol : '" << s << "'"<< endl;
			exit(1);
		}
	}

	cout << "basm error : invalid instruction [" << s << "]" << endl;
	exit(1);
}