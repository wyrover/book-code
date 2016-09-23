#ifndef _NTOS_H_
#define _NTOS_H_

#include "ntifs.h"
#include "InlineHook.h"
#include <ntimage.h>
#include "InitWindowsVersion.h"
#include "Protect.h"

extern BOOL bKernelSafeModule;
extern ULONG ShadowTable;
extern PSERVICE_DESCRIPTOR_TABLE OriginalShadowServiceDescriptorTable;
extern PSERVICE_DESCRIPTOR_TABLE Safe_ServiceDescriptorShadowSSDTTable;

extern ULONG ulMyDriverBase;
extern ULONG ulMyDriverSize;

//-------------------------------------------------------------------------
//保存所有导出的内核函数的一个结构
//-------------------------------------------------------------------------
typedef struct _KERNELFUNC_ADDRESS_INFORMATION {          //SAFESYSTEM_PROCESS_INFORMATION
	ULONG ulAddress;
	ULONG ulReloadAddress;
	WCHAR FuncName[50];
	int NumberOfFunctions; //函数在导出表里面的位置
}KERNELFUNC_ADDRESS_INFORMATION, *PKERNELFUNC_ADDRESS_INFORMATION;

typedef struct _NTOSFUNCINFO {          //PNTOSFUNCINFO
	ULONG ulCount;
	ULONG ulModuleBase;    //模块的基址
	WCHAR ModulePath[260];
	KERNELFUNC_ADDRESS_INFORMATION NtosFuncInfo[1];
} NTOSFUNCINFO, *PNTOSFUNCINFO;

PNTOSFUNCINFO NtosFuncAddressInfo;       //没有解析符号的ntos内核函数信息结构
PNTOSFUNCINFO PDBNtosFuncAddressInfo;    //解析符号的ntos内核函数信息结构（包括导出未导出函数）
PNTOSFUNCINFO SelectModuleFuncInfo;      //当前选择的解析符号的驱动函数信息结构（包括导出未导出函数）
PNTOSFUNCINFO KernelFuncInfo;            //这个只是一个过度而已
//-------------------------------------------------------------------------

typedef struct _SAFESYSTEM_PROCESS_INFORMATION {          //SAFESYSTEM_PROCESS_INFORMATION
	int   IntHideType;
	ULONG ulPid;
	PEPROCESS EProcess;
	WCHAR lpwzFullProcessPath[256];
	ULONG ulInheritedFromProcessId;  //父进程
	ULONG ulKernelOpen;
}SAFESYSTEM_PROCESS_INFORMATION, *PSAFESYSTEM_PROCESS_INFORMATION;

typedef struct _PROCESSINFO {          //PROCESSINFO
	ULONG ulCount;
	SAFESYSTEM_PROCESS_INFORMATION ProcessInfo[1];
} PROCESSINFO, *PPROCESSINFO;

//--------------------------------------------------------------------------
PPROCESSINFO NormalProcessInfo;
PPROCESSINFO HideProcessInfo;


PSERVICE_DESCRIPTOR_TABLE OriginalServiceDescriptorTable;
PSERVICE_DESCRIPTOR_TABLE Safe_ServiceDescriptorTable;

#define NOHOOK 0;
#define SSDTHOOK 1
#define SSDTINLINEHOOK 2
#define INLINEHOOK 3

BYTE *ImageModuleBase;
ULONG SystemKernelModuleBase;
ULONG SystemKernelModuleSize;
WCHAR *SystemKernelFilePath;

DWORD OriginalKiServiceTable;

PVOID KiFastCallEntryRet;
int PatchCodeLength;

PVOID KiFastCallEntryTempRet;
int KiFastCallEntryTempPatchCodeLength;

PUCHAR ul360HookAddress;
BYTE ulHookCodeBak[5];
BYTE ulRealCodeBak[5];

BYTE ByteKiFastCallEntryBak[5];
BYTE ByteReloadKiFastCallEntryBak[5];

extern BOOL bIsInitSuccess;
extern BOOL bInitWin32K;

KEVENT WaitEvent;

VOID WaitMicroSecond(LONG MicroSeconds);

UCHAR *PsGetProcessImageFileName(
	__in PEPROCESS eprocess
	);

ULONG IsHideProcess(
	ULONG ulPid,
	PPROCESSINFO Info
	);

PVOID
	MiFindExportedRoutine (
	IN PVOID DllBase,
	BOOL ByName,
	IN char *RoutineName,
	DWORD Ordinal
	);

