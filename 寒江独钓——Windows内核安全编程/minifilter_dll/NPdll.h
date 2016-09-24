#include "windows.h"
#include <stdio.h>
#include <FltUser.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "fltLib.lib")
#pragma comment(lib, "fltMgr.lib")
#pragma comment(lib, "ntoskrnl.lib")
#pragma comment(lib, "hal.lib")

extern HANDLE g_hPort;

#define NPMINI_NAME            L"NPminifilter"
#define NPMINI_PORT_NAME       L"\\NPMiniPort"

__declspec(dllexport)	int InitialCommunicationPort(void);
__declspec(dllexport)   int NPSendMessage(PVOID InputBuffer);

typedef enum _NPMINI_COMMAND {
    ENUM_PASS = 0,
    ENUM_BLOCK
} NPMINI_COMMAND;

typedef struct _COMMAND_MESSAGE {
    NPMINI_COMMAND 	Command;  
} COMMAND_MESSAGE, *PCOMMAND_MESSAGE;
