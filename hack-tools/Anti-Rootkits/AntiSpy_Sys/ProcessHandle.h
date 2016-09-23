#ifndef CXX_PROCESS_HANDLE_H
#define CXX_PROCESS_HANDLE_H

#include <ntifs.h>
#include "SSDT.h"
#include "ProcessManager.h"
#include "Function.h"



extern                                             
	NTSTATUS 
	NtQuerySystemInformation(   
	IN ULONG SystemInformationClass,   
	IN PVOID SystemInformation,   
	IN ULONG SystemInformationLength,   
	OUT PULONG ReturnLength);

//SystemHandleTable  结构

typedef struct _SYSTEM_HANDLE_TABLE_ENTRY_INFOR{
	USHORT  UniqueProcessId;
	USHORT  CreatorBackTraceIndex;
	UCHAR   ObjectTypeIndex;
	UCHAR   HandleAttributes;
	USHORT  HandleValue;
	PVOID   Object;
	ULONG   GrantedAccess;
} SYSTEM_HANDLE_TABLE_ENTRY_INFOR, *PSYSTEM_HANDLE_TABLE_ENTRY_INFOR;

typedef struct _SYSTEM_HANDLE_INFORMATION{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_TABLE_ENTRY_INFOR Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;



//Ring0 --  Ring3  返回数据的结构体
typedef struct _PROCESS_HANDLE_ENTRY_INFOR_
{
	ULONG_PTR      Handle;
	ULONG_PTR      Object;
	ULONG_PTR      ReferenceCount;
	WCHAR          wzHandleType[MAX_PATH];
	WCHAR          wzHandleName[512];
}PROCESS_HANDLE_ENTRY_INFOR, *PPROCESS_HANDLE_ENTRY_INFOR;

typedef struct _PROCESS_HANDLE_INFORMATION
{
	ULONG_PTR NumberOfHandles;
	PROCESS_HANDLE_ENTRY_INFOR Handles[1];
}PROCESS_HANDLE_INFORMATION, *PPROCESS_HANDLE_INFORMATION;


typedef NTSTATUS (*pfnNtQueryObject)(HANDLE Handle,
	OBJECT_INFORMATION_CLASS ObjectInformationClass,
	PVOID   ObjectInformation,
	ULONG ObjectInformationLength,
	PULONG ReturnLength);


typedef NTSTATUS
	(*pfnNtSetInformationObject) (
	HANDLE Handle,
	OBJECT_INFORMATION_CLASS ObjectInformationClass,
	PVOID ObjectInformation,
	ULONG ObjectInformationLength);


//得到Object对象的信息(类型和路径)
typedef struct _OBJECT_DATA_INFORMATION {
	BOOLEAN                 InheritHandle;
	BOOLEAN                 ProtectFromClose;
}OBJECT_DATA_INFORMATION, *POBJECT_DATA_INFORMATION;

typedef enum _OBJECT_INFORMATION_CLASS_ {
	BasicInformation,
	NameInformation,
	TypeInformation,
	TypesInformation,
	HandleFlagInformation,
	SessionInformation,
	MaxObjectInfoClass  
}_OBJECT_INFORMATION_CLASS;


//关闭句柄
struct _DATA_ 
{
	ULONG       ulProcessID;
	ULONG_PTR   ulHandle; 
}Data;


VOID      GetHandleName(HANDLE Handle,WCHAR* wzHandleName);
VOID      GetHandleType(HANDLE Handle,WCHAR* wzHandleType);
NTSTATUS  EnumProcessHandle(PVOID InBuffer, PVOID OutBuffer, ULONG_PTR ulOutSize);
NTSTATUS  KillProcessHandle(PVOID InBuffer);
NTSTATUS  GetProcessHandles(ULONG ulPid, ULONG_PTR EProcess, PPROCESS_HANDLE_INFORMATION OutBuffer, ULONG_PTR ulCount);
BOOLEAN   IsUnicodeStringValid(PUNICODE_STRING uniString);
VOID      InsertInformationToRing3Memroy(PEPROCESS EProcess, HANDLE Handle, ULONG_PTR Object, PPROCESS_HANDLE_INFORMATION OutBuffer);
#endif