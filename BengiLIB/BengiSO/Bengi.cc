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