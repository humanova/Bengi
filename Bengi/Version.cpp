// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "Version.h"

char* GetVersion()
{
	char version_buffer[20];
	sprintf_s(version_buffer, "%d.%d.%d", VERSION_MAJOR, VERSION_MINOR, VERSION_BUILD);
	return version_buffer;
}

std::string s_GetVersion()
{
	std::string version;
	version = std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_BUILD);
	return version;
}