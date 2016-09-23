#ifndef _KERNEL_FILTERDRIVER_H_
#define _KERNEL_FILTERDRIVER_H_

#include "ntifs.h"
#include "ntos.h"

extern POBJECT_TYPE *IoDriverObjectType;

typedef struct _DIRECTORY_BASIC_INFORMATION {
	UNICODE_STRING ObjectName;
	UNICODE_STRING ObjectTypeName;
} DIRECTORY_BASIC_INFORMATION, *PDIRECTORY_BASIC_INFORMATION;

//检测到的过滤驱动的信息
typedef struct _KERNEL_FILTERDRIVER_INFO{

	ULONG ulObjType;
	ULONG ulAttachDevice;
	WCHAR FileName[256];
	WCHAR FilePath[256];
	//宿主
	WCHAR HostFileName[256];
}KERNEL_FILTERDRIVER_INFO,*PKERNEL_FILTERDRIVER_INFO;

typedef struct _KERNEL_FILTERDRIVER{

	ULONG ulCount;
	KERNEL_FILTERDRIVER_INFO KernelFilterDriverInfo[1];

}KERNEL_FILTERDRIVER,*PKERNEL_FILTERDRIVER;

PKERNEL_FILTERDRIVER KernelFilterDriver;

int IntFilterCount;

extern BOOL DebugOn;

NTSTATUS ZwQueryDirectoryObject (
	IN HANDLE       DirectoryHandle,
	OUT PVOID       Buffer,
	IN ULONG        Length,
	IN BOOLEAN      ReturnSingleEntry,
	IN BOOLEAN      RestartScan,
	IN OUT PULONG   Context,
	OUT PULONG      ReturnLength OPTIONAL
	);

NTSTATUS 
	ObReferenceObjectByName( 
	IN PUNICODE_STRING ObjectName, 
	IN ULONG Attributes, 
	IN PACCESS_STATE PassedAccessState OPTIONAL, 
	IN ACCESS_MASK DesiredAccess OPTIONAL, 
	IN POBJECT_TYPE ObjectType, 
	IN KPROCESSOR_MODE AccessMode, 
	IN OUT PVOID ParseContext OPTIONAL, 
	OUT PVOID *Object 
	);

BOOLEAN ValidateUnicodeString(
	PUNICODE_STRING usStr
	);

NTSTATUS SafeCopyMemory(
	PVOID SrcAddr, 
	PVOID DstAddr,
	ULONG Size
	);

#endif