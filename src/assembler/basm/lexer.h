// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma once

#include <string>
#include <vector>
#include <cstdint>

class Lexer
{
    enum State : uint8_t
    {
        START,
        READCHAR,
        READBLOCK,
        SKIP,
        DUMP,
        COMMENT,
        END
    };
	bool is_space(char c);
	bool is_group(char c);
	char end_char, beg_char;

public:
    Lexer();
	std::vector<std::string> lex(std::string s);
};