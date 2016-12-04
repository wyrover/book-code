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
	void** getHandlerTb(){
		return m_handlerTb;
	}
protected:
	void ** m_handlerTb;
};

class UpperLnFormatter : public LnFormatter
{
private:
	static char * getFormatNameFunc(){
		return "uppercase format";
	}	
public:
	UpperLnFormatter();
	static int initClass();
};

class LowerLnFormatter : public LnFormatter
{
private:
	static char * getFormatNameFunc(){
		return "lowercase format";
	}	
public:
	LowerLnFormatter();
	static int initClass();
};

class FirstCharUpperLnFormatter : public LnFormatter
{
private:
	static char * getFormatNameFunc(){
		return "first char uppercase format";
	}	
public:
	FirstCharUpperLnFormatter();
	static int initClass();
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