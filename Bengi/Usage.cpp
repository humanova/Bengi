#include "Usage.h"

void PrintUsage()
{
	std::cerr << "Usage : bengi [options] file\n"
	<< "Options: \n"
	<< "\t-h, --help              Display this information\n"
	<< "\t-c, --compile			  Compile given bengi-asm file to binary\n"
	<< "\t-s                      Set input file(path,name)\n"
	<< "\t-o, --output            Specify the output(path,name)\n"
	<< "\t-v, --version           Display version information"
	<< std::endl;

}

void PrintVersion()
{
	std::cerr << "bengi (Bengi Language Interpreter) 0.1.0\n"
		<< "Copyright (C) 2018 Emir Erbasan"
		<< std::endl;
}