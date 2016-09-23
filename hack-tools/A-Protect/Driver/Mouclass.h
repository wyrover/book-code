#ifndef _MOUCLASS_H_
#define _MOUCLASS_H_

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

ULONG ulMouclassModuleBase;
ULONG ulMouclassModuleSize;

ULONG ulReLoadMouclassModuleBase;

extern BOOL DebugOn;

PDRIVER_OBJECT PMouclassDriverObjectBakup;

ULONG ulReal_MOUCLASS_IRP_MJ_CREATE;
ULONG ulReal_MOUCLASS_IRP_MJ_CLOSE;
ULONG ulReal_MOUCLASS_IRP_MJ_READ;
ULONG ulReal_MOUCLASS_IRP_MJ_FLUSH_BUFFERS;
ULONG ulReal_MOUCLASS_IRP_MJ_DEVICE_CONTROL;
ULONG ulReal_MOUCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL;
ULONG ulReal_MOUCLASS_IRP_MJ_CLEANUP;
ULONG ulReal_MOUCLASS_IRP_MJ_POWER;
ULONG ulReal_MOUCLASS_IRP_MJ_SYSTEM_CONTROL;
ULONG ulReal_MOUCLASS_IRP_MJ_PNP_POWER;

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