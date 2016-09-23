#ifndef _KERNEL_THREAD_H_
#define _KERNEL_THREAD_H_

#include "ntifs.h"
#include "ntos.h"

int PsCreateSystemThreadPatchCodeLen;
PVOID PsCreateSystemThreadRet;

typedef struct _KERNEL_INFO{
	ULONG ulHideType;           //0 正常模块，在driver_object中有   1 隐藏模块
	ULONG ulStatus;             //线程状态，0运行，1退出
	ULONG KernelThread;         //ETHREAD
	ULONG ThreadStart;          //开始
	CHAR lpszThreadModule[256];

}KERNEL_INFO,*PKERNEL_INFO;

typedef struct _KERNEL_THREAD_INFO {          //InlineHook
	ULONG ulCount;
	KERNEL_INFO KernelThreadInfo[1];
} KERNEL_THREAD_INFO, *PKERNEL_THREAD_INFO;

PKERNEL_THREAD_INFO KernelThread;

ULONG ThreadCount;
ULONG ulThreadModuleBase;
ULONG ulThreadModuleSize;

extern BOOL bIsInitSuccess;
extern BOOL bDisKernelThread;

typedef NTSTATUS (__stdcall *PSCREATESYSTEMTHREAD)(
	OUT PHANDLE ThreadHandle,
	IN ULONG DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN HANDLE ProcessHandle OPTIONAL,
	OUT PCLIENT_ID ClientId OPTIONAL,
	IN PKSTART_ROUTINE StartRoutine,
	IN PVOID StartContext
	);

BOOL IsAddressInSystem(
	ULONG ulDriverBase,
	ULONG *ulSysModuleBase,
	ULONG *ulSize,
	char *lpszSysModuleImage
	);

KPROCESSOR_MODE KeGetPreviousMode();

#endif