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


class TTextFileInfo{
public:
	FILE * F;
	char FileName[256];
	int CurrentLine;
	int TotalLine;
	char* Mode;//the open mode of the file
	TTextFileInfo(char * FileName, char * Mode);
    BOOLEAN Open();
	void Close();
	BOOLEAN ReadLn(char * Ln);
    void WriteLn(char * Ln);
};



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

BOOLEAN TTextFileInfo::Open()
{	
	if (0 == strcmp(Mode, "r"))
	{
		TotalLine = GetLineCount(FileName);
		CurrentLine = 0;
	}
	else if (0 == strcmp(Mode, "w"))
	{
		TotalLine = 0;
		CurrentLine = 0;
	}
	else if (0 == strcmp(Mode, "w+"))
	{
		CurrentLine = GetLineCount(FileName);
		TotalLine = CurrentLine;		
	}
	else
	{
		fatalErr("just support r, w, and w+");
	}
	F = fopen(FileName, Mode);		

	return F == NULL ? FALSE : TRUE;
}

BOOLEAN TTextFileInfo::ReadLn(char *Ln)
{
	if (feof(F))
      return FALSE;
    readLn(F, Ln);
	CurrentLine++;
	return TRUE;
}

void TTextFileInfo::WriteLn(char * Ln)
{
	fwrite(Ln, strlen(Ln), 1, F);
	fwrite("\r\n", 2, 1, F);
	CurrentLine++;
	TotalLine = CurrentLine;
}

void TTextFileInfo::Close()
{
	fclose(F);
}


TTextFileInfo::TTextFileInfo(char * FileName, char * Mode)
{
	strcpy(this->FileName, FileName);
	this->Mode = Mode;
}


void main(int argc, char* argv[])
{
	if (argc != 2)
	{
		usage();
		return;
	}
	TTextFileInfo info(argv[1], "r");
	
	if (FALSE == info.Open())
	{
		printf("cannot open file %s\n", argv[1]);
		return;
	}
    char Ln[512];
	while (TRUE == info.ReadLn(Ln))
	  printf("%s \n", Ln);
	info.Close();
	printf("----------------- file: %s -----------------\n", info.FileName);
	printf("----------------- total line count=%d -----------------\n", info.TotalLine);
	getchar();
}

