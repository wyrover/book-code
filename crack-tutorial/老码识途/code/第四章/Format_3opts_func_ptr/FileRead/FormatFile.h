#ifndef FormatFile_H_XXXXX
#define FormatFile_H_XXXXX

#include "TTextFileInfo.h"

#define FORMAT_UP "up"
#define FORMAT_LW "lw"
#define FORMAT_FU "fu"

typedef char* (*fpFormatLn)(char * ln);

class FormatFile
{
public:
	FormatFile(void);
	~FormatFile(void);
	void doFormat();
private:
	void usage();
	void formatTheFile(char * fileName);	
	fpFormatLn m_formatHandler;
};

#endif