BOOL GetSystemKernelModuleInfo(
	WCHAR **SystemKernelModulePath,
	PDWORD SystemKernelModuleBase,
	PDWORD SystemKernelModuleSize
	);

BOOL PeLoad(
	WCHAR *FileFullPath,
	BYTE **ImageModeleBase,
	PDRIVER_OBJECT DeviceObject,
	DWORD ExistImageBase
	);

ULONG ReLoadNtosCALL(
	ULONG *FuncSyntax,
	WCHAR *lpwzFuncTion,
	ULONG ulOldNtosBase,
	ULONG ulReloadNtosBase
	);

BOOL MmIsAddressValidEx(
	IN PVOID Pointer
	);
BOOL MmIsAddressRangeValid(
	IN PVOID Address,
	IN ULONG Size
	);

PIMAGE_NT_HEADERS RtlImageNtHeader(PVOID ImageBase);

BOOL IsExitProcess(PEPROCESS Eprocess);
NTSTATUS ReloadWin32K();
ULONG PsGetProcessCount();
BOOL InitControl();  //通信控制
BOOL IsRegKeyInSystem(PWCHAR ServicesKey);


typedef BOOLEAN (__stdcall *ReloadKeDeregisterBugCheckCallback)(
	__inout  PKBUGCHECK_CALLBACK_RECORD CallbackRecord
	);
ReloadKeDeregisterBugCheckCallback RKeDeregisterBugCheckCallback;

typedef BOOLEAN (__stdcall *ReloadKeDeregisterBugCheckReasonCallback)(
	__inout PKBUGCHECK_REASON_CALLBACK_RECORD CallbackRecord
	);
ReloadKeDeregisterBugCheckReasonCallback RKeDeregisterBugCheckReasonCallback;

typedef VOID (__stdcall *ReloadIoUnregisterShutdownNotification)(
	__in  PDEVICE_OBJECT DeviceObject
	);
ReloadIoUnregisterShutdownNotification RIoUnregisterShutdownNotification;

typedef NTSTATUS (__stdcall *ReloadSeUnregisterLogonSessionTerminatedRoutine)(
	IN PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine
	);
ReloadSeUnregisterLogonSessionTerminatedRoutine RSeUnregisterLogonSessionTerminatedRoutine;

typedef NTSTATUS (__stdcall *ReloadPsRemoveLoadImageNotifyRoutine)(
	__in  PLOAD_IMAGE_NOTIFY_ROUTINE NotifyRoutine
	);
ReloadPsRemoveLoadImageNotifyRoutine RPsRemoveLoadImageNotifyRoutine;

typedef NTSTATUS (__stdcall *ReloadPsRemoveCreateThreadNotifyRoutine)(
	__in  PCREATE_THREAD_NOTIFY_ROUTINE NotifyRoutine
	);
ReloadPsRemoveCreateThreadNotifyRoutine RPsRemoveCreateThreadNotifyRoutine;

typedef NTSTATUS (__stdcall *ReloadPsSetCreateProcessNotifyRoutine)(
	__in  PCREATE_PROCESS_NOTIFY_ROUTINE NotifyRoutine,
	__in  BOOLEAN Remove
	);
ReloadPsSetCreateProcessNotifyRoutine RPsSetCreateProcessNotifyRoutine;

typedef BOOLEAN (__stdcall *ReloadKeCancelTimer)(
	__inout  PKTIMER Timer
	);
ReloadKeCancelTimer RKeCancelTimer;

typedef NTSTATUS (__stdcall *ReloadObQueryNameString)(
	__in       PVOID Object,
	__out_opt  POBJECT_NAME_INFORMATION ObjectNameInfo,
	__in       ULONG Length,
	__out      PULONG ReturnLength
	);
ReloadObQueryNameString RObQueryNameString;

typedef NTSTATUS (__stdcall *ReloadPsTerminateSystemThread)(
  __in  NTSTATUS ExitStatus
);
ReloadPsTerminateSystemThread RPsTerminateSystemThread;

typedef VOID (__stdcall *ReloadKeInitializeApc)(
	PKAPC Apc,
	PETHREAD Thread,
	ULONG Environment,
	PKKERNEL_ROUTINE KernelRoutine,
	PKRUNDOWN_ROUTINE RundownRoutine,
	PKNORMAL_ROUTINE NormalRoutine,
	KPROCESSOR_MODE ProcessorMode,
	PVOID NormalContext
	);
ReloadKeInitializeApc RKeInitializeApc;

