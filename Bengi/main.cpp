//Emir Erbasan 2018

#include "ArgParser.h"
#include "Lexer.h"
#include "Parser.h"
#include "VM.h"


int main(int argc, char* argv[])
{
	IOFile iofile;
	ParseArg(argc, argv, iofile);

	VM BengiVM(iofile);
	
	return 0;
}