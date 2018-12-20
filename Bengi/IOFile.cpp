#include "IOFile.h"

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


void IOFile::LoadSource(char* sourcePath, FILE* file)
{
	fopen_s(&file,sourcePath, "r");
	if (file == NULL)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->sourcePath = sourcePath;
}

void IOFile::LoadSource(char* sourcePath)
{
	fopen_s(&sourceFile, sourcePath, "r");
	if (sourceFile == NULL)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->sourcePath = sourcePath;
}

void IOFile::LoadSource()
{
	fopen_s(&sourceFile, sourcePath, "r");
	if (sourceFile == NULL)
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->sourcePath = sourcePath;
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

//Load the source from s_sourcePath (given in argument) and open with referenced "file"
void IOFile::s_LoadSource(std::string s_sourcePath, std::ofstream &file)
{
	file.open(s_sourcePath, 1);
	if (!file.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->s_sourcePath = s_sourcePath;
}

//Overload 1 - load the source from s_sourcePath and open with class variable "file" 
void IOFile::s_LoadSource(std::string s_sourcePath)
{
	
	this->s_sourceFile.open(s_sourcePath, 1);
	if (!s_sourceFile.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
	this->s_sourcePath = s_sourcePath;
}

//Overload - load the source from s_sourcePath (class variable) and open with class variable "file"
void IOFile::s_LoadSource()
{

	this->s_sourceFile.open(s_sourcePath, 1);
	if (!s_sourceFile.is_open())
	{
		BengiError(Error::INPUT_FILE_NOT_FOUND);
		exit(-1);
	}
}

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
