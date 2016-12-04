#ifndef TTextFileInfo_H_XXXXX
#define TTextFileInfo_H_XXXXX

#include "types.h"
#include "utils.h"

class TTextFileInfo{
public:	
	int CurrentLine;
	int TotalLine;
	char* Mode;//the open mode of the file
	TTextFileInfo(char * FileName, char * Mode);
	BOOLEAN Open();
	void Close();
	BOOLEAN ReadLn(char * Ln);
	void WriteLn(char * Ln);
	bool endOf();
private:
	FILE * F;
	char FileName[256];
};

#endif