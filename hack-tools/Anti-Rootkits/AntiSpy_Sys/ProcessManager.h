#ifndef CXX_PROCESS_MANAGER_H
#define CXX_PROCESS_MANAGER_H

#include <ntifs.h>
#include <stdlib.h>
#include "Function.h"
#include "SSDT.h"
#define   MAX_PATH 260

typedef struct _PROCESS_INFO_
{
	ULONG ulProcessID;
	ULONG ulProcessParentID;
	ULONG_PTR ulEProcess;
	WCHAR wzCompanyName[MAX_PATH];
	WCHAR wzProcessFileName[MAX_PATH];
	WCHAR wzProcessName[MAX_PATH];
	BOOLEAN  bHide;
}PROCESS_INFO,*PPROCESS_INFO;

typedef struct _PROCESS_INFORMATION_
{
	ULONG_PTR    NumberOfProcess;
	PROCESS_INFO Process[1];
}RTL_PROCESS_INFORMATION,*PRTL_PROCESS_INFORMATION;


typedef struct _LDR_DATA_TABLE_ENTRY32 
{
	LIST_ENTRY32 InLoadOrderLinks;
	LIST_ENTRY32 InMemoryOrderLinks;
	LIST_ENTRY32 InInitializationOrderLinks;
	ULONG DllBase;
	ULONG EntryPoint;
	ULONG SizeOfImage;
	UNICODE_STRING32 FullDllName;
	UNICODE_STRING32 BaseDllName;
	ULONG Flags;
	USHORT LoadCount;
	USHORT TlsIndex;
	union {
		LIST_ENTRY32 HashLinks;
		struct {
			ULONG SectionPointer;
			ULONG  CheckSum;
		};
	};
	union {
		struct {
			ULONG  TimeDateStamp;
		};
		struct {
			ULONG LoadedImports;
		};
	};
} LDR_DATA_TABLE_ENTRY32, *PLDR_DATA_TABLE_ENTRY32;


typedef enum _SYSTEM_INFORMATION_CLASS {
	SystemBasicInformation,
	SystemProcessorInformation,             
	SystemPerformanceInformation,
	SystemTimeOfDayInformation,
	SystemPathInformation,
	SystemProcessInformation,
	SystemCallCountInformation,
	SystemDeviceInformation,
	SystemProcessorPerformanceInformation,
	SystemFlagsInformation,
	SystemCallTimeInformation,
	SystemModuleInformation,
	SystemLocksInformation,
	SystemStackTraceInformation,
	SystemPagedPoolInformation,
	SystemNonPagedPoolInformation,
	SystemHandleInformation,
	SystemObjectInformation,
	SystemPageFileInformation,
	SystemVdmInstemulInformation,
	SystemVdmBopInformation,
	SystemFileCacheInformation,
	SystemPoolTagInformation,
	SystemInterruptInformation,
	SystemDpcBehaviorInformation,
	SystemFullMemoryInformation,
	SystemLoadGdiDriverInformation,
	SystemUnloadGdiDriverInformation,
	SystemTimeAdjustmentInformation,
	SystemSummaryMemoryInformation,
	SystemMirrorMemoryInformation,
	SystemPerformanceTraceInformation,
	SystemObsolete0,
	SystemExceptionInformation,
	SystemCrashDumpStateInformation,
	SystemKernelDebuggerInformation,
	SystemContextSwitchInformation,
	SystemRegistryQuotaInformation,
	SystemExtendServiceTableInformation,
	SystemPrioritySeperation,
	SystemVerifierAddDriverInformation,
	SystemVerifierRemoveDriverInformation,
	SystemProcessorIdleInformation,
	SystemLegacyDriverInformation,
	SystemCurrentTimeZoneInformation,
	SystemLookasideInformation,
	SystemTimeSlipNotification,
	SystemSessionCreate,
	SystemSessionDetach,
	SystemSessionInformation,
	SystemRangeStartInformation,
	SystemVerifierInformation,
	SystemVerifierThunkExtend,
	SystemSessionProcessInformation,
	SystemLoadGdiDriverInSystemSpace,
	SystemNumaProcessorMap,
	SystemPrefetcherInformation,
	SystemExtendedProcessInformation,
	SystemRecommendedSharedDataAlignment,
	SystemComPlusPackage,
	SystemNumaAvailableMemory,
	SystemProcessorPowerInformation,
	SystemEmulationBasicInformation,
	SystemEmulationProcessorInformation,
	SystemExtendedHandleInformation,
	SystemLostDelayedWriteInformation,
	SystemBigPoolInformation,
	SystemSessionPoolTagInformation,
	SystemSessionMappedViewInformation,
	SystemHotpatchInformation,
	SystemObjectSecurityMode,
	SystemWatchdogTimerHandler,
	SystemWatchdogTimerInformation,
	SystemLogicalProcessorInformation,
	SystemWow64SharedInformation,
	SystemRegisterFirmwareTableInformationHandler,
	SystemFirmwareTableInformation,
	SystemModuleInformationEx,
	SystemVerifierTriageInformation,
	SystemSuperfetchInformation,
	SystemMemoryListInformation,
	SystemFileCacheInformationEx,
	MaxSystemInfoClass  
} SYSTEM_INFORMATION_CLASS;


