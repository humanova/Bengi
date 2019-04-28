#include "Error.h"

void BengiError(int errorId, char* arg)
{
	switch (errorId)
	{
	case NO_INPUT_FILE:
		std::cerr << "bengi error : No input files" << std::endl;
		break;

	case BASM_NOT_SET:
		std::cerr << "bengi error : No basm files" << std::endl;
		break;

	case INPUT_FILE_NOT_FOUND:
		std::cerr << "bengi error : Input file not found" << std::endl;
		break;

	case TOO_MANY_ARGUMENTS:
		std::cerr << "bengi error : Too many arguments" << std::endl;
		break;

	case COMMAND_NOT_RECOGNIZED:
		std::cerr << "bengi error : '" << arg << "' command not recognized. Use --help to see CL options" << std::endl;
		break;

	case OUTPUT_NOT_SET:
		std::cerr << "bengi error : Output file not set" << std::endl;
		break;

	case SOURCE_ALREADY_SET:
		std::cerr << "bengi error : Source file is already set" << std::endl;
		break;

	case OUTPUT_ALREADY_SET:
		std::cerr << "bengi error : Output file is already set" << std::endl;
		break;

	default:
		std::cerr << "wrong error id";
		break;
	}
}
