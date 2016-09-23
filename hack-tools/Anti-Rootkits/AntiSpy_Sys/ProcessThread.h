

#ifndef CXX_PROCESS_THREAD_H
#define CXX_PROCESS_THREAD_H

#include <ntifs.h>
#include "ProcessManager.h"
#include "Function.h"
#include "SSDT.h"
#include "ProcessModule.h"

#define MAX_MODULE_COUNT 0x1000

typedef struct _PROCESS_THREAD_ENTRY_INFOR_
{
	ULONG_PTR Thread;
	ULONG_PTR Tid;
	ULONG_PTR Teb;
	UCHAR Priority;
	ULONG_PTR Win32StartAddress;
	ULONG ContextSwitches;
	UCHAR State;
	WCHAR Module[260];
}PROCESS_THREAD_ENTRY_INFOR, *PPROCESS_THREAD_ENTRY_INFOR;

typedef struct _PROCESS_THREAD_INFORMATION
{
	ULONG_PTR    NumberOfThreads;
	PROCESS_THREAD_ENTRY_INFOR Threads[1];
}PROCESS_THREAD_INFORMATION, *PPROCESS_THREAD_INFORMATION;




/*
typedef struct _MODULEINFOR  
{
	ULONG_PTR ModuleBase;
	ULONG_PTR ModuleSize;
	WCHAR     ModulePath[260];
}MODULEINFOR,*PMODULEINFOR;*/

typedef
	NTSTATUS
	(*pfnNtTerminateThreadAddress)(
	IN HANDLE Thread,
	IN NTSTATUS ExitStatus
	);

typedef
	NTSTATUS
	(*pfnPspTerminateThreadByPointer)(
	IN PETHREAD Thread,
	IN NTSTATUS ExitStatus,
	IN BOOLEAN DirectTerminate
	);

typedef
	NTSTATUS
	(*pfnPsTerminateSystemThread)(
	IN NTSTATUS ExitStatus
	);

typedef
	NTSTATUS
	(*pfnNtSuspendThread)(
	HANDLE hThread,
	ULONG_PTR* ulPreviousSuspendCount
	);

typedef
	NTSTATUS
	(*pfnNtResumeThread)(
	HANDLE hThread,
	ULONG_PTR* ulPreviousSuspendCount
	);
extern PPEB PsGetProcessPeb(PEPROCESS Eprocess);

VOID GetProcessThreads(PEPROCESS EProcess,  PPROCESS_THREAD_INFORMATION OutBuffer, ULONG_PTR ulCount);
VOID InsertThreadInforToRing3Memroy(PETHREAD EThread, PEPROCESS EProcess, PPROCESS_THREAD_INFORMATION OutBuffer, ULONG ulCount);
BOOLEAN IsThreadInRing3Memory(PETHREAD EThread, PPROCESS_THREAD_INFORMATION OutBuffer, ULONG ulCount);

NTSTATUS EnumProcessThread(PVOID InBuffer,PVOID OutBuffer,ULONG_PTR ulOutSize);
NTSTATUS JudgeThreadIsSuspend(PETHREAD EThread,ULONG_PTR* ulIsThreadSuspend);
NTSTATUS PspSuspendOrResumeThread(PETHREAD EThread, BOOLEAN bSuspend);
NTSTATUS KillProcessThread(PETHREAD EThread);
NTSTATUS ForceKillProcessThread(PETHREAD EThread);
VOID GetThreadsModule(ULONG ulPid,PVOID OutBuffer);
NTSTATUS EnumModule(ULONG_PTR ProcessID,PPROCESS_MODULE_INFORMATION ModuleInfor);
NTSTATUS MatchModuleToThread(PVOID OutputBuffer,PPROCESS_MODULE_INFORMATION ModuleInfor);

BOOLEAN	UnicodeStringToWchar(UNICODE_STRING* uniSource,WCHAR** wzDest);



#endif