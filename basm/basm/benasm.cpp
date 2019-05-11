// 2019 Emir Erbasan(humanova)
// GPL v2 License, see LICENSE for more details

#pragma warning(disable : 4996)
#include <fstream>
#include <cstring>
#include <regex>
#include "assembler_utils.h"

using namespace std;

int main(int argc, char *argv[])
{
	if (argc != 2)
	{
		cout << "Usage: " << argv[0] << " <basm-file>" << endl;
		exit(1);
	}
	
	//read input file
	ifstream input_file;
	input_file.open(argv[1]);
	if (!input_file.is_open())
	{
		cout << "basm error : could not open [" << argv[1] << "]" << endl;
		exit(1);
	}

	string line;
	string contents;
	while(getline(input_file, line))
	{
		contents += line + "\n";
	}
	input_file.close();

	// parse input file
	Lexer lexer;
	strings lexemes = lexer.lex(contents);

	//compile to binary
	DefineMain();
	vector<ui32> instructions = Compile(lexemes);
	
	// create and write instructions to a bin file
	ofstream ofile;

	string filename;
    string fn = string(argv[1]);
    fn = std::regex_replace(fn, std::regex("\\.basm"), "");
    filename = fn + ".cben";
    ofile.open(filename.c_str(), ios::binary);
    for (ui32 i = 0; i < instructions.size(); i++)
    {
        ofile.write(reinterpret_cast<char *>(&instructions[i]), sizeof(ui32));
    }

	ofile.close();
	return 0;
}

