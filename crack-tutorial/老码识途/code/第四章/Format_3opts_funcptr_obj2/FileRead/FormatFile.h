#ifndef FormatFile_H_XXXXX
#define FormatFile_H_XXXXX

#include "TTextFileInfo.h"
#include <string.h>

#define FORMAT_UP "up"
#define FORMAT_LW "lw"
#define FORMAT_FU "fu"

typedef char* (*fpFormatLn)(char * ln);

typedef char * (*fpGetFormatName)();

char * strFirstCharUpperCase(char * ln);

class LnFormatter
{
public:
	fpFormatLn getHandler(){
		return m_handler;
	}
	fpGetFormatName getFormatNameFunc(){
		return m_getFormatName;
	}
protected:
	fpFormatLn m_handler;
	fpGetFormatName m_getFormatName;
};

class UpperLnFormatter : public LnFormatter
{
private:
	static char * getFormatNameFunc(){
		return "uppercase format";
	}
public:
	UpperLnFormatter(){
		m_handler = strupr;
		m_getFormatName = getFormatNameFunc;
	}
};

class LowerLnFormatter : public LnFormatter
{
private:
	static char * getFormatNameFunc(){
		return "lowercase format";
	}

public:
	LowerLnFormatter(){
		m_handler = strlwr;
		m_getFormatName = getFormatNameFunc;
	}
};

class FirstCharUpperLnFormatter : public LnFormatter
{
private:
	static char * getFormatNameFunc(){
		return "first char uppercase format";
	}
public:
	FirstCharUpperLnFormatter(){
		m_handler = strFirstCharUpperCase;
		m_getFormatName = getFormatNameFunc;
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