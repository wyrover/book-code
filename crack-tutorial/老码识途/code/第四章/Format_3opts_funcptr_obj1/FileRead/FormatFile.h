#ifndef FormatFile_H_XXXXX
#define FormatFile_H_XXXXX

#include "TTextFileInfo.h"
#include <string.h>

#define FORMAT_UP "up"
#define FORMAT_LW "lw"
#define FORMAT_FU "fu"

typedef char* (*fpFormatLn)(char * ln);

char * strFirstCharUpperCase(char * ln);

class LnFormatter
{
public:
	fpFormatLn getHandler(){
		return m_handler;
	}
protected:
	fpFormatLn m_handler;
};

class UpperLnFormatter : public LnFormatter
{
public:
	UpperLnFormatter(){
		m_handler = strupr;
	}
};

class LowerLnFormatter : public LnFormatter
{
public:
	LowerLnFormatter(){
		m_handler = strlwr;
	}
};

class FiretCharUpperLnFormatter : public LnFormatter
{
public:
	FiretCharUpperLnFormatter(){
		m_handler = strFirstCharUpperCase;
	}
};

class FormatFile
{
public:
	FormatFile(void);
	~FormatFile(void);
	void doFormat();
private:
	void usage();
	void formatTheFile(char * fileName);	
	LnFormatter * m_formatHandler;
};

#endif