#ifndef _PROCESS_H_
#define _PROCESS_H_

#include "ntifs.h"
#include "ntos.h"

#define SystemProcessesAndThreadsInformation 5

typedef struct _SYSTEM_THREADS {
	LARGE_INTEGER   KernelTime;
	LARGE_INTEGER   UserTime;
	LARGE_INTEGER   CreateTime;
	ULONG           WaitTime;
	PVOID           StartAddress;
	CLIENT_ID       ClientIs;
	KPRIORITY       Priority;
	KPRIORITY       BasePriority;
	ULONG           ContextSwitchCount;
	ULONG           ThreadState;
	KWAIT_REASON    WaitReason;
}SYSTEM_THREADS;

typedef struct _SYSTEM_PROCESSES {
	ULONG           NextEntryDelta;
	ULONG           ThreadCount;
	ULONG           Reserved[6];
	LARGE_INTEGER   CreateTime;
	LARGE_INTEGER   UserTime;
	LARGE_INTEGER   KernelTime;
	UNICODE_STRING  ProcessName;
	KPRIORITY       BasePriority;
	ULONG           ProcessId;
	ULONG           InheritedFromProcessId;
	ULONG           HandleCount;
	ULONG           Reserved2[2];
	VM_COUNTERS     VmCounters;
	IO_COUNTERS     IoCounters; //windows 2000 only
	struct _SYSTEM_THREADS Threads[1];
}SYSTEM_PROCESSES;

//--------------------------------------------------------------------------
// 
// NTSTATUS ZwQuerySystemInformation(
//   __in       ULONG SystemInformationClass,
//   __inout    PVOID SystemInformation,
//   __in       ULONG SystemInformationLength,
//   __out_opt  PULONG ReturnLength
// );

extern BOOL DebugOn;
extern PEPROCESS ProtectEProcess;
extern PEPROCESS SystemEProcess;

extern  BOOL bPaused;

typedef NTSTATUS (__stdcall *NTSUSPENDPROCESS)(
	 IN HANDLE hProcess
	);

typedef NTSTATUS (__stdcall *NTRESUMEPROCESS)(
	 IN HANDLE hProcess
	);

///////////////////////////////////////////////////////////////////////////////////////
NTKERNELAPI VOID KeSetSystemAffinityThread (KAFFINITY Affinity );
NTKERNELAPI VOID KeRevertToUserAffinityThread (VOID);

BOOL GetProcessFullImagePath(    
	IN  PEPROCESS Eprocess, 
	OUT WCHAR *FullProcessImagePath    
	);

NTSTATUS LookupProcessByPid(
	IN HANDLE hProcessPid,
	OUT PEPROCESS *pEprocess
	);

NTSTATUS SafeCopyMemory(PVOID SrcAddr, PVOID DstAddr, ULONG Size);

BOOL IsExitProcess(PEPROCESS Eprocess);
BOOL KernelStatus(HANDLE hPid);
ULONG GetInheritedProcessPid(PEPROCESS Eprocess);
NTSTATUS SafeQueryFileDosDeviceName(__in WCHAR *wzNtImageName,__out WCHAR *wzDosFullPath);

#endif