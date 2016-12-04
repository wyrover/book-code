#include "TTextFileInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

bool TTextFileInfo::endOf()
{
	return feof(F) == 0 ? false : true;
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
