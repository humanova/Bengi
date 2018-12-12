#pragma once
#include <fstream>
#include <string>
#include <tchar.h>

#include "Error.h"

class IOFile
{
public:

	IOFile();
	IOFile(std::string sourcePath, std::string outPath);

	std::ofstream sourceFile;
	std::string sourcePath;
	std::ofstream outFile;
	std::string outPath;
	bool isSourceSet(); 
	bool isOutputSet();

	void LoadSource(std::string sourcePath, std::ofstream &file);
	void LoadSource(std::string sourcePath);
	void LoadSource();

private:

};