typedef BOOLEAN (__stdcall *ReloadKeInsertQueueApc)(
	PKAPC Apc,
	PVOID SystemArg1,
	PVOID SystemArg2,
	KPRIORITY Increment
	);
ReloadKeInsertQueueApc RKeInsertQueueApc;

typedef LONG (__stdcall * ReloadRtlCompareUnicodeString)(
	__in  PCUNICODE_STRING String1,
	__in  PCUNICODE_STRING String2,
	__in  BOOLEAN CaseInSensitive
	);
ReloadRtlCompareUnicodeString RRtlCompareUnicodeString;

typedef LONG (__stdcall * ReloadRtlCompareString)(
	__in  const STRING *String1,
	__in  const STRING *String2,
	__in  BOOLEAN CaseInSensitive
	);
ReloadRtlCompareString RRtlCompareString;

typedef VOID (__stdcall * ReloadRtlInitString)(
	__out     PSTRING DestinationString,
	__in_opt  PCSZ SourceString
	);
ReloadRtlInitString RRtlInitString;

typedef NTSTATUS (__stdcall * ReloadZwQueryVirtualMemory)(
	IN HANDLE               ProcessHandle,
	IN PVOID                BaseAddress,
	IN ULONG MemoryInformationClass,
	OUT PVOID               Buffer,
	IN ULONG                Length,
	OUT PULONG              ResultLength OPTIONAL
	);
ReloadZwQueryVirtualMemory RZwQueryVirtualMemory;

typedef NTSTATUS (__stdcall * ReloadIoQueryFileDosDeviceName)(
	IN PFILE_OBJECT  FileObject,
	OUT POBJECT_NAME_INFORMATION  *ObjectNameInformation
	);
ReloadIoQueryFileDosDeviceName RIoQueryFileDosDeviceName;

typedef UCHAR * (__stdcall * ReloadPsGetProcessImageFileName)(
	__in PEPROCESS eprocess
	);
ReloadPsGetProcessImageFileName RPsGetProcessImageFileName;


typedef PEPROCESS (__stdcall * ReloadIoThreadToProcess)(
    IN PETHREAD  Thread
    ); 
ReloadIoThreadToProcess RIoThreadToProcess;

typedef NTSTATUS (__stdcall * ReloadZwQueryDirectoryObject) (
	IN HANDLE DirectoryHandle,
	OUT PVOID Buffer,
	IN ULONG BufferLength,
	IN BOOLEAN ReturnSingleEntry,
	IN BOOLEAN RestartScan,
	IN OUT PULONG Context,
	OUT PULONG ReturnLength OPTIONAL
	);
ReloadZwQueryDirectoryObject RZwQueryDirectoryObject;

typedef NTSTATUS (__stdcall *ReloadZwOpenDirectoryObject)(
	__out PHANDLE  DirectoryHandle,
	__in ACCESS_MASK  DesiredAccess,
	__in POBJECT_ATTRIBUTES  ObjectAttributes
	);
ReloadZwOpenDirectoryObject RZwOpenDirectoryObject;

typedef VOID (__stdcall *ReloadIoSetCompletionRoutine)(
	__in      PIRP Irp,
	__in_opt  PIO_COMPLETION_ROUTINE CompletionRoutine,
	__in_opt  PVOID Context,
	__in      BOOLEAN InvokeOnSuccess,
	__in      BOOLEAN InvokeOnError,
	__in      BOOLEAN InvokeOnCancel
	);
ReloadIoSetCompletionRoutine RIoSetCompletionRoutine;

typedef PIRP (__stdcall *ReloadIoAllocateIrp)(
	__in  CCHAR StackSize,
	__in  BOOLEAN ChargeQuota
	);
ReloadIoAllocateIrp RIoAllocateIrp;

typedef LONG (__stdcall *ReloadKeSetEvent)(
	__inout  PRKEVENT Event,
	__in     KPRIORITY Increment,
	__in     BOOLEAN Wait
	);
ReloadKeSetEvent RKeSetEvent;

typedef NTSTATUS (__stdcall *ReloadIoCreateFile)(
	__out     PHANDLE FileHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__in_opt  PLARGE_INTEGER AllocationSize,
	__in      ULONG FileAttributes,
	__in      ULONG ShareAccess,
	__in      ULONG Disposition,
	__in      ULONG CreateOptions,
	__in_opt  PVOID EaBuffer,
	__in      ULONG EaLength,
	__in      CREATE_FILE_TYPE CreateFileType,
	__in_opt  PVOID InternalParameters,
	__in      ULONG Options
	);
