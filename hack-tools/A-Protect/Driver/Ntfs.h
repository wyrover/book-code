#ifndef _NTFS_H_
#define _NTFS_H_

#include "ntifs.h"
#include <ntimage.h>
#include "ntos.h"
#include "InitWindowsVersion.h"
//IRP_MJ_MAXIMUM_FUNCTION

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

PDRIVER_OBJECT PNtfsDriverObjectBakup;

ULONG ulReLoadNtfsModuleBase;

ULONG ulNtfsModuleBase;
ULONG ulNtfsModuleSize;

ULONG ulReal_IRP_MJ_CREATE;
ULONG ulReal_IRP_MJ_CLOSE;
ULONG ulReal_IRP_MJ_READ;
ULONG ulReal_IRP_MJ_WRITE;
ULONG ulReal_IRP_MJ_QUERY_INFORMATION;
ULONG ulReal_IRP_MJ_SET_INFORMATION;
ULONG ulReal_IRP_MJ_QUERY_EA;
ULONG ulReal_IRP_MJ_SET_EA;
ULONG ulReal_IRP_MJ_FLUSH_BUFFERS;
ULONG ulReal_IRP_MJ_QUERY_VOLUME_INFORMATION;
ULONG ulReal_IRP_MJ_SET_VOLUME_INFORMATION;
ULONG ulReal_IRP_MJ_DIRECTORY_CONTROL;
ULONG ulReal_IRP_MJ_FILE_SYSTEM_CONTROL;
ULONG ulReal_IRP_MJ_DEVICE_CONTROL;
ULONG ulReal_IRP_MJ_SHUTDOWN;
ULONG ulReal_IRP_MJ_LOCK_CONTROL;
ULONG ulReal_IRP_MJ_CLEANUP;
ULONG ulReal_IRP_MJ_QUERY_SECURITY;
ULONG ulReal_IRP_MJ_SET_SECURITY;
ULONG ulReal_IRP_MJ_QUERY_QUOTA;
ULONG ulReal_IRP_MJ_SET_QUOTA;
ULONG ulReal_IRP_MJ_PNP_POWER;


extern BOOL DebugOn;

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