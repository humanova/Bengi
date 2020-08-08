// (c) 2019, 2020 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#include "Lexer.h"

Lexer::Lexer() {}

bool Lexer::is_space(char c)
{
	switch (c)
	{
        case '\n':
        case '\r':
        case '\t':
        case '\v':
        case ' ':
        case '\f':
            return true;
        default:
            return false;
	}
}

bool Lexer::is_group(char c)
{
	beg_char = c;
	switch (c)
	{
        case '[':
            end_char = ']';
            return true;
        case ']':
            return true;
        default:
            return false;
	}
}

std::vector<std::string> Lexer::lex(std::string s)
{
	std::vector<std::string> lexeme_list;
    State state = START;
	char lexeme[256];
	int g_idx = 0, l_idx = 0;
	
	int balance = 0;
    int len = s.length();
    bool done = 0;

	while (g_idx < len)
	{
		switch (state)
		{
            case START:

                if (is_space(s[g_idx]))
                    state = SKIP;
                else if (is_group(s[g_idx]))
                {
                    if (s[g_idx] == '[')
                    {
                        lexeme[l_idx++] = s[g_idx++];   
                    }
                    state = READBLOCK;
                }
                else if (s[g_idx] == '/' && s[g_idx + 1] == '/')
                {
                    g_idx += 2;
                    state = COMMENT;
                }
                else
                    state = READCHAR;
                break;
            
            case READCHAR:

                if (is_space(s[g_idx]))
                    state = DUMP;
                else if (s[g_idx] == '\\')
                    g_idx += 2;
                else if (is_group(s[g_idx]))
                {
                    if (s[g_idx] == '[')
                    {
                        lexeme[l_idx++] = s[g_idx++];
                    }
                    state = READBLOCK;
                }
                else if (s[g_idx] == '/' && s[g_idx + 1] == '/')
                {
                    g_idx += 2;
                    state = COMMENT;
                }
                else
                    lexeme[l_idx++] = s[g_idx++];
                break;

            case READBLOCK:

                if (s[g_idx] == beg_char && s[g_idx] != ']')
                {
                    balance++;
                    lexeme[l_idx++] = s[g_idx++];
                }
                else if (s[g_idx] == end_char)
                {
                    balance--;
                    lexeme[l_idx++] = s[g_idx++];
                    if (balance <= 0)
                    {
                        state = DUMP;
                    }
                }
                else if (end_char == ']' && s[g_idx] == '\\')
                {
                    g_idx += 2;
                }
                else
                    lexeme[l_idx++] = s[g_idx++];

                break;

            case SKIP:

                if (is_space(s[g_idx]))
                    g_idx++;
                else
                    state = READCHAR;

                break;

            case DUMP:
                if (l_idx > 0)
                {
                    lexeme[l_idx] = 0;
                    lexeme_list.push_back(lexeme);
                    l_idx = 0;
                }
                state = START;
                break;

            case COMMENT:
                if (s[g_idx] != '\n')
                    g_idx++;
                else
                    state = READCHAR;
                break;

            case END:
                g_idx = len;
                break;
		}
	}
	if (l_idx > 0)
	{
		lexeme[l_idx] = 0;
		lexeme_list.push_back(lexeme);
	}
	return lexeme_list;
}