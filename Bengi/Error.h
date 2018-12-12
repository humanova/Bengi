#pragma once

#include <iostream>

enum Error
{
	NO_INPUT_FILES,
	INPUT_FILE_NOT_FOUND,
	TOO_MANY_ARGUMENTS,
	COMMAND_NOT_RECOGNIZED,
	OUTPUT_NOT_SET,
	SOURCE_ALREADY_SET,
	OUTPUT_ALREADY_SET
};

void BengiError(int errorId, char* arg = 0);