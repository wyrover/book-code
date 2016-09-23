#include "stdafx.h"

#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

typedef struct _NTFSDISPATCH {
	ULONG ulNumber;
	ULONG ulNtfsDispatch;
	ULONG ulCurrentNtfsDispatch;
	CHAR lpszBaseModule[256];
	ULONG ulModuleSize;
	ULONG ulModuleBase;
	WCHAR lpwzNtfsDispatchName[256];
	int Hooked;                         //0 no hook  1 fsd hook 2 fsd inline hook
} NTFSDISPATCH, *PNTFSDISPATCH;

typedef struct NTFSDISPATCHBAKU {
	ULONG ulCount;
	NTFSDISPATCH NtfsDispatch[1];
} NTFSDISPATCHBAKU, *PNTFSDISPATCHBAKUP;

PNTFSDISPATCHBAKUP NtfsDispatchBakUp;

CHAR* setClipboardText(CHAR* str);

CImageList NtfsImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);