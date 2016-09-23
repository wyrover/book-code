#ifndef _SSDT_H_
#define _SSDT_H_

#include "ntifs.h"
#include <ntimage.h>
#include "ntos.h"
#include "ldasm.h"
#include "libdasm.h"

extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;
extern ULONG SystemKernelModuleSize;
extern DWORD OriginalKiServiceTable;


extern BOOL DebugOn;
extern BOOL bKrnlPDBSuccess;
//---------------------------------------------------------------------------------------
//SSDT
//---------------------------------------------------------------------------------------
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

//---------------------------------------------------------------------------------------

extern BOOL bSSDTAll;
//int SSDTIndex;

ULONG GetSystemRoutineAddress(
	int IntType,
	PVOID lpwzFunction
	);

unsigned long __fastcall GetFunctionCodeSize(
	void *Proc
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

BOOL GetFunctionNameByIndex(
	ULONG ulNtDllModuleBase,
	int *Index,
	CHAR *lpszFunctionName
	);

HANDLE MapFileAsSection(
	PUNICODE_STRING FileName,
	PVOID *ModuleBase
	);

#endif