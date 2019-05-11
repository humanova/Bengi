// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma once
#include <fstream>
#include <string>
#if defined(__WIN32__)
	#include <tchar.h>
#endif

#include "Error.h"

class IOFile
{
public:
	
	IOFile();
	IOFile(char* sourcePath, char* outPath);
	IOFile(std::string sourcePath, std::string outPath);

	FILE* sourceFile = NULL;
	FILE* outFile = NULL;
	char* sourcePath = NULL;
	char* outPath = NULL;

	std::ifstream s_sourceFile;
	std::string s_sourcePath;
	std::ofstream s_outFile;
	std::string s_outPath;

	FILE* LoadSource(char* sourcePath);
	FILE* LoadSource(char* sourcePath, FILE* outPath);
	FILE* LoadSource();

	void s_LoadSource(std::string sourcePath, std::ofstream &file);
	void s_LoadSource(std::string sourcePath);
	void s_LoadSource();

	bool isSourceSet();
	bool isOutputSet();

	bool checkSource();
	bool s_isSourceSet(); 
	bool s_isOutputSet();

private:

};

