// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details
// Note : I learned and get this lexer implementation from Philip Bohun's stack-vm-tutorials github repo.

#include "Lexer.h"

strings Lexer::lex(string s)
{
	strings strlist;
	char lexeme[256];
	int i = 0, j = 0;
	State state = START;
	int done = 0;
	int len = s.length();
	int balance = 0;

	while (i < len)
	{
		switch (state)
		{
		case START:

			if (my_isspace(s[i]))
			{
				state = SKIP;
			}
			else if (isgroup(s[i]))
			{
				if (s[i] == '"')
				{
					lexeme[j++] = s[i++];
				}
			}
			else if (s[i] == '/' && s[i + 1] == '/')
			{
				i += 2;
				state = COMMENT;
			}
			else
			{
				state = READCHAR;
			}
			break;
		
		case READCHAR:

			if (my_isspace(s[i]))
			{
				state = DUMP;
			}
			else if (s[i] == '\\')
			{
				i += 2;
			}
			else if (isgroup(s[i]))
			{
				if (s[i] == '"')
				{
					lexeme[j++] = s[i++];
				}
				state = READBLOCK;
			}
			else if (isspecial(s[i]))
			{
				if (j == 0)
				{
					lexeme[j++] = s[i++];
				}
				state = DUMP;
			}
			else if (s[i] == '/' && s[i + 1] == '/')
			{
				i += 2;
				state = COMMENT;
			}
			else
			{
				lexeme[j++] = s[i++];
			}
			break;

		case READBLOCK:

			if (s[i] == beg_char && s[i] != '"')
			{
				balance++;
				lexeme[j++] = s[i++];
			}
			else if (s[i] == end_char)
			{
				balance--;
				lexeme[j++] = s[i++];
				if (balance <= 0)
				{
					state = DUMP;
				}
			}
			else if (end_char == '"' && s[i] == '\\')
			{
				i += 2;
			}
			else
			{
				lexeme[j++] = s[i++];
			}

			break;

		case SKIP:
			if (my_isspace(s[i]))
			{
				i++;
			}
			else
			{
				state = READCHAR;
			}
			break;

		case DUMP:
			if (j > 0)
			{
				lexeme[j] = 0;
				strlist.push_back(lexeme);
				j = 0;
			}
			state = START;
			break;

		case COMMENT:
			if (s[i] != '\n')
			{
				i++;
			}
			else
			{
				state = READCHAR;
			}
			break;

		case END:
			i = len;
			break;
		}
	}
	if (j > 0)
	{
		lexeme[j] = 0;
		strlist.push_back(lexeme);
	}
	return strlist;
}

bool Lexer::my_isspace(char c)
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

bool Lexer::isgroup(char c)
{
	beg_char = c;
	switch (c)
	{
	case '"':
		end_char = '"';
		return true;
	case '(':
		end_char = ')';
		return true;
	case ')':
		return true;
	default:
		return false;
	}
}

bool Lexer::isspecial(char c)
{
	switch (c)
	{
	case '[':
	case ']':
	case '.':
	case ':':
	case '-':
		return true;
	default:
		return false;
	}
}