ReloadIoCreateFile RIoCreateFile;

typedef NTSTATUS (__stdcall *ReloadMmUnmapViewOfSection)(
	IN PEPROCESS Process, 
	IN ULONG BaseAddress 
	); 
ReloadMmUnmapViewOfSection RMmUnmapViewOfSection;

typedef NTSTATUS (__stdcall *ReloadPsLookupProcessByProcessId)(
	__in   HANDLE ProcessId,
	__out  PEPROCESS *Process
	);
ReloadPsLookupProcessByProcessId RPsLookupProcessByProcessId;

typedef HANDLE  (__stdcall *ReloadPsGetCurrentProcessId)(void);
ReloadPsGetCurrentProcessId RPsGetCurrentProcessId;

typedef PEPROCESS  (__stdcall *ReloadPsGetCurrentProcess)(void);
ReloadPsGetCurrentProcess RPsGetCurrentProcess;

typedef VOID (__stdcall *ReloadKeUnstackDetachProcess)(
	__in  PRKAPC_STATE ApcState
	);
ReloadKeUnstackDetachProcess RKeUnstackDetachProcess;

typedef VOID (__stdcall *ReloadKeStackAttachProcess)(
	__inout  PRKPROCESS Process,
	__out    PRKAPC_STATE ApcState
	);
ReloadKeStackAttachProcess RKeStackAttachProcess;

typedef VOID (__stdcall *ReloadKeDetachProcess)();
ReloadKeDetachProcess RKeDetachProcess;

typedef VOID (__stdcall *ReloadKeAttachProcess)(
	__inout  PRKPROCESS Process
	);
ReloadKeAttachProcess RKeAttachProcess;

typedef void *(__stdcall* Reloadmemcpy)(
	void *dest,
	const void *src,
	size_t count
	);
Reloadmemcpy Rmemcpy;

typedef NTSTATUS (__stdcall *ReloadZwQueryInformationProcess)(
  __in       HANDLE ProcessHandle,
  __in       ULONG ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);
ReloadZwQueryInformationProcess RZwQueryInformationProcess;

typedef NTSTATUS (__stdcall *ReloadObOpenObjectByPointer)(
  __in      PVOID Object,
  __in      ULONG HandleAttributes,
  __in_opt  PACCESS_STATE PassedAccessState,
  __in      ACCESS_MASK DesiredAccess,
  __in_opt  POBJECT_TYPE ObjectType,
  __in      KPROCESSOR_MODE AccessMode,
  __out     PHANDLE Handle
);
ReloadObOpenObjectByPointer RObOpenObjectByPointer;

typedef NTSTATUS (__stdcall *ReloadObReferenceObjectByName)( 
	IN PUNICODE_STRING ObjectName, 
	IN ULONG Attributes, 
	IN PACCESS_STATE AccessState OPTIONAL, 
	IN ACCESS_MASK DesiredAccess OPTIONAL, 
	IN POBJECT_TYPE ObjectType, 
	IN KPROCESSOR_MODE AccessMode, 
	IN OUT PVOID ParseContext OPTIONAL, 
	OUT PVOID *Object 
	);
ReloadObReferenceObjectByName RObReferenceObjectByName;

typedef NTSTATUS (__stdcall *ReloadIoCallDriver)(
	__in     PDEVICE_OBJECT DeviceObject,
	__inout  PIRP Irp
	);
ReloadIoCallDriver RIoCallDriver;

typedef NTSTATUS (__stdcall *ReloadKeWaitForSingleObject)(
	__in      PVOID Object,
	__in      KWAIT_REASON WaitReason,
	__in      KPROCESSOR_MODE WaitMode,
	__in      BOOLEAN Alertable,
	__in_opt  PLARGE_INTEGER Timeout
	);
ReloadKeWaitForSingleObject RKeWaitForSingleObject;


typedef VOID (__stdcall *ReloadKeInitializeEvent)(
	__out  PRKEVENT Event,
	__in   EVENT_TYPE Type,
	__in   BOOLEAN State
	);
ReloadKeInitializeEvent RKeInitializeEvent;

typedef PDEVICE_OBJECT (__stdcall *ReloadIoGetRelatedDeviceObject)(
	__in  PFILE_OBJECT FileObject
	);
ReloadIoGetRelatedDeviceObject RIoGetRelatedDeviceObject;

