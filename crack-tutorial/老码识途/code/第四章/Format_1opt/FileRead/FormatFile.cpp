#include "FormatFile.h"
#include <string.h>
#include <stdio.h>

FormatFile::FormatFile(void)
{
}

FormatFile::~FormatFile(void)
{
}

void FormatFile::usage()
{
	printf("format a file: -f filename\n");
	printf("quit: -q\n");
}

void FormatFile::formatTheFile(char * fileName)
{
	char ln[1024];
	TTextFileInfo file(fileName, "r");
	if (file.Open() == FALSE)
	{
		printf("cannot open the file\n");
		return;
	}

	while (!file.endOf())
	{
		if (file.ReadLn(ln) > 0)
		{
			strupr(ln);
			printf("%s\n", ln);
		}		
	}
	file.Close();			
}

void FormatFile::doFormat()
{
	char cmd[128];
	char * arg;

	while (1)
	{
		printf("please input cmd:");
		scanf("%[^\n]", cmd);
		fflush(stdin);
		char oldC = cmd[2];
		cmd[2] = 0;
		if (strcmp(cmd, "-f") == 0)
		{
			cmd[2] = oldC;
			arg = getArg(cmd + 2);
			if (arg == NULL)
			{
				printf("miss file arg\n");
				usage();
				continue;
			}
			formatTheFile(arg);
		}
		else if (strcmp(cmd, "-q") == 0)
		{
			break;
		}
		else
			usage();
	}
}