typedef struct _RTL_SYSTEM_PROCESS_INFORMATION {
	ULONG NextEntryOffset;
	ULONG NumberOfThreads;
	LARGE_INTEGER SpareLi1;
	LARGE_INTEGER SpareLi2;
	LARGE_INTEGER SpareLi3;
	LARGE_INTEGER CreateTime;
	LARGE_INTEGER UserTime;
	LARGE_INTEGER KernelTime;
	UNICODE_STRING ImageName;
	KPRIORITY BasePriority;
	HANDLE UniqueProcessId;
	HANDLE InheritedFromUniqueProcessId;
	ULONG HandleCount;
	ULONG SessionId;
	ULONG_PTR PageDirectoryBase;
	SIZE_T PeakVirtualSize;
	SIZE_T VirtualSize;
	ULONG PageFaultCount;
	SIZE_T PeakWorkingSetSize;
	SIZE_T WorkingSetSize;
	SIZE_T QuotaPeakPagedPoolUsage;
	SIZE_T QuotaPagedPoolUsage;
	SIZE_T QuotaPeakNonPagedPoolUsage;
	SIZE_T QuotaNonPagedPoolUsage;
	SIZE_T PagefileUsage;
	SIZE_T PeakPagefileUsage;
	SIZE_T PrivatePageCount;
	LARGE_INTEGER ReadOperationCount;
	LARGE_INTEGER WriteOperationCount;
	LARGE_INTEGER OtherOperationCount;
	LARGE_INTEGER ReadTransferCount;
	LARGE_INTEGER WriteTransferCount;
	LARGE_INTEGER OtherTransferCount;
} RTL_SYSTEM_PROCESS_INFORMATION, *PRTL_SYSTEM_PROCESS_INFORMATION;



BOOLEAN        EnumProcess(PULONG OutputBuffer,ULONG OutputSize);
PVOID          GetFunctionAddressofName(WCHAR *wzFunction);
ULONG_PTR      GetEProcessByProcessID(ULONG ulProcessID);
PVOID          GetFunctionAddressByName(WCHAR *wzFunction);


NTSTATUS    GetProcessImagePath(
	ULONG   ulProcessId,
	WCHAR*  ProcessImagePath
	);

extern         POBJECT_TYPE* PsProcessType; 

extern NTSTATUS        
	ZwQuerySystemInformation(IN SYSTEM_INFORMATION_CLASS SystemInformationClass,    
	OUT PVOID SystemInformation,    
	IN ULONG SystemInformationLength,    
	OUT PULONG ReturnLength OPTIONAL);
extern UCHAR* PsGetProcessImageFileName(PEPROCESS EProcess);

typedef NTSTATUS  (*pfnZwQueryInformationProcess)(
	HANDLE           ProcessHandle,
    PROCESSINFOCLASS ProcessInformationClass,
	PVOID            ProcessInformation,
	ULONG            ProcessInformationLength,
	PULONG           ReturnLength
	);


typedef struct _DELETE_DATA_
{
	ULONG_PTR ulProcessId;
	WCHAR     wzImagePath[MAX_PATH];
}DATA,*PDATA;

BOOLEAN  KillProcessByProcessId(ULONG_PTR ProcessID);
BOOLEAN  DeleteFile(WCHAR* wzFilePath);
BOOLEAN  charToWCHAR(char* szSource,WCHAR** wzDest);

typedef  ULONG_PTR (*pfnObGetObjectType)(PVOID pObject);

BOOLEAN       IsRealProcess(PEPROCESS EProcess);
BOOLEAN       IsProcessDie(PEPROCESS EProcess);
ULONG_PTR     KeGetObjectType(PVOID Object);
VOID          ForceEnumHideProcessData(ULONG ulProcessID,PULONG OutputBuffer);

#define PS_CROSS_THREAD_FLAGS_SYSTEM 0x00000010UL
typedef enum _KAPC_ENVIRONMENT_INFOR {	OriginalApcEnvironment,	AttachedApcEnvironment,	CurrentApcEnvironment,	InsertApcEnvironment} KAPC_ENVIRONMENT_INFOR;


VOID KeInitializeApc (	PKAPC Apc,	PETHREAD Thread,	KAPC_ENVIRONMENT_INFOR Environment,	PKKERNEL_ROUTINE KernelRoutine,	PKRUNDOWN_ROUTINE RundownRoutine,	PKNORMAL_ROUTINE NormalRoutine,	KPROCESSOR_MODE ProcessorMode,	PVOID NormalContext	);VOID KernelKillThreadRoutine(PKAPC Apc,	PKNORMAL_ROUTINE *NormalRoutine,	PVOID *NormalContext,	PVOID *SystemArgument1,	PVOID *SystemArgument2	);BOOLEAN KeInsertQueueApc(PKAPC Apc,PVOID SystemArg1,PVOID SystemArg2,KPRIORITY Increment);
BOOLEAN KillProcessForcedByProcessId_InsertAPC(PEPROCESS EProcess);

#endif