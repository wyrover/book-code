#include "stdafx.h"

#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

//IRP_MJ_MAXIMUM_FUNCTION

typedef struct _KBDCLASSDISPATCH {
	ULONG ulNumber;
	ULONG ulKbdclassDispatch;
	ULONG ulCurrentKbdclassDispatch;
	CHAR lpszBaseModule[256];
	ULONG ulModuleSize;
	ULONG ulModuleBase;
	WCHAR lpwzKbdclassDispatchName[256];
	int Hooked;
} KBDCLASSDISPATCH, *PKBDCLASSDISPATCH;

typedef struct _KBDCLASSDISPATCHBAKUP {
	ULONG ulCount;
	KBDCLASSDISPATCH KbdclassDispatch[1];
} KBDCLASSDISPATCHBAKUP, *PKBDCLASSDISPATCHBAKUP;

PKBDCLASSDISPATCHBAKUP KbdclassDispatchBakUp;

CHAR* setClipboardText(CHAR* str);

CImageList KdbclassImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);