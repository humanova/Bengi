// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include <iostream>
#include "InstructionMap.h"
#include "Lexer.h"

typedef uint32_t ui32;

#define  MAIN_SYMBOL 0xE0000000

struct Symbol 
{
	string name;
	ui32 symbol;
};

vector<Symbol> SymbolTable;
map<string, ui32> instructionMap;
bool isMainDefined = false;

// function decl.
vector<ui32> Compile(strings s);
void DefineMain();
void SetInstructionMap();
bool CheckDefined(string func_name);
ui32 mapToNumber(string s);
ui32 mapToSymbol(string s);
ui32 GetSymbol(string name);

bool isInteger(string s);
bool isAddress(string s);
bool isFunction(string s);
bool isNegative(string s);


vector<ui32> Compile(strings s)
{	
	SetInstructionMap();
	DefineMain();
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
				cout << "basm error : invalid function declaration" << endl;
				exit(1);
			}
			str += s[i];
			// Push func instruction first eg: func .main: 
			ui32 FuncInstruction = mapToNumber("func");
			instructions.push_back(FuncInstruction);

			// Get symbol by function name
			ui32 FunctionSymbol = mapToSymbol(func_name);
			printf("symbolizing function : %s (symbol : %x)\n", func_name.c_str(), FunctionSymbol);
			instructions.push_back(FunctionSymbol);
		}
		// check if it's label declaration...
		else if (s[i + 1] == ":")
		{
			string name = s[i++];
			ui32 LabelInstruction = mapToNumber("label");
			instructions.push_back(LabelInstruction);

			// Get symbol by label name
			ui32 LabelSymbol = mapToSymbol(name);
			printf("symbolizing label : %s (symbol : %x)\n", name.c_str(), LabelSymbol);
			instructions.push_back(LabelSymbol);
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
	if (s == "[")
	{
		return true;
	}
	return false;
}

bool isFunction(string s)
{
	if (s == ".")
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

void DefineMain()
{
	Symbol symbol;
	symbol.name = "main";
	symbol.symbol = MAIN_SYMBOL;
	SymbolTable.push_back(symbol);
}

void SetInstructionMap()
{
	InstructionMap *iMap = new InstructionMap();
	instructionMap = iMap->instructionMap;
	delete iMap;
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
			cout << "basm error : 'main' is already defined" << endl;
			exit(1);
		}
	}
	else
	{
		if (CheckDefined(s))
		{
			cout << "basm error : '" << s << "' is already defined" << endl;
			exit(1);
		}
			
		Symbol symbol;
		symbol.name = s;
		symbol.symbol = SymbolTable.back().symbol + 0x10;
		SymbolTable.push_back(symbol);

		return symbol.symbol;
	}
}

ui32 mapToNumber(string s)
{
	if (instructionMap.count(s) > 0)
		return instructionMap[s];

	else if (GetSymbol(s) != -1)
		return GetSymbol(s);

	else if (s.front() == '[' && s.back() == ']')
	{	
		if (s[1] == '-')
		{
			s.erase(0, 2);
			s.pop_back();
			ui32 addr = stoi(s);
			return 0x40000000 | addr;
		}
		else
		{
			s.erase(0, 1);
			s.pop_back();
			ui32 addr = stoi(s);
			return 0x60000000 | addr;
		}
		cout << "basm error : invalid instruction [" << s << "]" << endl;
		exit(1);
	}
	else
	{
		cout << "basm error : invalid instruction " << s << endl;
		exit(1);
	}

}
