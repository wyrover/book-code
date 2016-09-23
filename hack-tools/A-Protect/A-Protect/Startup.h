#include "stdafx.h"


typedef struct _STARTUP_INFORMATION {

	WCHAR lpwzName[256];
	WCHAR lpwzKeyPath[256];
	WCHAR lpwzKeyValue[256];

} STARTUP_INFORMATION, *PSTARTUP_INFORMATION;

typedef struct _STARTUP_INFO {          //InlineHook
	ULONG ulCount;
	STARTUP_INFORMATION Startup[1];
} STARTUP_INFO, *PSTARTUP_INFO;

PSTARTUP_INFO StartupInfo;

CImageList StartupImg;//Í¼±ê

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);