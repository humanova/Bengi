#include "Error.h"

void BengiError(int errorId, char* arg)
{
	switch (errorId)
	{
	case 0:
		std::cerr << "bengi error : No input files" << std::endl;
		break;

	case 1:
		std::cerr << "bengi error : Input file not found" << std::endl;
		break;

	case 2:
		std::cerr << "bengi error : Too many options" << std::endl;
		break;

	case 3:
		std::cerr << "bengi error : '" << arg << "' command not recognized. Use --help to see CL arguments" << std::endl;
		break;

	case 4:
		std::cerr << "bengi error : Output file not set" << std::endl;
		break;

	case 5:
		std::cerr << "bengi error : Source file is already set" << std::endl;
		break;

	case 6:
		std::cerr << "bengi error : Output file is already set" << std::endl;
		break;

	default:
		std::cerr << "wrong error id";
		break;
	}
}