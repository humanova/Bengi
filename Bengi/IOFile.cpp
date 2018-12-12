#include "IOFile.h"

IOFile::IOFile()
{
	return;
}

IOFile::IOFile(std::string sourcePath, std::string outPath)
{
	this->sourcePath = sourcePath;
	this->outPath = outPath;
	LoadSource(this->sourcePath);
	return;
}

void IOFile::LoadSource(std::string sourcePath, std::ofstream &file)
{
	file.open(sourcePath, 1);
	if (!file.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(1);
	}
	this->sourcePath = sourcePath;
}

void IOFile::LoadSource(std::string sourcePath)
{
	
	this->sourceFile.open(sourcePath, 1);
	if (!sourceFile.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(1);
	}
	this->sourcePath = sourcePath;
}

void IOFile::LoadSource()
{

	this->sourceFile.open(sourcePath, 1);
	if (!sourceFile.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(1);
	}
}

bool IOFile::isSourceSet()
{
	if (sourcePath != std::string())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

bool IOFile::isOutputSet()
{
	if (outPath != std::string())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
