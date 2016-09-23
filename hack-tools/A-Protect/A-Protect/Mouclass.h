#include "stdafx.h"

#define IRP_MJ_MAXIMUM_FUNCTION         0x1b

//IRP_MJ_MAXIMUM_FUNCTION

typedef struct _MOUCLASSDISPATCH {
	ULONG ulNumber;
	ULONG ulMouclassDispatch;
	ULONG ulCurrentMouclassDispatch;
	CHAR lpszBaseModule[256];
	ULONG ulModuleSize;
	ULONG ulModuleBase;
	WCHAR lpwzMouclassDispatchName[256];
	int Hooked;
} MOUCLASSDISPATCH, *PMOUCLASSDISPATCH;

typedef struct _MOUCLASSDISPATCHBAKUP {
	ULONG ulCount;
	MOUCLASSDISPATCH MouclassDispatch[1];
} MOUCLASSDISPATCHBAKUP, *PMOUCLASSDISPATCHBAKUP;

PMOUCLASSDISPATCHBAKUP MouclassDispatchBakUp;

CHAR* setClipboardText(CHAR* str);

CImageList MouclassImg;

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);