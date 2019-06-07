// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "../../Bengi/Bengi/VM.h"
#include "../../Bengi/Bengi/Error.h"
#include "../../Bengi/Bengi/IOFile.h"

VM *BengiVM;
IOFile iofile;

extern "C" bool LoadBinary(char* path)
{
	BengiVM = new VM();
	iofile.sourcePath = path;
	iofile.checkSource();
	BengiVM->LoadBinary(std::string(path));
}

extern "C" i32 Run()
{
	if (iofile.checkSource())
	{
		i32 result = BengiVM->run();
		delete BengiVM;
		return result;
	}
}


extern "C" i32 RunStep()
{
	static bool init = true;
	static bool running = true;
	static bool check_file = false;

	if (init)
	{
		check_file = iofile.checkSource();
		init = false;
	}
		
	if (check_file && running)
	{
		i32 tos = BengiVM->run_step();
		running = BengiVM->running;
		return tos;
	}
	else
	{
		delete BengiVM;
		check_file = false;
		running = false;
		init = true;
		return 0xFFFFFFFF;
	}
}

// VM UTILS

extern "C" int isRunning()
{
	return (int)(BengiVM->running);
}

extern "C" i32 GetStackElement(int addr)
{
	return BengiVM->_GetStackElement(addr);
}

extern "C" i32 GetRegister(int regId)
{
	return BengiVM->_GetRegisterValue(regId);
}

extern "C" i32 GetCurrFuncAddr()
{
	return BengiVM->currFunctionAddr;
}

extern "C" i32 GetCurrFuncSymbol()
{
	return BengiVM->_Addr2Symbol(BengiVM->currFunctionAddr);
}
