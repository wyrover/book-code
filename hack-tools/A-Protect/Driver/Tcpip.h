#ifndef _TCPIP_H_
#define _TCPIP_H_

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

BOOL GetDriverEntryPoint(PVOID ImageBase,DWORD *pOutDriverEntry);
HANDLE MapFileAsSection(PUNICODE_STRING FileName,PVOID *ModuleBase);


//IRP_MJ_MAXIMUM_FUNCTION

typedef struct _TCPDISPATCH {

		ULONG ulNumber;
		ULONG ulTcpDispatch;
		ULONG ulCurrentTcpDispatch;
		CHAR lpszBaseModule[256];
		ULONG ulModuleSize;
		ULONG ulModuleBase;
		WCHAR lpwzTcpDispatchName[256];
		int Hooked;                         //0 no hook  1 fsd hook 2 fsd inline hook

} TCPDISPATCH, *PTCPDISPATCH;

typedef struct _TCPDISPATCHBAKUP {
	ULONG ulCount;
	TCPDISPATCH TcpDispatch[1];
} TCPDISPATCHBAKUP, *PTCPDISPATCHBAKUP;

PTCPDISPATCHBAKUP TcpDispatchBakUp;

ULONG ulTcpipModuleBase;
ULONG ulTcpipModuleSize;

ULONG ulReLoadTcpipModuleBase;

PDRIVER_OBJECT PTcpDriverObjectBakup;

ULONG ulReal_TCP_IRP_MJ_CREATE;
ULONG ulReal_TCP_IRP_MJ_CREATE_NAMED_PIPE;
ULONG ulReal_TCP_IRP_MJ_CLOSE;
ULONG ulReal_TCP_IRP_MJ_READ;
ULONG ulReal_TCP_IRP_MJ_WRITE;
ULONG ulReal_TCP_IRP_MJ_QUERY_INFORMATION;
ULONG ulReal_TCP_IRP_MJ_SET_INFORMATION;
ULONG ulReal_TCP_IRP_MJ_QUERY_EA;
ULONG ulReal_TCP_IRP_MJ_SET_EA;
ULONG ulReal_TCP_IRP_MJ_FLUSH_BUFFERS;
ULONG ulReal_TCP_IRP_MJ_QUERY_VOLUME_INFORMATION;
ULONG ulReal_TCP_IRP_MJ_SET_VOLUME_INFORMATION;
ULONG ulReal_TCP_IRP_MJ_DIRECTORY_CONTROL;
ULONG ulReal_TCP_IRP_MJ_FILE_SYSTEM_CONTROL;
ULONG ulReal_TCP_IRP_MJ_DEVICE_CONTROL;
ULONG ulReal_TCP_IRP_MJ_SHUTDOWN;
ULONG ulReal_TCP_IRP_MJ_LOCK_CONTROL;
ULONG ulReal_TCP_IRP_MJ_CLEANUP;
ULONG ulReal_TCP_IRP_MJ_CREATE_MAILSLOT;
ULONG ulReal_TCP_IRP_MJ_QUERY_SECURITY;
ULONG ulReal_TCP_IRP_MJ_SET_SECURITY;
ULONG ulReal_TCP_IRP_MJ_POWER;
ULONG ulReal_TCP_IRP_MJ_SYSTEM_CONTROL;
ULONG ulReal_TCP_IRP_MJ_DEVICE_CHANGE;
ULONG ulReal_TCP_IRP_MJ_QUERY_QUOTA;
ULONG ulReal_TCP_IRP_MJ_SET_QUOTA;
ULONG ulReal_TCP_IRP_MJ_PNP_POWER;

extern BOOL DebugOn;

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

unsigned long __fastcall GetFunctionCodeSize(
	void *Proc
	);

#endif