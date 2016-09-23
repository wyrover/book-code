#ifndef CXX_PROCESS_MODULE_H
#define CXX_PROCESS_MODULE_H

#include <ntifs.h>
#include"Function.h"
#include"SSDT.h"


#define _MAX_OBJECT_NAME   1024/sizeof(WCHAR) 
#define  MEM_IMAGE         0x01000000     // winnt




typedef struct _MEMORY_BASIC_INFORMATION {  
	PVOID       BaseAddress;           //查询内存块所占的第一个页面基地址
	PVOID       AllocationBase;        //内存块所占的第一块区域基地址，小于等于BaseAddress，
	ULONG       AllocationProtect;     //区域被初次保留时赋予的保护属性
	SIZE_T      RegionSize;            //从BaseAddress开始，具有相同属性的页面的大小，
	ULONG       State;                 //页面的状态，有三种可能值MEM_COMMIT、MEM_FREE和MEM_RESERVE
	ULONG       Protect;               //页面的属性，其可能的取值与AllocationProtect相同
	ULONG       Type;                  //该内存块的类型，有三种可能值：MEM_IMAGE、MEM_MAPPED和MEM_PRIVATE
} MEMORY_BASIC_INFORMATION, *PMEMORY_BASIC_INFORMATION;

typedef struct _MEMORY_SECTION_NAME  {  
	UNICODE_STRING Name;  
	WCHAR     Buffer[_MAX_OBJECT_NAME];  
}MEMORY_SECTION_NAME,*PMEMORY_SECTION_NAME;

typedef 
	NTSTATUS (*pfnNtQueryVirtualMemory)(
	__in HANDLE ProcessHandle,
	__in PVOID BaseAddress,
	__in MEMORY_INFORMATION_CLASS MemoryInformationClass,
	__out_bcount(MemoryInformationLength) PVOID MemoryInformation,
	__in SIZE_T MemoryInformationLength,
	__out_opt PSIZE_T ReturnLength
	);

//返回Ring3的信息

typedef struct _PROCESS_MODULE_ENTRY_INFOR_
{
	ULONG_PTR ModuleBase;
	ULONG     ModuleSize;
	WCHAR     ModulePath[260];
}PROCESS_MODULE_ENTRY_INFOR, *PPROCESS_MODULE_ENTRY_INFOR;

typedef struct _PROCESS_MODULE_INFORMATION
{
	ULONG_PTR    NumberOfModule;
	PROCESS_MODULE_ENTRY_INFOR Module[1];
}PROCESS_MODULE_INFORMATION, *PPROCESS_MODULE_INFORMATION;

NTSTATUS EnumMoudleByNtQueryVirtualMemory(ULONG ProcessId,PPROCESS_MODULE_INFORMATION OutBuffer,ULONG_PTR ulOutSize);


#endif