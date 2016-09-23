#ifndef CXX_FILE_H
#define CXX_FILE_H

#include <ntifs.h>
#include "ProcessManager.h"
#include "ProcessHandle.h"

typedef struct _SYSTEM_HANDLE_ENTRY_INFOR_
{
	USHORT	UniqueProcessId;
	USHORT	CreatorBackTraceIndex;
	UCHAR	ObjectTypeIndex;
	UCHAR	HandleAttributes;
	USHORT	HandleValue;
	PVOID	Object;
	ULONG	GrantedAccess;
} SYSTEM_HANDLE_ENTRY_INFOR, *PSYSTEM_HANDLE_ENTRY_INFOR;

typedef struct _SYSTEM_HANDLE_INFOR
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_ENTRY_INFOR Handles[1];
} SYSTEM_HANDLE_INFOR, *PSYSTEM_HANDLE_INFOR;

typedef enum _SYSTEM_HANDLE_TYPE
{
	OB_TYPE_UNKNOWN =0,	
	OB_TYPE_TYPE,			
	OB_TYPE_DIRECTORY,	
	OB_TYPE_SYMBOLIC_LINK,
	OB_TYPE_TOKEN,		
	OB_TYPE_PROCESS,	
	OB_TYPE_THREAD,		
	OB_TYPE_JOB,			
	OB_TYPE_DEBUG_OBJECT,	
	OB_TYPE_EVENT,		
	OB_TYPE_EVENT_PAIR,		
	OB_TYPE_MUTANT,		
	OB_TYPE_CALLBACK,	
	OB_TYPE_SEMAPHORE,	
	OB_TYPE_TIMER,		
	OB_TYPE_PROFILE,		
	OB_TYPE_KEYED_EVENT,	
	OB_TYPE_WINDOWS_STATION,
	OB_TYPE_DESKTOP,		
	OB_TYPE_SECTION,		
	OB_TYPE_KEY,			
	OB_TYPE_PORT,			
	OB_TYPE_WAITABLE_PORT,	
	OB_TYPE_ADAPTER,		
	OB_TYPE_CONTROLLER,	
	OB_TYPE_DEVICE,	
	OB_TYPE_DRIVER,		
	OB_TYPE_IOCOMPLETION,
	OB_TYPE_FILE,		
	OB_TYPE_WMIGUID		
}SYSTEM_HANDLE_TYPE;




typedef struct _OBJECT_DIRECTORY_INFORMATION
{
	UNICODE_STRING Name;
	UNICODE_STRING TypeName;
} OBJECT_DIRECTORY_INFORMATION, *POBJECT_DIRECTORY_INFORMATION;

extern
NTSTATUS
ZwQuerySystemInformation(SYSTEM_INFORMATION_CLASS SystemInformationClass,
PVOID SystemInformation,
ULONG SystemInformationLength,
PULONG ReturnLength);

extern
NTSTATUS
ZwQueryDirectoryObject (
HANDLE DirectoryHandle,
PVOID Buffer,
ULONG Length,
BOOLEAN ReturnSingleEntry,
BOOLEAN RestartScan,
PULONG Context,
PULONG ReturnLength
);

extern
NTSTATUS    
ZwOpenDirectoryObject(PHANDLE DirectoryHandle,ACCESS_MASK DesiredAccess,POBJECT_ATTRIBUTES ObjectAttributes);

BOOLEAN   GetPathByFileObject(PFILE_OBJECT FileObject, WCHAR* wzPath);
NTSTATUS  ForceDeleteProcessHandle(WCHAR* wzFileFullPath);
BOOLEAN   DosPathToNtPathW(WCHAR* wzFullDosPath,WCHAR* wzFullNtPath);
ULONG     NtQueryDosDevice(WCHAR* wzDosDevice,WCHAR* wzNtDevice,ULONG ucchMax);
#endif