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

char * strFirstCharUpperCase(char * ln)
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
	return ln;
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
			m_formatHandler(ln);
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
			
			if (strcmp(cmd, FORMAT_UP) == 0)
			{
				m_formatHandler = strupr;
			}
			else if (strcmp(cmd, FORMAT_LW) == 0)
			{
				m_formatHandler = strlwr;
			}
			else if (strcmp(cmd, FORMAT_FU) == 0)
			{
				m_formatHandler = strFirstCharUpperCase;
			}
			else
			{
				printf("error format\n");
				goto input_format;
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