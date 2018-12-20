#pragma once
#include <string>

struct Token
{
	std::string identifier;
	std::string value;

};

class Lexer
{
public:
	Lexer();
	~Lexer();
};

