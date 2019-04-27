#include "Usage.h"
#include "Version.h"

void PrintUsage()
{
	std::cerr << "Usage : bengi [options] file\n"
	<< "Options: \n"
	<< "\t-h, --help              Display this information\n"
	<< "\t-c, --compile		Compile given basm file to .cben\n"
	<< "\t-s                      Set input file(path,name)\n"
	<< "\t-o, --output            Specify the output(path,name)\n"
	<< "\t-v, --version           Display version information"
	<< std::endl;

}

void PrintVersion()
{
	std::cerr << "bengi (Bengi Language Virtual Machine) "
		<< s_GetVersion() << "\n"
		<< "Copyright (C) 2019 Emir Erbasan"
		<< std::endl;
}