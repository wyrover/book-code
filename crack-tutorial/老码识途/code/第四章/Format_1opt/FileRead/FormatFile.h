#ifndef FormatFile_H_XXXXX
#define FormatFile_H_XXXXX

#include "TTextFileInfo.h"

#define FILE_FORMAT_UPPERCASE 0
#define FILE_FORMAT_LOWERCASE 1

#define FORMAT_UP "up"
#define FORMAT_LW "lw"

class FormatFile
{
public:
	FormatFile(void);
	~FormatFile(void);
	void doFormat();
private:
	void usage();
	void formatTheFile(char * fileName);
};

#endif