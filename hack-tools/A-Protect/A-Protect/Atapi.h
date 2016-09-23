#include "stdafx.h"

#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

//IRP_MJ_MAXIMUM_FUNCTION

typedef struct _ATAPIDISPATCH {
	ULONG ulNumber;
	ULONG ulAtapiDispatch;
	ULONG ulCurrentAtapiDispatch;
	CHAR lpszBaseModule[256];
	ULONG ulModuleSize;
	ULONG ulModuleBase;
	WCHAR lpwzAtapiDispatchName[256];
	int Hooked;
} ATAPIDISPATCH, *PATAPIDISPATCH;

typedef struct _ATAPIDISPATCHBAKUP {
	ULONG ulCount;
	ATAPIDISPATCH AtapiDispatch[1];
} ATAPIDISPATCHBAKUP, *PATAPIDISPATCHBAKUP;

PATAPIDISPATCHBAKUP AtapiDispatchBakUp;

CHAR* setClipboardText(CHAR* str);

CImageList AtapiImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);