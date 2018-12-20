#pragma once
#include <fstream>
#include <string>
#include <tchar.h>

#include "Error.h"



class IOFile
{
public:
	
	IOFile();
	IOFile(char* sourcePath, char* outPath);
	IOFile(std::string sourcePath, std::string outPath);

	FILE* sourceFile;
	FILE* outFile;
	char* sourcePath;
	char* outPath;

	std::ifstream s_sourceFile;
	std::string s_sourcePath;
	std::ofstream s_outFile;
	std::string s_outPath;

	void LoadSource(char* sourcePath);
	void LoadSource(char* sourcePath, FILE* outPath);
	void LoadSource();

	void s_LoadSource(std::string sourcePath, std::ofstream &file);
	void s_LoadSource(std::string sourcePath);
	void s_LoadSource();

	bool isSourceSet();
	bool isOutputSet();

	bool s_isSourceSet(); 
	bool s_isOutputSet();



private:

};

