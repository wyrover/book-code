#include "FormatFile.h"
#include <string.h>
#include <stdio.h>

FormatFile::FormatFile(void)
{
	m_formatHandler = NULL;	
}

FormatFile::~FormatFile(void)
{
	if (m_formatHandler != NULL)
	{
		delete m_formatHandler;
	}	
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
	
	fpFormatLn handler = m_formatHandler->getHandler();
	while (!file.endOf())
	{
		if (file.ReadLn(ln) > 0)
		{
			handler(ln);
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
				m_formatHandler = new UpperLnFormatter();
			}
			else if (strcmp(cmd, FORMAT_LW) == 0)
			{
				m_formatHandler = new LowerLnFormatter();
			}
			else if (strcmp(cmd, FORMAT_FU) == 0)
			{
				m_formatHandler = new FirstCharUpperLnFormatter();
			}
			else
			{
				printf("error format\n");
				goto input_format;
			}
			formatTheFile(arg);
		}
		else if (strcmp(cmd, "-v") == 0)
		{
			if (m_formatHandler == NULL)
			{
				printf("now no handler is set\n");
				continue;
			}
			printf("current format is : %s\n", m_formatHandler->getFormatNameFunc()());
		}
		else if (strcmp(cmd, "-q") == 0)
		{
			break;
		}
		else
			usage();
	}
}