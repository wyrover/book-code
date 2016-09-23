#ifndef _SAFE_SYSTEM_H_
#define _SAFE_SYSTEM_H_

#include "ntifs.h"
#include "services.h"

//≤‚ ‘
//#include "iotimer.h"

BOOL DebugOn = FALSE;
BOOL bKernelBooting = FALSE;

PDRIVER_OBJECT PDriverObject;
DWORD RetAddress;

PEPROCESS SystemEProcess;

ULONG ulMyDriverBase;
ULONG ulMyDriverSize;

NTSTATUS ReLoadNtos(
	PDRIVER_OBJECT   DriverObject,
	DWORD RetAddress
	);

NTSTATUS LookupProcessByName(
	IN PCHAR pcProcessName,
	OUT PEPROCESS *pEprocess
	);

VOID WaitMicroSecond(
	LONG MicroSeconds
	);

BOOL DeleteRegKey(WCHAR *ServicesKey);
BOOL IsRegKeyInSystem(PWCHAR ServicesKey);

BOOL Safe_CreateValueKey(
	PWCHAR SafeKey,
	ULONG Reg_Type,
	PWCHAR ValueName,
	PWCHAR Value
	);
BOOL KeBugCheckCreateValueKey(PWCHAR SafeKey);
BOOL IsFileInSystem(WCHAR *lpwzFile);
ULONG PsGetProcessCount();

VOID ImageNotify(
	PUNICODE_STRING  FullImageName,
	HANDLE  ProcessId,
	PIMAGE_INFO  ImageInfo
	);

//≤‚ ‘ 
//VOID EnumKernelModule(PDRIVER_OBJECT PDriverObject,PSYSINFO SysModuleInfo);
//NTSTATUS ReloadWin32K();
//PEPROCESS GetIdleProcess();
//VOID QueryStartup(PSTARTUP_INFO Startup);
//VOID QueryWorkQueue(PWORKQUEUE WorkQueueThread);
//NTSTATUS SuspendThread(PETHREAD Thread);
// PVOID GetPsXXXXXThread(CHAR *Func);
// NTSTATUS ResumeThread(PETHREAD Thread);
//NTSTATUS ReadThreadStack(PETHREAD Thread,PTHREAD_STACK StackOfThread);
//VOID LookupKernelDataInfo(PVOID LookupBase,ULONG ulLookupSize,PVOID LookupKernelData);
//BOOL GetShadowSSDTFunctionIndexByName(char *FunctionName,int *Index);
//VOID QueryIoTimer(PMyIoTimer IoTimer);
VOID MsGetMsgHookInfo();

#endif