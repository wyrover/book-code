#ifndef _KBDCLASS_H_
#define _KBDCLASS_H_

#include "ntifs.h"
#include <ntimage.h>
#include "InitWindowsVersion.h"
#include "ntos.h"

BOOL PeLoad(
	WCHAR *FileFullPath,
	BYTE **ImageModeleBase,
	PDRIVER_OBJECT DeviceObject,
	DWORD ExistImageBase
	);

NTSTATUS GetDriverObject(
	WCHAR *lpwzDevice,
	PDRIVER_OBJECT *PDriverObject
	);

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

ULONG ulKbdclassModuleBase;
ULONG ulKbdclassModuleSize;

ULONG ulReLoadKbdclassModuleBase;

extern BOOL DebugOn;

PDRIVER_OBJECT PKbdclassDriverObjectBakup;

ULONG ulReal_KBDCLASS_IRP_MJ_CREATE;
ULONG ulReal_KBDCLASS_IRP_MJ_CLOSE;
ULONG ulReal_KBDCLASS_IRP_MJ_READ;
ULONG ulReal_KBDCLASS_IRP_MJ_FLUSH_BUFFERS;
ULONG ulReal_KBDCLASS_IRP_MJ_DEVICE_CONTROL;
ULONG ulReal_KBDCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL;
ULONG ulReal_KBDCLASS_IRP_MJ_CLEANUP;
ULONG ulReal_KBDCLASS_IRP_MJ_POWER;
ULONG ulReal_KBDCLASS_IRP_MJ_SYSTEM_CONTROL;
ULONG ulReal_KBDCLASS_IRP_MJ_PNP_POWER;

BOOL PeLoad(
	WCHAR *FileFullPath,
	BYTE **ImageModeleBase,
	PDRIVER_OBJECT DeviceObject,
	DWORD ExistImageBase
	);

NTSTATUS GetDriverObject(
	WCHAR *lpwzDevice,
	PDRIVER_OBJECT *PDriverObject
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

unsigned long __fastcall GetFunctionCodeSize(
	void *Proc
	);

PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID ImageBase);

HANDLE MapFileAsSection(PUNICODE_STRING FileName,PVOID *ModuleBase);

BOOL GetDriverEntryPoint(PVOID ImageBase,DWORD *pOutDriverEntry);

#endif