typedef NTSTATUS (__stdcall *ReloadObReferenceObjectByHandle)(
	__in       HANDLE Handle,
	__in       ACCESS_MASK DesiredAccess,
	__in_opt   POBJECT_TYPE ObjectType,
	__in       KPROCESSOR_MODE AccessMode,
	__out      PVOID *Object,
	__out_opt  POBJECT_HANDLE_INFORMATION HandleInformation
	);
ReloadObReferenceObjectByHandle RObReferenceObjectByHandle;

typedef NTSTATUS (__stdcall *ReloadZwCreateFile)(
	__out     PHANDLE FileHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__out     PIO_STATUS_BLOCK IoStatusBlock,
	__in_opt  PLARGE_INTEGER AllocationSize,
	__in      ULONG FileAttributes,
	__in      ULONG ShareAccess,
	__in      ULONG CreateDisposition,
	__in      ULONG CreateOptions,
	__in_opt  PVOID EaBuffer,
	__in      ULONG EaLength
	);
ReloadZwCreateFile RZwCreateFile;

typedef NTSTATUS (__stdcall *ReloadZwOpenProcess)(
	__out     PHANDLE ProcessHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in      POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PCLIENT_ID ClientId
	);
ReloadZwOpenProcess RZwOpenProcess;

typedef NTSTATUS (__stdcall *ReloadZwTerminateProcess)(
	__in_opt  HANDLE ProcessHandle,
	__in      NTSTATUS ExitStatus
	);
ReloadZwTerminateProcess RZwTerminateProcess;

typedef NTSTATUS (__stdcall *ReloadZwClose)(
	__in  HANDLE Handle
	);
ReloadZwClose RZwClose;

typedef PVOID (__stdcall *ReloadMmGetSystemRoutineAddress)(
	__in  PUNICODE_STRING SystemRoutineName
	);
ReloadMmGetSystemRoutineAddress RMmGetSystemRoutineAddress;


typedef VOID (__stdcall *ReloadRtlInitUnicodeString)(
	__inout   PUNICODE_STRING DestinationString,
	__in_opt  PCWSTR SourceString
	);
ReloadRtlInitUnicodeString RRtlInitUnicodeString;


typedef BOOLEAN (__stdcall * ReloadMmIsAddressValid)(
	__in  PVOID VirtualAddress
	);
ReloadMmIsAddressValid RMmIsAddressValid;

typedef PVOID (__stdcall *ReloadExAllocatePoolWithTag)(
	__in  POOL_TYPE PoolType,
	__in  SIZE_T NumberOfBytes,
	__in  ULONG Tag
	);
ReloadExAllocatePoolWithTag RExAllocatePoolWithTag;

typedef PVOID (__stdcall *ReloadExAllocatePool)(
	__in  POOL_TYPE PoolType,
	__in  SIZE_T NumberOfBytes
	);
ReloadExAllocatePool RExAllocatePool;

typedef VOID (__stdcall *ReloadExFreePool)(
	__in  PVOID P
	);
ReloadExFreePool RExFreePool;

typedef NTSTATUS (__stdcall *ReloadZwOpenFile)(
	__out  PHANDLE FileHandle,
	__in   ACCESS_MASK DesiredAccess,
	__in   POBJECT_ATTRIBUTES ObjectAttributes,
	__out  PIO_STATUS_BLOCK IoStatusBlock,
	__in   ULONG ShareAccess,
	__in   ULONG OpenOptions
	);
ReloadZwOpenFile RZwOpenFile;

typedef NTSTATUS (__stdcall *ReloadZwCreateSection)(
	__out     PHANDLE SectionHandle,
	__in      ACCESS_MASK DesiredAccess,
	__in_opt  POBJECT_ATTRIBUTES ObjectAttributes,
	__in_opt  PLARGE_INTEGER MaximumSize,
	__in      ULONG SectionPageProtection,
	__in      ULONG AllocationAttributes,
	__in_opt  HANDLE FileHandle
	);
ReloadZwCreateSection RZwCreateSection;

typedef NTSTATUS (__stdcall *ReloadZwMapViewOfSection)(
	__in     HANDLE SectionHandle,
	__in     HANDLE ProcessHandle,
	__inout  PVOID *BaseAddress,
	__in     ULONG_PTR ZeroBits,
	__in     SIZE_T CommitSize,
	__inout  PLARGE_INTEGER SectionOffset,
	__inout  PSIZE_T ViewSize,
	__in     SECTION_INHERIT InheritDisposition,
	__in     ULONG AllocationType,
	__in     ULONG Win32Protect
	);
