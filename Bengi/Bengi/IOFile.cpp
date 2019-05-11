// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "IOFile.h"

//disable 
#pragma warning( disable : 4996 )

//Default constructor
IOFile::IOFile()
{
	return;
}

//Constructor with pre-defined sourcePath and outPath
IOFile::IOFile(char* sourcePath, char* outPath)
{
	this->sourcePath = sourcePath;
	this->outPath = outPath;
	LoadSource(this->sourcePath);
	return;
}

FILE* IOFile::LoadSource(char* sourcePath, FILE* file)
{
	file = fopen(sourcePath, "r");
	if (file == NULL)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->sourcePath = sourcePath;
	return file;
}

FILE* IOFile::LoadSource(char* sourcePath)
{
	sourceFile = fopen(sourcePath, "r");
	if (sourceFile == NULL)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->sourcePath = sourcePath;
	return sourceFile;
}

FILE* IOFile::LoadSource()
{
	sourceFile = fopen(sourcePath, "r");
	if (sourceFile == NULL)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->sourcePath = sourcePath;
	return sourceFile;
}

//========== LOAD FILE WITH STRING STUFF ===========

//Constructor with pre-defined s_sourcePath and s_outPath

IOFile::IOFile(std::string s_sourcePath, std::string s_outPath)
{
	this->s_sourcePath = s_sourcePath;
	this->s_outPath = s_outPath;
	s_LoadSource(this->s_sourcePath);
	return;
}

/*Load the source from s_sourcePath (given in argument) and open with referenced "file"
void IOFile::s_LoadSource(std::string s_sourcePath, std::ofstream &file)
{
	file.open(s_sourcePath.c_str(), 1);
	if (!file.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->s_sourcePath = s_sourcePath;
	file.close();
}*/

#if defined(__WIN32__)

//Overload 1 - load the source from s_sourcePath and open with class variable "file" 
void IOFile::s_LoadSource(std::string s_sourcePath)
{
	
	this->s_sourceFile.open(s_sourcePath.c_str(), 1);
	if (!s_sourceFile.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->s_sourcePath = s_sourcePath;
	s_sourceFile.close();
}

//Overload 2 - load the source from s_sourcePath (class variable) and open with class variable "file"
void IOFile::s_LoadSource()
{

	this->s_sourceFile.open(s_sourcePath.c_str(), 1);
	if (!s_sourceFile.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	s_sourceFile.close();
}

#else
void IOFile::s_LoadSource(std::string s_sourcePath)
{
	
	this->s_sourceFile = std::ifstream(s_sourcePath);
	if (!s_sourceFile)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->s_sourcePath = s_sourcePath;
	s_sourceFile.close();
}

//Overload - load the source from s_sourcePath (class variable) and open with class variable "file"
void IOFile::s_LoadSource()
{

	this->s_sourceFile = std::ifstream(s_sourcePath);
	if (!s_sourceFile)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	s_sourceFile.close();
}
#endif

bool IOFile::isSourceSet()
{
	if (sourcePath != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IOFile::isOutputSet()
{
	if (outPath != NULL)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IOFile::checkSource()
{
	sourceFile = fopen(sourcePath, "r");
	if (sourceFile == NULL)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	return true;
}

//Returns true if s_sourcePath is set
bool IOFile::s_isSourceSet()
{
	if (s_sourcePath != std::string())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

//Returns true if s_outPath is set
bool IOFile::s_isOutputSet()
{
	if (s_outPath != std::string())
	{
		return 1;
	}
	else
	{
		return 0;
	}
}
