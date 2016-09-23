#ifndef _PROTECTCODE_H_
#define _PROTECTCODE_H_

#include "ntifs.h"
#include "InitWindowsVersion.h"
#include "ldasm.h"
#include "ntos.h"
#include "ObjectHookCheck.h"

#define SystemHandleInformation 0x10

UCHAR *PsGetProcessImageFileName(__in PEPROCESS eprocess);

typedef VOID (FASTCALL *KIINSERTQUEUEAPC)(
	IN PKAPC Apc,
	IN KPRIORITY Increment
	); 

typedef BOOLEAN (__stdcall *KEINSERTQUEUEAPC)(
	__inout PRKAPC Apc,
	__in_opt PVOID SystemArgument1,
	__in_opt PVOID SystemArgument2,
	__in KPRIORITY Increment
	);

typedef NTSTATUS (_stdcall*PARSEPROCEDURE) (
	__in PVOID RootDirectory,
	__in POBJECT_TYPE ObjectType,
	__in PACCESS_STATE AccessState,
	__in KPROCESSOR_MODE AccessCheckMode,
	__in ULONG Attributes,
	__in PUNICODE_STRING ObjectName,
	__in PUNICODE_STRING RemainingName,
	__in PVOID ParseContext ,
	__in PSECURITY_QUALITY_OF_SERVICE SecurityQos,
	__out PVOID *Object
	);

int KiInsertQueueApcPatchCodeLen;
PVOID KiInsertQueueApcRet;

int KeInsertQueueApcPatchCodeLen;
PVOID KeInsertQueueApcRet;

int ParseFilePatchCodeLen;
PVOID ParseFileRet;

BOOL KiInsertQueueApcHooked;
BOOL KeInsertQueueApcHooked;
BOOL ParseFileHooked;

WIN_VER_DETAIL WinVer;
ULONG ulThreadOffset;
ULONG ulApcStateOffset;
ULONG ulProcessOffset;
ULONG ulSystemArgument1Offset;

extern PEPROCESS AttachGuiEProcess;
extern PEPROCESS ProtectEProcess;  //自己进程的ep
extern PEPROCESS SystemEProcess;

extern BOOL DebugOn;
extern BOOL bProtectDriverFile;

extern BYTE *ImageModuleBase;
extern ULONG SystemKernelModuleBase;

extern BOOL bProtectProcess;   //是否保护进程
extern BOOL bIsInitSuccess;

typedef struct _SYSTEM_HANDLE_INFORMATION{
	ULONG ProcessID;                //进程的标识ID
	UCHAR ObjectTypeNumber;         //对象类型
	UCHAR Flags;                    //0x01 = PROTECT_FROM_CLOSE,0x02 = INHERIT
	USHORT Handle;                  //对象句柄的数值
	PVOID  Object;                  //对象句柄所指的内核对象地址
	ACCESS_MASK GrantedAccess;      //创建句柄时所准许的对象的访问权
}SYSTEM_HANDLE_INFORMATION, * PSYSTEM_HANDLE_INFORMATION;

typedef struct _SYSTEM_HANDLE_INFORMATION_EX
{
	ULONG NumberOfHandles;
	SYSTEM_HANDLE_INFORMATION Information[];
}SYSTEM_HANDLE_INFORMATION_EX, *PSYSTEM_HANDLE_INFORMATION_EX;

typedef enum _OBJECT_INFORMATION_CLASS{
	ObjectBasicInformation,
	ObjectNameInformation,
	ObjectTypeInformation,
	ObjectAllInformation,
	ObjectDataInformation
}OBJECT_INFORMATION_CLASS, *POBJECT_INFORMATION_CLASS;

//------------------------------------------------------------
//防御日志
//------------------------------------------------------------
typedef struct _LOGDEFENSE_INFORMATION {          //LOGDEFENSE_INFORMATION
	int  Type;     //类型 1:自己是否被结束 2:创建修改服务
	ULONG ulPID;
	ULONG ulInheritedFromProcessId;      //父进程
	CHAR lpszProName[100];            //进程名
	ULONG EProcess;
	WCHAR lpwzMoreEvents[1024];
	WCHAR lpwzCreateProcess[260];

} LOGDEFENSE_INFORMATION, *PLOGDEFENSE_INFORMATION;

typedef struct _LOGDEFENSE {          //防御日志

	ULONG ulCount;
	LOGDEFENSE_INFORMATION LogDefense[1];

} LOGDEFENSE, *PLOGDEFENSE;

PLOGDEFENSE LogDefenseInfo;
ULONG ulLogCount;
//------------------------------------------------------------
NTSTATUS ZwQuerySystemInformation(
	__in       ULONG SystemInformationClass,
	__inout    PVOID SystemInformation,
	__in       ULONG SystemInformationLength,
	__out_opt  PULONG ReturnLength
	);

NTSTATUS ZwQueryObject(
	__in_opt   HANDLE Handle,
	__in       OBJECT_INFORMATION_CLASS ObjectInformationClass,
	__out_opt  PVOID ObjectInformation,
	__in       ULONG ObjectInformationLength,
	__out_opt  PULONG ReturnLength
	);

BOOL HookFunctionByHeaderAddress(
	DWORD NewFunctionAddress,
	DWORD oldFunctionAddress,
	PVOID HookZone,
	int *patchCodeLen,
	PVOID *lpRet
	);

NTSTATUS LookupProcessByPid(
	IN HANDLE hProcessPid,
	OUT PEPROCESS *pEprocess
	);

ULONG GetSystemRoutineAddress(
	int IntType,
	PVOID lpwzFunction
	);

BOOLEAN ValidateUnicodeString(PUNICODE_STRING usStr);

#endif