ReloadZwMapViewOfSection RZwMapViewOfSection;

typedef NTSTATUS (__stdcall *ReloadZwClose)(
	__in  HANDLE Handle
	);
ReloadZwClose RZwClose;

typedef NTSTATUS (__stdcall *ReloadZwQuerySystemInformation)(
	__in       ULONG SystemInformationClass,
	__inout    PVOID SystemInformation,
	__in       ULONG SystemInformationLength,
	__out_opt  PULONG ReturnLength
	);
ReloadZwQuerySystemInformation RZwQuerySystemInformation;

typedef NTSTATUS (__stdcall *ReloadZwOpenKey)(
	__out  PHANDLE KeyHandle,
	__in   ACCESS_MASK DesiredAccess,
	__in   POBJECT_ATTRIBUTES ObjectAttributes
	);
ReloadZwOpenKey RZwOpenKey;

typedef NTSTATUS (__stdcall *ReloadZwEnumerateKey)(
	__in       HANDLE KeyHandle,
	__in       ULONG Index,
	__in       KEY_INFORMATION_CLASS KeyInformationClass,
	__out_opt  PVOID KeyInformation,
	__in       ULONG Length,
	__out      PULONG ResultLength
	);
ReloadZwEnumerateKey RZwEnumerateKey;

typedef NTSTATUS (__stdcall *ReloadZwQueryValueKey)(
	__in       HANDLE KeyHandle,
	__in       PUNICODE_STRING ValueName,
	__in       KEY_VALUE_INFORMATION_CLASS KeyValueInformationClass,
	__out_opt  PVOID KeyValueInformation,
	__in       ULONG Length,
	__out      PULONG ResultLength
	);
ReloadZwQueryValueKey RZwQueryValueKey;

typedef NTSTATUS (__stdcall *ReloadZwCreateKey)(
	__out       PHANDLE KeyHandle,
	__in        ACCESS_MASK DesiredAccess,
	__in        POBJECT_ATTRIBUTES ObjectAttributes,
	__reserved  ULONG TitleIndex,
	__in_opt    PUNICODE_STRING Class,
	__in        ULONG CreateOptions,
	__out_opt   PULONG Disposition
	);
ReloadZwCreateKey RZwCreateKey;

typedef NTSTATUS (__stdcall *ReloadZwSetValueKey)(
	__in      HANDLE KeyHandle,
	__in      PUNICODE_STRING ValueName,
	__in_opt  ULONG TitleIndex,
	__in      ULONG Type,
	__in_opt  PVOID Data,
	__in      ULONG DataSize
	);
ReloadZwSetValueKey RZwSetValueKey;

typedef NTSTATUS (__stdcall *ReloadZwFlushKey)(
	__in  HANDLE KeyHandle
	);
ReloadZwFlushKey RZwFlushKey;

typedef PMDL (__stdcall *ReloadIoAllocateMdl)(
	__in_opt     PVOID VirtualAddress,
	__in         ULONG Length,
	__in         BOOLEAN SecondaryBuffer,
	__in         BOOLEAN ChargeQuota,
	__inout_opt  PIRP Irp
	);
ReloadIoAllocateMdl RIoAllocateMdl;

typedef VOID (__stdcall *ReloadMmBuildMdlForNonPagedPool)(
	__inout  PVOID MemoryDescriptorList
	);
ReloadMmBuildMdlForNonPagedPool RMmBuildMdlForNonPagedPool;

typedef VOID (__stdcall *ReloadMmProbeAndLockPages)(
	__inout  PVOID MemoryDescriptorList,
	__in     KPROCESSOR_MODE AccessMode,
	__in     LOCK_OPERATION Operation
	);
ReloadMmProbeAndLockPages RMmProbeAndLockPages;

typedef VOID (__stdcall *ReloadMmUnlockPages)(
	__inout  PVOID MemoryDescriptorList
	);
ReloadMmUnlockPages RMmUnlockPages;

typedef VOID (__stdcall *ReloadIoFreeMdl)(
	__in  PMDL Mdl
	);
ReloadIoFreeMdl RIoFreeMdl;

typedef VOID (__stdcall *ReloadIoStartTimer)(
	 __in  PDEVICE_OBJECT DeviceObject
	);
ReloadIoStartTimer RIoStartTimer;

typedef VOID (__stdcall *ReloadIoStopTimer)(
	__in  PDEVICE_OBJECT DeviceObject
	);
ReloadIoStopTimer RIoStopTimer;

#endif