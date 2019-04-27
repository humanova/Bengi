//Emir Erbasan (humanova) 2019

#include "../Bengi/VM.h"
#include "../Bengi/IOFile.h"

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





