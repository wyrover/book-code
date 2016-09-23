#include "KernelThread.h"

__declspec(naked) NTSTATUS PsCreateSystemThreadHookZone(,...)
{
	_asm
	{
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		jmp [PsCreateSystemThreadRet];
	}
}
NTSTATUS __stdcall NewPsCreateSystemThread(
	OUT PHANDLE ThreadHandle,
	IN ULONG DesiredAccess,
	IN POBJECT_ATTRIBUTES ObjectAttributes OPTIONAL,
	IN HANDLE ProcessHandle OPTIONAL,
	OUT PCLIENT_ID ClientId OPTIONAL,
	IN PKSTART_ROUTINE StartRoutine,
	IN PVOID StartContext
	)
{
	PSCREATESYSTEMTHREAD OldPsCreateSystemThread;
	CHAR lpszKernelModule[256]; 
	PETHREAD Thread;
	NTSTATUS st,status;
	int i=0;
	KPROCESSOR_MODE PreviousMode;

	if (bDisKernelThread == FALSE)  //禁止创建系统
	{
		return STATUS_UNSUCCESSFUL;
	}
	OldPsCreateSystemThread = (PSCREATESYSTEMTHREAD)PsCreateSystemThreadHookZone;
	st = OldPsCreateSystemThread(
		ThreadHandle,
		DesiredAccess,
		ObjectAttributes,
		ProcessHandle,
		ClientId,
		StartRoutine,
		StartContext
		);
	if (!NT_SUCCESS(st))
		return st;

	if (!bIsInitSuccess)
		return st;

	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		return st;
	}
	if (KernelThread->ulCount <= 1000)
	{
		for(i=0;i<ThreadCount;i++)
		{
			if (KernelThread->KernelThreadInfo[i].ThreadStart == (ULONG)StartRoutine)
			{
				//线程存在，就不需要记录了
				return st;
			}
		}
		memset(KernelThread->KernelThreadInfo[ThreadCount].lpszThreadModule,0,sizeof(KernelThread->KernelThreadInfo[ThreadCount].lpszThreadModule));
		if (!IsAddressInSystem(
			(ULONG)StartRoutine,
			&ulThreadModuleBase,
			&ulThreadModuleSize,
			KernelThread->KernelThreadInfo[ThreadCount].lpszThreadModule))
		{
			strcat(KernelThread->KernelThreadInfo[ThreadCount].lpszThreadModule,"Unknown");
			KernelThread->KernelThreadInfo[ThreadCount].ulHideType = 1;  //隐藏线程

		}else
			KernelThread->KernelThreadInfo[ThreadCount].ulHideType = 0;  //默认正常

		KernelThread->KernelThreadInfo[ThreadCount].ulStatus = 0;
		KernelThread->KernelThreadInfo[ThreadCount].ThreadStart = (ULONG)StartRoutine;

		status = ObReferenceObjectByHandle(
			*ThreadHandle,
			THREAD_ALL_ACCESS,
			*PsThreadType,
			KernelMode,
			(PVOID *)&Thread,
			NULL);
		if (NT_SUCCESS(status))
		{
			ObDereferenceObject(Thread);
			KernelThread->KernelThreadInfo[ThreadCount].KernelThread = (ULONG)Thread;
		}
		if (DebugOn)
			KdPrint(("create thread :%08x:%s",KernelThread->KernelThreadInfo[ThreadCount].ThreadStart,KernelThread->KernelThreadInfo[ThreadCount].lpszThreadModule));
		ThreadCount++;
	}
	return st;
}
BOOL InitKernelThreadData()
{
	BOOL bRetOK = FALSE;
	UNICODE_STRING functionName;
	PUCHAR ulPsCreateSystemThread;
	ULONG ulReloadPsCreateSystemThread;

	ThreadCount = 0;
	//为内核线程创建一个表
	KernelThread = (PKERNEL_THREAD_INFO)ExAllocatePool(NonPagedPool,sizeof(KERNEL_THREAD_INFO)*256);
	if (!KernelThread)
	{
		if (DebugOn)
			KdPrint(("KernelThread failed"));
		return FALSE;
	}
	memset(KernelThread,0,sizeof(KERNEL_THREAD_INFO)*256);

	
	RtlInitUnicodeString(&functionName,L"PsCreateSystemThread");
	ulPsCreateSystemThread = (DWORD)MmGetSystemRoutineAddress(&functionName);
	if (ulPsCreateSystemThread)
	{
		ulReloadPsCreateSystemThread = (ULONG)ulPsCreateSystemThread - SystemKernelModuleBase + ImageModuleBase;

		//屌，已经被hook过了
		if (*ulPsCreateSystemThread == 0xE9)
		{
			//获取被hook的跳转地址
			ulPsCreateSystemThread = *(PULONG)((ULONG)ulPsCreateSystemThread+1)+(ULONG)((ULONG)ulPsCreateSystemThread+5);
		}
		bRetOK = HookFunctionByHeaderAddress(
			ulReloadPsCreateSystemThread,
			ulPsCreateSystemThread,
			(PVOID)PsCreateSystemThreadHookZone,
			&PsCreateSystemThreadPatchCodeLen,
			&PsCreateSystemThreadRet
			);
		if (bRetOK == TRUE)
		{
			bRetOK = HookFunctionByHeaderAddress(
				(DWORD)NewPsCreateSystemThread,
				ulReloadPsCreateSystemThread,
				(PVOID)PsCreateSystemThreadHookZone,
				&PsCreateSystemThreadPatchCodeLen,
				&PsCreateSystemThreadRet
				);
			if (bRetOK == TRUE)
			{
				//初始化OK
				if (DebugOn)
					KdPrint(("Init Kernel Thread success"));
			}
		}
	}
	return bRetOK;
}