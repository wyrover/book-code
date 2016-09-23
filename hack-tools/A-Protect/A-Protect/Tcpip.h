#include "stdafx.h"

#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

typedef struct _TCPDISPATCH {

	ULONG ulNumber;
	ULONG ulTcpDispatch;
	ULONG ulCurrentTcpDispatch;
	CHAR lpszBaseModule[256];
	ULONG ulModuleSize;
	ULONG ulModuleBase;
	WCHAR lpwzTcpDispatchName[256];
	int Hooked;                         //0 no hook  1 fsd hook 2 fsd inline hook

} TCPDISPATCH, *PTCPDISPATCH;

typedef struct _TCPDISPATCHBAKUP {
	ULONG ulCount;
	TCPDISPATCH TcpDispatch[1];
} TCPDISPATCHBAKUP, *PTCPDISPATCHBAKUP;

PTCPDISPATCHBAKUP TcpDispatchBakUp;

CHAR* setClipboardText(CHAR* str);
BOOL IsWindows7();

CImageList TcpipImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);