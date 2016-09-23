#ifndef _ANTI_THREAD_H_
#define _ANTI_THREAD_H_

#include "ntifs.h"
#include "InitWindowsVersion.h"
#include "ntos.h"

extern POBJECT_TYPE *PsThreadType;
extern PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;
extern PSERVICE_DESCRIPTOR_TABLE KeServiceDescriptorTable;

#define OBJECT_TO_OBJECT_HEADER(obj) \
	CONTAINING_RECORD( (obj), OBJECT_HEADER, Body )


typedef PVOID (_stdcall *OBGETOBJECTTYPE)( IN PVOID pObject); //win7中添加的函数
OBGETOBJECTTYPE MyGetObjectType;

int ZwCreateSectionIndex;
extern BOOL bDisCreateProcess;
extern PEPROCESS ProtectEProcess;

extern BOOL bAntiDllInject;

extern BOOL bDisDllFuck;  //是否允许dll挟持


typedef NTSTATUS (__stdcall* ZWCREATESECTION)(
	__out     PHANDLE SectionHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in_opt  POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PLARGE_INTEGER MaximumSize,
	__in      ULONG SectionPageProtection,
	__in      ULONG AllocationAttributes,
	__in_opt  HANDLE FileHandle
	);

//网络防御
NTSTATUS _stdcall NewZwCreateSection(
	__out     PHANDLE SectionHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in_opt  POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PLARGE_INTEGER MaximumSize,
	__in      ULONG SectionPageProtection,
	__in      ULONG AllocationAttributes,
	__in_opt  HANDLE FileHandle
	);

BOOLEAN ValidateUnicodeString(PUNICODE_STRING usStr);

BOOL SystemCallEntryTableHook(
	PUNICODE_STRING FunctionName,
	int *Index,
	DWORD NewFuctionAddress
	);
NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size);
PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID ImageBase);
KPROCESSOR_MODE KeGetPreviousMode();
HANDLE MapFileAsSection(PUNICODE_STRING FileName,PVOID *ModuleBase);
ULONG GetInheritedProcessPid(PEPROCESS Eprocess);
BOOL IsFileInSystem(WCHAR *lpwzFile);
#endif