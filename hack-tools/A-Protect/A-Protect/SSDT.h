#include "StdAfx.h"

typedef struct _SSDT_INFORMATION {          //SSDT_INFORMATION
	ULONG ulNumber;
	ULONG ulMemoryFunctionBase;
	ULONG ulRealFunctionBase;
	CHAR lpszFunction[256];
	CHAR lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	int  IntHookType;                     //
} SSDT_INFORMATION, *PSSDT_INFORMATION;
typedef struct _SSDTINFO {          //SSDT
	ULONG ulCount;
	SSDT_INFORMATION SSDT[1];
} SSDTINFO, *PSSDTINFO;

PSSDTINFO SSDTInfo;

char lpOpenUrl[260];

CHAR* setClipboardText(CHAR* str);

CImageList SSDTImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);
extern void RunAProcess(char *comline);