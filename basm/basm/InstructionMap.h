// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma once

#include <map>

using namespace std;
class InstructionMap
{
public:

	InstructionMap();
	~InstructionMap();

	void InitMap();

	map<string, uint32_t> instructionMap;
};

InstructionMap::InstructionMap()
{
	InitMap();
}


InstructionMap::~InstructionMap()
{
}

void InstructionMap::InitMap()
{
	// Arithmetic Instructions
	instructionMap =
	{
			{ "end",		0x80000000 },
			{ "add",		0x80000001 },
			{ "sub",		0x80000002 },
			{ "mul",		0x80000003 },
			{ "div",		0x80000004 },
			{ "mod",		0x80000005 },
			{ "or",			0x80000006 },
			{ "xor",		0x80000007 },
			{ "and",		0x80000008 },
			{ "eq",			0x80000009 },
			{ "ne",			0x8000000A },
			{ "lt",			0x8000000B },
			{ "le",			0x8000000C },
			{ "gt",			0x8000000D },
			{ "ge",			0x8000000E },
			{ "shl",		0x8000000F },
			{ "shr",		0x80000010 },
			{ "inc",		0x80000011 },
			{ "dec",		0x80000012 },

			// Primitive Instructions
			{ "push",		0x80000050 },
			{ "pop",		0x80000051 },
			{ "load",		0x80000052 },
			{ "mov",		0x80000053 },
			{ "jmp",		0x80000054 },
			{ "jz",			0x80000055 },
			{ "jnz",		0x80000056 },
			{ "cmp",		0x80000057 },
			{ "func",		0x80000090 },
			{ "ret",		0x80000091 },
			{ "call",		0x80000092 },
			{ "label",		0x80000093 },

			// Registers
			{ "ax",			0xc0000001 },
			{ "bx",			0xc0000002 },
			{ "sp",			0xc0000003 },
			{ "bp",			0xc0000004 },
			{ "pc",			0xc0000005 },

			// Register Addr
			{ "[ax]",		0xc00000f1 },
			{ "[bx]",		0xc00000f2 },
			{ "[sp]",		0xc00000f3 },
			{ "[bp]",		0xc00000f4 }
	};
}