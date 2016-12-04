// FileRead.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BOOLEAN int
#define TRUE 1
#define FALSE 0

int readLn(FILE * fp, char * ln)
{
	int c, ret;
	ret = 0;char * first = ln;
	while (1)
	{
		c = fgetc(fp);
		if (c == EOF)
		{
			*ln = 0;
			return ret;
		}
		*ln = (char)c;
		ln++;
		ret++;

		if (c == '\n')
		{
			if ((first < (ln -1)) && (*(ln - 2) == '\r'))
			{   
				*(ln - 2) = 0;
				ret--;
			}
			else
				*(ln - 1) = 0;
			ret--;
			return ret;
		}
	}
	return ret;
}

struct TTextFileInfo{
	FILE * F;
	char FileName[256];
	int CurrentLine;
	int TotalLine;
	char* Mode;//the open mode of the file
};

typedef TTextFileInfo * PTextFileInfo ;

void usage()
{
	printf("please input file path like this: fileread c:\temp \n");
}

int GetLineCount(char *FileName)
{
  FILE * f;
  char Ln[256];
  int cnt = 0;
  f = fopen(FileName, "r");  
  if (f == NULL)
  {
	  return 0;
  }
  
  while (! feof(f))
  {
	  readLn(f, Ln);
      cnt++;
  }
  fclose(f);   
  return cnt;
}

void fatalErr(char * msg)
{
	printf("%s\n", msg);
	exit(1);
}

BOOLEAN OpenTextFile(PTextFileInfo pInfo)
{	
	if (0 == strcmp(pInfo->Mode, "r"))
	{
		pInfo->TotalLine = GetLineCount(pInfo->FileName);
		pInfo->CurrentLine = 0;
	}
	else if (0 == strcmp(pInfo->Mode, "w"))
	{
		pInfo->TotalLine = 0;
		pInfo->CurrentLine = 0;
	}
	else if (0 == strcmp(pInfo->Mode, "w+"))
	{
		pInfo->CurrentLine = GetLineCount(pInfo->FileName);
		pInfo->TotalLine = pInfo->CurrentLine;		
	}
	else
	{
		fatalErr("just support r, w, and w+");
	}
	pInfo->F = fopen(pInfo->FileName, pInfo->Mode);		

	return pInfo->F == NULL ? FALSE : TRUE;
}

BOOLEAN ReadLine(PTextFileInfo pInfo, char *Ln)
{
	if (feof(pInfo->F))
      return FALSE;
    readLn(pInfo->F, Ln);
	pInfo->CurrentLine++;
	return TRUE;
}

void WriteLine(PTextFileInfo pInfo, char * Ln)
{
	fwrite(Ln, strlen(Ln), 1, pInfo->F);
	fwrite("\r\n", 2, 1, pInfo->F);
	pInfo->CurrentLine++;
	pInfo->TotalLine = pInfo->CurrentLine;
}

void CloseTextFile(PTextFileInfo pInfo)
{
	fclose(pInfo->F);
}


void main(int argc, char* argv[])
{
	if (argc != 2)
	{
		usage();
		return;
	}
	TTextFileInfo info;
	strcpy(info.FileName, argv[1]);
	info.Mode = "r";
	if (FALSE == OpenTextFile(&info))
	{
		printf("cannot open file %s\n", argv[1]);
		return;
	}
    char Ln[512];
	while (TRUE == ReadLine(&info, Ln))
		printf("%s \n", Ln);
	printf("----------------- file: %s -----------------\n", info.FileName);
	printf("----------------- total line count=%d -----------------\n", info.TotalLine);
	CloseTextFile(&info);
	getchar();
}

