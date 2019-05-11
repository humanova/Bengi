// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "../../Bengi/Bengi/VM.h"
#include "../../Bengi/Bengi/IOFile.h"

#define DLLFUNC extern "C" __declspec(dllexport)

VM *BengiVM;
IOFile iofile;

DLLFUNC bool LoadBinary(char* path)
{
	BengiVM = new VM();
	iofile.sourcePath = path;
	iofile.checkSource();
	BengiVM->LoadBinary(std::string(path));
}

DLLFUNC i32 Run()
{
	if (iofile.checkSource())
	{
		i32 result = BengiVM->run();
		delete BengiVM;
		return result;
	}
}





