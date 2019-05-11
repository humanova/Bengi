// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "ArgParser.h"
#include "VM.h"

using namespace std;

int main(int argc, char* argv[])
{
	IOFile iofile;
	ParseArg(argc, argv, iofile);

	VM BengiVM;
	BengiVM.LoadBinary(iofile.sourcePath);
	i32 result = BengiVM.run();

	return result;
}