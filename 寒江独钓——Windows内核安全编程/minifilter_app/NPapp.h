#pragma once

#include "windows.h"
#include <vector>
#include <string>
using namespace std;

typedef enum _NPMINI_COMMAND {
    ENUM_PASS = 0,
    ENUM_BLOCK
} NPMINI_COMMAND;

typedef struct _COMMAND_MESSAGE {
    NPMINI_COMMAND 	Command;  
} COMMAND_MESSAGE, *PCOMMAND_MESSAGE;

class CNPApp
{
public:
	CNPApp();
	virtual ~CNPApp();
	void NPMessage(COMMAND_MESSAGE data);

private:
	HINSTANCE m_hModule;
	bool LoadNPminifilterDll(void);	
};
