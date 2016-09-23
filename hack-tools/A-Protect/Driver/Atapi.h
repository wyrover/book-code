#ifndef _ATAPI_H_
#define _ATAPI_H_

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

ULONG ulAtapiModuleBase;
ULONG ulAtapiModuleSize;

ULONG ulReLoadAtapiModuleBase;

extern BOOL DebugOn;

PDRIVER_OBJECT PAtapiDriverObjectBakup;

ULONG ulReal_ATAPI_IRP_MJ_CREATE;
ULONG ulReal_ATAPI_IRP_MJ_CLOSE;

ULONG ulReal_ATAPI_IRP_MJ_DEVICE_CONTROL;
ULONG ulReal_ATAPI_IRP_MJ_INTERNAL_DEVICE_CONTROL;

ULONG ulReal_ATAPI_IRP_MJ_POWER;
ULONG ulReal_ATAPI_IRP_MJ_SYSTEM_CONTROL;

ULONG ulReal_ATAPI_IRP_MJ_PNP_POWER;
ULONG ulReal_DriverStartIo;


ULONG IRP_DRIVER_START_IO;

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