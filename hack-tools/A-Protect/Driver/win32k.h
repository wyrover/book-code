#ifndef _WIN32K_H_
#define _WIN32K_H_

#include "ntifs.h"
#include <ntimage.h>
#include "ldasm.h"
#include "ShadowSSDT.h"

extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;
extern BOOL DebugOn;
extern PDRIVER_OBJECT PDriverObject;

PSERVICE_DESCRIPTOR_TABLE OriginalShadowServiceDescriptorTable;
PSERVICE_DESCRIPTOR_TABLE Safe_ServiceDescriptorShadowSSDTTable;

BYTE *Win32kImageModuleBase;
ULONG ulWin32kBase;
ULONG ulWin32kSize;

PSERVICE_DESCRIPTOR_TABLE ShadowSSDTTable;

ULONG ShadowTable;


PVOID LookupKernelModuleByName(PDRIVER_OBJECT DriverObject,char *KernelModuleName,DWORD *ulWin32kSize);

NTSTATUS LookupProcessByName(
	IN PCHAR pcProcessName,
	OUT PEPROCESS *pEprocess
	);

BOOL PeLoad(
	WCHAR *FileFullPath,
	BYTE **ImageModeleBase,
	PDRIVER_OBJECT DeviceObject,
	DWORD ExistImageBase
	);

PVOID
	MiFindExportedRoutine (
	IN PVOID DllBase,
	BOOL ByName,
	IN char *RoutineName,
	DWORD Ordinal
	);

ULONG GetSystemRoutineAddress(
	int IntType,
	PVOID lpwzFunction
	);

BOOLEAN
	KeAddSystemServiceTable(
	IN  PVOID   ServiceTableBase,
	IN  PVOID   ServiceCounterTableBase,
	IN  ULONG   NumberOfService,
	IN  PVOID   ParamTableBase,
	IN  ULONG   InsertServiceTableIndex
	);

UINT AlignSize(UINT nSize, UINT nAlign);


PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID ImageBase);

VOID InitShadowSSDTHook();

#endif