#include "Lexer.h"
#include "Parser.h"
#include "Usage.h"
#include "ArgParser.h"

#include "IOFile.h"

int main(int argc, char* argv[])
{
	IOFile iofile;
	ParseArg(argc, argv, iofile);



	return 0;
}