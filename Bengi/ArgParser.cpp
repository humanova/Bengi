#include "ArgParser.h"

#pragma warning(disable: 4996)

void ParseArg(int argc, char* argv[], IOFile &iofile)
{
	/*
	for (int i = 0; i < argc; i++)
	{
		std::cout << argv[i] << std::endl;
	}*/

	if (argc < 2)
	{
		BengiError(Error::NO_INPUT_FILE);
		PrintUsage();
		exit(-1);
	}

	else if (argc == 2)
	{
		if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
		{
			PrintUsage();
			exit(1);
		}
		else if (!strcmp(argv[1], "-v") || !strcmp(argv[1], "--version"))
		{
			PrintVersion();
			exit(1);
		}
		else
		{
			iofile.sourcePath = argv[1];
			return;
		}
	}
	else
	{
		for (int i = 1; i < argc; i++)
		{
			if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
			{
				PrintUsage();
				exit(-1);
			}
			
			else if (!strcmp(argv[i], "-v") || !strcmp(argv[i], "--version"))
			{
				PrintVersion();
				exit(-1);
			}

			else if (!strcmp(argv[i], "-s"))
			{
				if (!(i + 1 < argc))
				{
					BengiError(NO_INPUT_FILE);
					exit(-1);
				}
				iofile.sourcePath = argv[i + 1];
				i++;
			}

			else if (!strcmp(argv[i], "-o") || !strcmp(argv[i], "--output"))
			{
				if (iofile.isOutputSet())
				{
					BengiError(OUTPUT_ALREADY_SET);
					exit(-1);
				}
				else if (!(i + 1 < argc))
				{
					BengiError(OUTPUT_NOT_SET);
					exit(-1);
				}
				iofile.outPath = argv[i + 1];
				i++;
			}

			else if (!strcmp(argv[i], "-c") || !strcmp(argv[i], "--compile"))
			{
				if (!(i + 1 < argc))
				{
					BengiError(BASM_NOT_SET);
					exit(-1);
				}
				char command[] = "basm ";
				char* comm;
				comm = strcat(command, argv[i + 1]);
				system(comm);
				i++;
				exit(1); // for now
			}

			else
			{
				//if not starts with '-', it may be source
				if (argv[i][0] != '-')
				{
					if (iofile.isSourceSet())
					{
						BengiError(SOURCE_ALREADY_SET);
						exit(-1);
					}
					else
					{
						iofile.sourcePath = argv[i + 1];
					}
				}
				else
				{
					BengiError(COMMAND_NOT_RECOGNIZED, argv[i]);
					exit(-1);
				}
			}
		}
	}
	return;
}