//Emir Erbasan 2018

#include "ArgParser.h"
#include "VM.h"

using namespace std;

int main(int argc, char* argv[])
{
	IOFile iofile;
	ParseArg(argc, argv, iofile);

	ifstream bin_file(iofile.sourcePath, ios::binary);
	i32 i;

	vector<i32> prog;

	while (bin_file.read((char *)&i, sizeof(i)))
	{
		prog.push_back(i);
	}

	VM BengiVM;
	BengiVM.loadProgram(prog);
	BengiVM.run();

	return 0;
}