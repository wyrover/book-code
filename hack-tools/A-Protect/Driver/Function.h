#ifndef _FUNCTION_H_
#define _FUNCTION_H_

#include "ntifs.h"
#include "ntos.h"

extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;

#define SystemModuleInformation 11

#define SEC_IMAGE         0x1000000
#define MEM_IMAGE SEC_IMAGE 

extern BOOL DebugOn;

typedef enum _MEMORY_INFORMATION_CLASS
{
	MemoryBasicInformation,
	MemoryWorkingSetList,
	MemorySectionName
}MEMORY_INFORMATION_CLASS;

typedef struct _MEMORY_BASIC_INFORMATION { 
	PVOID BaseAddress; 
	PVOID AllocationBase; 
	DWORD AllocationProtect; 
	DWORD RegionSize; 
	DWORD State; 
	DWORD Protect; 
	DWORD Type;
} MEMORY_BASIC_INFORMATION,*PMEMORY_BASIC_INFORMATION; 

extern POBJECT_TYPE *PsProcessType;


NTSTATUS ZwOpenProcess(
	__out     PHANDLE ProcessHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PCLIENT_ID ClientId
	);

NTSTATUS KillProcess(
	ULONG ulEprocess
	);

NTSTATUS SafeCopyMemory(
	PVOID SrcAddr, 
	PVOID DstAddr,
	ULONG Size
	);

BOOLEAN ValidateUnicodeString(
	PUNICODE_STRING usStr
	);

//ULONG KeGetPreviousMode();
BOOL MmIsAddressRangeValid(
	IN PVOID Address,
	IN ULONG Size
	);

BOOL ZeroProcessMemory(ULONG eprocess);

#endif