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

void FormatFile::strFirstCharUpperCase(char * ln)
{
	bool startOfAWord;
	char charBuf[2];
	charBuf[1] = NULL;
	int len = strlen(ln);
	startOfAWord = true;
	for (int i = 0; i < len; i++)
	{
		if (!startOfAWord && (ln[i] == ' '))
		{
			startOfAWord = true;
			continue;
		}
		else if (!startOfAWord && (ln[i] != ' '))
		{
			charBuf[0] = ln[i];
			strlwr(charBuf);
			ln[i] = charBuf[0];
		};

		if (startOfAWord && (ln[i] != ' '))
		{
			charBuf[0] = ln[i];
			strupr(charBuf);
			ln[i] = charBuf[0];
			startOfAWord = false;
		};
	};
}

void FormatFile::formatTheFile(char * fileName, int formatType)
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
			switch (formatType)
			{
			case FILE_FORMAT_UPPERCASE:
				strupr(ln);
				break;
			case FILE_FORMAT_LOWERCASE:
				strlwr(ln);
				break;
			case FILE_FORMAT_FIRST_UPPERCASE:
				strFirstCharUpperCase(ln);
				break;
			default:
				printf("error formattype\n");
			}
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
input_format:
			printf("please input format type: up, lw, fu\n");
			scanf("%[^\n]", cmd);
			fflush(stdin);
			int formatType;
			if (strcmp(cmd, FORMAT_UP) == 0)
			{
				formatType = FILE_FORMAT_UPPERCASE;
			}
			else if (strcmp(cmd, FORMAT_LW) == 0)
			{
				formatType = FILE_FORMAT_LOWERCASE;
			}
			else if (strcmp(cmd, FORMAT_FU) == 0)
			{
				formatType = FILE_FORMAT_FIRST_UPPERCASE;
			}
			else
			{
				printf("error format\n");
				goto input_format;
			}
			formatTheFile(arg, formatType);
		}
		else if (strcmp(cmd, "-q") == 0)
		{
			break;
		}
		else
			usage();
	}
}