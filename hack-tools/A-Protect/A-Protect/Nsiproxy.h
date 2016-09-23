#include "stdafx.h"

#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

typedef struct _NSIPROXYDISPATCH {
	ULONG ulNumber;
	ULONG ulNsiproxyDispatch;
	ULONG ulCurrentNsiproxyDispatch;
	CHAR lpszBaseModule[256];
	ULONG ulModuleSize;
	ULONG ulModuleBase;
	WCHAR lpwzNsiproxyDispatchName[256];
	int Hooked;
} NSIPROXYDISPATCH, *PNSIPROXYDISPATCH;

typedef struct _NSIPROXYDISPATCHBAKUP {
	ULONG ulCount;
	NSIPROXYDISPATCH NsiproxyDispatch[1];
} NSIPROXYDISPATCHBAKUP, *PNSIPROXYDISPATCHBAKUP;

PNSIPROXYDISPATCHBAKUP NsiproxyDispatchBakUp;

CHAR* setClipboardText(CHAR* str);
BOOL IsWindows7();

CImageList NsiproxyImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);