#ifndef CXX_PROCESS_MEMORY_H
#define CXX_PROCESS_MEMORY_H

#include <ntifs.h>
#include "SSDT.h"
#include "Function.h"

typedef struct _PROCESS_MEMORY_ENTRY_INFOR_
{
	ULONG_PTR ulBase;
	ULONG_PTR ulSize;
	ULONG ulProtect;   
	ULONG ulState;
	ULONG ulType;
}PROCESS_MEMORY_ENTRY_INFOR, *PPROCESS_MEMORY_ENTRY_INFOR;

typedef struct _PROCESS_MEMORY_INFORMATION_
{
	ULONG_PTR             NumberOfMemorys;
	PROCESS_MEMORY_ENTRY_INFOR Memorys[1];
}PROCESS_MEMORY_INFORMATION, *PPROCESS_MEMORY_INFORMATION;


typedef struct _PROCESS_MEMORY_BASIC_INFORMATION {
	PVOID    BaseAddress;
	PVOID    AllocationBase;
	ULONG  AllocationProtect;
	SIZE_T    RegionSize;
	ULONG  State;
	ULONG  Protect;
	ULONG  Type;
}PROCESS_MEMORY_BASIC_INFORMATION,*PPROCESS_MEMORY_BASIC_INFORMATION;


typedef
	NTSTATUS
	(*pfnNtQueryVirtualMemory)(HANDLE ProcessHandle,PVOID BaseAddress,MEMORY_INFORMATION_CLASS MemoryInformationClass,
	PVOID MemoryInformation,
	SIZE_T MemoryInformationLength,
	PSIZE_T ReturnLength);

//获得进程内信息
NTSTATUS EnumProcessMemory(ULONG ulProcessID,PVOID OutBuffer,ULONG_PTR ulOutSize);
NTSTATUS GetProcessMemorys(PEPROCESS EProcess, PPROCESS_MEMORY_INFORMATION OutBuffer, ULONG_PTR ulCount);

//修改内存属性
typedef struct _MODIFY_MEMORY 
{
	ULONG       ulProcessID;
	ULONG_PTR   ulBase;
	ULONG_PTR   ulSize;
	ULONG       ulNewProtect;
}MODIFY_MEMORY,*PMODIFY_MEMORY;


typedef
	NTSTATUS
	(*pfnNtProtectVirtualMemory)(
	 HANDLE ProcessHandle,
	 PVOID *BaseAddress,
	 PSIZE_T RegionSize,
	 ULONG NewProtect,
	 PULONG OldProtect
	);

NTSTATUS ModifyMemoryProtect(PVOID InBuffer, ULONG InSize);

//读取进程内存
typedef struct _READ_MEMORY_    
{
	ULONG_PTR  ulBase;
	ULONG_PTR  ulSize;
	ULONG      ulProcessID;
}READ_MEMORY,*PREAD_MEMORY;

NTSTATUS ReadProcessMemory(PVOID InBuffer, PVOID OutBuffer);
NTSTATUS ReadRing3Memory(ULONG_PTR ulBase,ULONG ulPid,ULONG ulSize,PVOID OutBuffer);

#endif