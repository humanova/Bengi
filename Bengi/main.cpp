//Emir Erbasan 2018-19

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