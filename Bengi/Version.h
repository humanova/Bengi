#pragma once
#include <string>

#define VERSION_MAJOR 0
#define VERSION_MINOR 1
#define VERSION_BUILD 0

struct version
{
	short int major = VERSION_MAJOR;
	short int minor = VERSION_MINOR;
	short int build = VERSION_BUILD;
};

char* GetVersion();
std::string s_GetVersion();