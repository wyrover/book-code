#include "FormatFile.h"
#include <string.h>
#include <stdio.h>

void* funcPtrTbUpperFormatter[2];

int UpperLnFormatter::initClass()
{
	funcPtrTbUpperFormatter[0] = strupr;
	funcPtrTbUpperFormatter[1] = UpperLnFormatter::getFormatNameFunc;
	return 0;
}

int gInitUpper = UpperLnFormatter::initClass();

UpperLnFormatter::UpperLnFormatter(){
		m_handlerTb = funcPtrTbUpperFormatter;
};

void * funcPtrTbLowerFormatter[2];

int LowerLnFormatter::initClass()
{
	funcPtrTbLowerFormatter[0] = strlwr;
	funcPtrTbLowerFormatter[1] = LowerLnFormatter::getFormatNameFunc;
	return 0;
}

int gInitLower = LowerLnFormatter::initClass();

LowerLnFormatter::LowerLnFormatter(){
		m_handlerTb = funcPtrTbLowerFormatter;			
};

void * funcPtrTbFirstCharUpperFormatter[2];

int FirstCharUpperLnFormatter::initClass()
{
	funcPtrTbFirstCharUpperFormatter[0] = strFirstCharUpperCase;
	funcPtrTbFirstCharUpperFormatter[1] = FirstCharUpperLnFormatter::getFormatNameFunc;
	return 0;
}

int gInitfirstUpper = FirstCharUpperLnFormatter::initClass();

FirstCharUpperLnFormatter::FirstCharUpperLnFormatter(){
		m_handlerTb = funcPtrTbFirstCharUpperFormatter;			
};

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
			fpFormatLn pFormat = (fpFormatLn)(m_formatHandler->getHandlerTb()[0]);
			pFormat(ln);
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
			fpGetFormatName pGetName = (fpGetFormatName)(m_formatHandler->getHandlerTb()[1]);
			printf("current format is : %s\n", pGetName());
		}
		else if (strcmp(cmd, "-q") == 0)
		{
			break;
		}
		else
			usage();
	}
}