#include "stdafx.h"

typedef struct _HOOK_INFO
{
	ULONG	Handle;
	ULONG	FuncOffset;
	ULONG	FuncBaseAddr;
	ULONG	iHook;
	ULONG	pid;
	ULONG	tid;
	CHAR	ProcessName[16];
} HOOK_INFO, *PHOOK_INFO;

typedef struct _MESSAGE_HOOK_INFO {
	ULONG ulCount;
	HOOK_INFO MessageHook[1];
} MESSAGE_HOOK_INFO, *PMESSAGE_HOOK_INFO;

PMESSAGE_HOOK_INFO MessageHookInfo;

CImageList MessageHookImg;//Í¼±ê

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);