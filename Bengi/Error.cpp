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

void VMError(int errorId, int arg)
{
	switch (errorId)
	{
	case MALLOC_ERROR_SRC_SEGMENT:
		std::cerr << "bengi error : [VM] Couldn't allocate memory for the source segment" << std::endl;
		break;

	case MALLOC_ERROR_TEXT_SEGMENT:
		std::cerr << "bengi error : [VM] Couldn't allocate memory for the text segment" << std::endl;
		break;

	case MALLOC_ERROR_DATA_SEGMENT:
		std::cerr << "bengi error : [VM] Couldn't allocate memory for the data segment" << std::endl;
		break;

	case MALLOC_ERROR_STACK_SEGMENT:
		std::cerr << "bengi error : [VM] Couldn't allocate memory for the stack segment" << std::endl;
		break;

	case VM_UNKNOWN_INSTRUCTION:
		std::cerr << "bengi error : [VM] Unknown instruction:" << std::endl;
		break;

	case SOURCE_READING_ERROR:
		std::cerr << "bengi error : [VM] Error while reading input file" << std::endl;
		break;

	default:
		std::cerr << "wrong error id";
		break;
	}
}
