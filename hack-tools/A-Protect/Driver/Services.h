#ifndef _SERVICES_H_
#define _SERVICES_H_

#include "ntifs.h"
#include <stdio.h>
#include <wchar.h>
#include "Protect.h"
#include "ntos.h"

typedef struct _SERVICESREGISTRY_INFORMATION {          //SERVICESREGISTRY_INFORMATION
	WCHAR lpwzSrvName[256];
	WCHAR lpwzImageName[256];
	WCHAR lpwzDLLPath[256];
	//WCHAR lpwzDescription[256];
	//ULONG BootType;
	//ULONG RunStatus;     //ÔËÐÐ×´Ì¬

} SERVICESREGISTRY_INFORMATION, *PSERVICESREGISTRY_INFORMATION;

typedef struct _SERVICESREGISTRY {          //SERVICESREGISTRY
	ULONG ulCount;
	SERVICESREGISTRY_INFORMATION SrvReg[1];

} SERVICESREGISTRY, *PSERVICESREGISTRY;

PSERVICESREGISTRY ServicesRegistry;
PSERVICESREGISTRY DepthServicesRegistry;

extern BOOL DebugOn;
extern BOOL bDisResetSrv;

typedef NTSTATUS (__stdcall* ZWSETVALUEKEY)(
	__in      HANDLE KeyHandle,
	__in      PUNICODE_STRING ValueName,
	__in_opt  ULONG TitleIndex,
	__in      ULONG Type,
	__in_opt  PVOID Data,
	__in      ULONG DataSize
	);

int ZwSetValueKeyIndex;


NTSTATUS QueryServicesRegistry(PSERVICESREGISTRY ServicesRegistry);
NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size);

BOOL SystemCallEntryTableHook(
	PUNICODE_STRING FunctionName,
	int *Index,
	DWORD NewFuctionAddress
	);

NTSTATUS SafeQueryNameString(
	IN PVOID FileObject,
	OUT POBJECT_NAME_INFORMATION *FileNameInfo
	);

ULONG GetInheritedProcessPid(PEPROCESS Eprocess);

KPROCESSOR_MODE KeGetPreviousMode();

#endif