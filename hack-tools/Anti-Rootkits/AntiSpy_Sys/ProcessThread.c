#include "ProcessThread.h"


extern ULONG_PTR  SSDTDescriptor;

ULONG_PTR ThreadListHeadOffsetOf_EPROCESS = 0x050;
ULONG_PTR ThreadListHeadOtherOffsetOf_EPROCESS  = 0x190;
ULONG_PTR ThreadListEntryOffsetOf_ETHREAD = 0x1b0;
ULONG_PTR StartAddressOffsetOf_ETHREAD = 0x224;
ULONG_PTR Win32StartAddressOffsetOf_ETHREAD = 0x228;
ULONG_PTR TebOffsetOf_ETHREAD = 0x020;   
ULONG_PTR PriorityOffsetOf_ETHREAD = 0x033;
ULONG_PTR ContextSwitchesOffsetOf_ETHREAD = 0x04c;
ULONG_PTR StateOffsetOf_ETHREAD = 0x2d;
ULONG_PTR PreviousModeOffsetOf_KTHREAD = 0x140;
ULONG_PTR JudgeThreadIsSuspendOffsetOf_ETHREAD = 0x1b9;
ULONG_PTR LdrInPebOffset = 0x00c;
ULONG_PTR ModListInLdrOffset = 0x00c;

ULONG_PTR  IndexOffsetOfFunction = 1;


PETHREAD g_EThread = NULL;

pfnNtTerminateThreadAddress NtTerminateThreadAddress = NULL;
pfnPspTerminateThreadByPointer PspTerminateThreadByPointerAddress = NULL;
pfnPsTerminateSystemThread PsTerminateSystemThreadAddress = NULL;
pfnNtSuspendThread NtSuspendThreadAddress = NULL;
pfnNtResumeThread  NtResumeThreadAddress = NULL;



VOID SetGlobalVar()
{
	PUCHAR a;

	NtTerminateThreadAddress = (pfnNtTerminateThreadAddress)GetFunctionAddressByNameFromSSDT("NtTerminateThread",SSDTDescriptor);
	PsTerminateSystemThreadAddress = (pfnPsTerminateSystemThread)GetFunctionAddressByNameFormNtos(L"PsTerminateSystemThread");
	NtSuspendThreadAddress = (pfnNtSuspendThread)GetFunctionAddressByNameFromSSDT("NtSuspendThread",SSDTDescriptor);
	NtResumeThreadAddress = (pfnNtResumeThread)GetFunctionAddressByNameFromSSDT("NtResumeThread",SSDTDescriptor);

	for(a=(PUCHAR)PsTerminateSystemThreadAddress;a<(PUCHAR)PsTerminateSystemThreadAddress+0x200;a++)
	{
		if( MmIsAddressValid(a) )
		{
			UCHAR b1=*a;
			UCHAR b2=*(a+1);
			ULONG_PTR Temp = 0;

			if( b1==0xe8&&b2==0xa4) //e8
			{
				memcpy(&Temp,a+1,4);   

				PspTerminateThreadByPointerAddress = (pfnPspTerminateThreadByPointer)((ULONG_PTR)Temp + (ULONG_PTR)a + 5);   

				break;
			}
		}
	}
}

NTSTATUS EnumProcessThread(PVOID InBuffer,PVOID OutBuffer, ULONG_PTR ulOutSize)
{
	
	NTSTATUS Status = STATUS_UNSUCCESSFUL;

	PEPROCESS EProcess = NULL;
	ULONG ulPid = 0;
	ULONG_PTR ulCount = (ulOutSize - sizeof(PROCESS_THREAD_INFORMATION)) / sizeof(PROCESS_THREAD_ENTRY_INFOR);


	SetGlobalVar();

	ulPid = (ULONG)InBuffer;

	DbgPrint("EnumProcessThread\n");

	if (ulPid)
	{
		Status = PsLookupProcessByProcessId((HANDLE)ulPid, &EProcess);

		if (!NT_SUCCESS(Status))
		{
			return STATUS_UNSUCCESSFUL;
		}
		ObfDereferenceObject(EProcess);
		GetProcessThreads(EProcess, (PPROCESS_THREAD_INFORMATION)OutBuffer, ulCount);   

		GetThreadsModule(ulPid,OutBuffer);
	

		
	}
	return Status;
}

VOID GetThreadsModule(ULONG ulPid,PVOID OutBuffer)
{
	ULONG ulSize = 0;
	
	PPROCESS_MODULE_INFORMATION ModuleInformation = NULL;
	ulSize = sizeof(PROCESS_MODULE_INFORMATION) + MAX_MODULE_COUNT * sizeof(PROCESS_MODULE_ENTRY_INFOR);
	ModuleInformation = (PPROCESS_MODULE_INFORMATION)ExAllocatePool(PagedPool,ulSize);
	if(ModuleInformation == NULL)
	{
		DbgPrint("ModuleInfor Is NULL\r\n");
		return STATUS_UNSUCCESSFUL;
	}
	memset(ModuleInformation,0,ulSize);
/*
	PMODULEINFOR ModuleInfor = NULL;
	ModuleInfor = (PMODULEINFOR)ExAllocatePool(PagedPool,sizeof(MODULEINFOR)*MAX_MODULE_COUNT);
	if(ModuleInfor == NULL)
	{
		DbgPrint("ModuleInfor Is NULL\r\n");
		return STATUS_UNSUCCESSFUL;
	}
	memset(ModuleInfor,0,sizeof(MODULEINFOR)*MAX_MODULE_COUNT);
	EnumModule(ulPid,ModuleInfor);*/
	EnumMoudleByNtQueryVirtualMemory(ulPid,ModuleInformation,ulSize);
	MatchModuleToThread(OutBuffer,ModuleInformation);
}

VOID GetProcessThreads(PEPROCESS EProcess,  PPROCESS_THREAD_INFORMATION OutBuffer, ULONG_PTR ulCount)
{
	if (EProcess && MmIsAddressValid(EProcess))
	{
		PLIST_ENTRY  ListEntry = (PLIST_ENTRY)((ULONG_PTR)EProcess + ThreadListHeadOffsetOf_EPROCESS);
		if (ListEntry && MmIsAddressValid(ListEntry) && MmIsAddressValid(ListEntry->Flink))
		{
			KIRQL     OldIrql = KeRaiseIrqlToDpcLevel();
			ULONG_PTR ulMaxThreadCount = PAGE_SIZE * 2;   
			PLIST_ENTRY ListNext = ListEntry->Flink;
			while (MmIsAddressValid(ListNext) && ListNext != ListEntry && ulMaxThreadCount--)
			{
				PETHREAD EThread = (PETHREAD)((ULONG_PTR)ListNext - ThreadListEntryOffsetOf_ETHREAD);

				InsertThreadInforToRing3Memroy(EThread, EProcess, OutBuffer, ulCount);
				ListNext = ListNext->Flink;
			}

			KeLowerIrql(OldIrql);
		}

		ListEntry = (PLIST_ENTRY)((ULONG)EProcess + ThreadListHeadOtherOffsetOf_EPROCESS);
		if (ListEntry && MmIsAddressValid(ListEntry) && MmIsAddressValid(ListEntry->Flink))
		{
			KIRQL     OldIrql = KeRaiseIrqlToDpcLevel();
			ULONG_PTR ulMaxThreadCount = PAGE_SIZE * 2;
			PLIST_ENTRY ListNext = ListEntry->Flink;
			while (MmIsAddressValid(ListNext) && ListNext != ListEntry && ulMaxThreadCount--)
			{
				PETHREAD EThread = (PETHREAD)((ULONG_PTR)ListNext - ThreadListEntryOffsetOf_ETHREAD);
				InsertThreadInforToRing3Memroy(EThread,EProcess, OutBuffer,ulCount);
				ListNext = ListNext->Flink;
			}

			KeLowerIrql(OldIrql);
		}
	}
}

NTSTATUS JudgeThreadIsSuspend(PETHREAD EThread,ULONG_PTR* ulIsThreadSuspend)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	CHAR bIsSuspend =*((PUCHAR) ((ULONG_PTR)EThread+JudgeThreadIsSuspendOffsetOf_ETHREAD ) );

	
	DbgPrint("%p,%d\r\n",  (ULONG_PTR)EThread+JudgeThreadIsSuspendOffsetOf_ETHREAD  ,bIsSuspend);

	*ulIsThreadSuspend = (ULONG_PTR)bIsSuspend;

	/*if(bIsSuspend == '0')
	{
		*ulIsThreadSuspend = 0;
		Status = STATUS_SUCCESS;
	}

	else if(bIsSuspend>48&&bIsSuspend<58)
	{
		*ulIsThreadSuspend = 1;
		Status = STATUS_SUCCESS;
	}
	else
	{
		Status = STATUS_UNSUCCESSFUL;
	}*/

	return STATUS_SUCCESS;
}

NTSTATUS PspSuspendOrResumeThread(PETHREAD EThread, BOOLEAN bSuspend)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	HANDLE hThread;
	PETHREAD CurrentThread = NULL;
	CHAR PreMode = 0;

	if (EThread==0)
	{
		return Status;
	}

	//
	// 如果是当前线程就直接返回了
	//
	CurrentThread = PsGetCurrentThread();
	if (CurrentThread == EThread)
	{
		Status = STATUS_SUCCESS;   //不挂自己
	}
	else
	{
		Status = ObOpenObjectByPointer(EThread, 
			OBJ_KERNEL_HANDLE | OBJ_CASE_INSENSITIVE, 
			NULL, 
			GENERIC_ALL, 
			*PsThreadType, 
			KernelMode, 
			&hThread
			);

		if (NT_SUCCESS(Status))
		{
			ULONG_PTR PreviousSuspendCount = -1;
			PreMode = ChangePreMode(CurrentThread);
			if (bSuspend)
			{
				Status = NtSuspendThreadAddress(hThread, &PreviousSuspendCount);
				
			}
			else
			{
				Status = NtResumeThreadAddress(hThread, &PreviousSuspendCount);
				
			}

			NtClose(hThread);
			RecoverPreMode(CurrentThread, PreMode);
		}
	}

	return Status;
}




NTSTATUS KillProcessThread(PETHREAD EThread)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	if (EThread)
	{

		HANDLE hThread = NULL;
		Status = ObOpenObjectByPointer(EThread, OBJ_KERNEL_HANDLE, NULL, GENERIC_ALL,*PsThreadType, KernelMode, &hThread);
		if (NT_SUCCESS(Status))
		{
			PETHREAD CurrentThread = PsGetCurrentThread();
			CHAR PreMode = ChangePreMode(CurrentThread);

			Status = NtTerminateThreadAddress(hThread, 0);  //句柄

			NtClose(hThread);
			RecoverPreMode(CurrentThread, PreMode);
		}

	}

	return Status;
}

NTSTATUS ForceKillProcessThread(PETHREAD EThread)
{
	NTSTATUS Status = STATUS_UNSUCCESSFUL;
	if (EThread)
	{

		HANDLE hThread = NULL;
		Status = ObOpenObjectByPointer(EThread, OBJ_KERNEL_HANDLE, NULL, GENERIC_ALL,*PsThreadType, KernelMode, &hThread);
		if (NT_SUCCESS(Status))
		{
			PETHREAD CurrentThread = PsGetCurrentThread();
			CHAR PreMode = ChangePreMode(CurrentThread);

			Status = PspTerminateThreadByPointerAddress(EThread,0,1);  //句柄

			NtClose(hThread);
			RecoverPreMode(CurrentThread, PreMode);
		}

	}

	return Status;
}




VOID InsertThreadInforToRing3Memroy(PETHREAD EThread, PEPROCESS EProcess, PPROCESS_THREAD_INFORMATION OutBuffer, ULONG ulCount)
{
	g_EThread = EThread;
	if (EThread && EProcess && MmIsAddressValid((PVOID)EThread))
	{ 

		PEPROCESS Temp = NULL;

		if (IoThreadToProcess)   
		{
			Temp = IoThreadToProcess(EThread);   
		}


		if (EProcess == Temp &&
			!IsThreadInRing3Memory(EThread,OutBuffer,ulCount) && 
			NT_SUCCESS(ObReferenceObjectByPointer(EThread, 0, NULL, KernelMode)))
		{

			if (ulCount > OutBuffer->NumberOfThreads)
			{
				if (PsGetThreadId)
				{
					OutBuffer->Threads[OutBuffer->NumberOfThreads].Tid = (ULONG_PTR)PsGetThreadId(EThread);
				}

				OutBuffer->Threads[OutBuffer->NumberOfThreads].Thread = (ULONG_PTR)EThread; 
				if(*((ULONG_PTR*)((ULONG_PTR)EThread+Win32StartAddressOffsetOf_ETHREAD)) < 0x400000)
				{
					OutBuffer->Threads[OutBuffer->NumberOfThreads].Win32StartAddress = *((ULONG_PTR*)((ULONG_PTR)EThread+StartAddressOffsetOf_ETHREAD));
				}
				else
				{
					OutBuffer->Threads[OutBuffer->NumberOfThreads].Win32StartAddress = *((ULONG_PTR*)((ULONG_PTR)EThread+Win32StartAddressOffsetOf_ETHREAD));
				}
				
				OutBuffer->Threads[OutBuffer->NumberOfThreads].Teb = *(PULONG_PTR)((ULONG_PTR)EThread + TebOffsetOf_ETHREAD);
				OutBuffer->Threads[OutBuffer->NumberOfThreads].Priority = *(PUCHAR)((ULONG_PTR)EThread + PriorityOffsetOf_ETHREAD);
				OutBuffer->Threads[OutBuffer->NumberOfThreads].ContextSwitches = *(PULONG)((ULONG_PTR)EThread + ContextSwitchesOffsetOf_ETHREAD);
				OutBuffer->Threads[OutBuffer->NumberOfThreads].State = *(PUCHAR)((ULONG_PTR)EThread + StateOffsetOf_ETHREAD);
			}

			OutBuffer->NumberOfThreads++;
			ObfDereferenceObject(EThread);
		}
	} 
}


BOOLEAN IsThreadInRing3Memory(PETHREAD EThread, PPROCESS_THREAD_INFORMATION OutBuffer, ULONG ulCount)
{
	BOOLEAN bRet = FALSE;
	ULONG_PTR ulTemp = ulCount > OutBuffer->NumberOfThreads ? OutBuffer->NumberOfThreads : ulCount;
	ULONG_PTR i = 0;

	if (!EThread || !OutBuffer)
	{
		return TRUE;
	}



	for (i = 0; i < ulTemp; i++)
	{
		if (OutBuffer->Threads[i].Thread == (ULONG_PTR)EThread)
		{
			bRet = TRUE;
			break;
		}
	}

	return bRet; 
}

NTSTATUS MatchModuleToThread(PVOID OutputBuffer,PPROCESS_MODULE_INFORMATION ModuleInfor)
{
	NTSTATUS Status = STATUS_SUCCESS;
	ULONG ModuleCount = 0;
	ULONG ThreadCount = 0;
	ObReferenceObjectByPointer(g_EThread, 0, NULL, KernelMode);
	for(ThreadCount=0;ThreadCount<((PPROCESS_THREAD_INFORMATION)OutputBuffer)->NumberOfThreads;ThreadCount++)
	{
		while(ModuleCount<MAX_MODULE_COUNT)
		{
			/*if(((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Win32StartAddress>ModuleInfor[ModuleCount].ModuleBase
				&&((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Win32StartAddress<ModuleInfor[ModuleCount].ModuleBase+ModuleInfor[ModuleCount].ModuleSize)
			{ 
				wcscpy(((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Module,ModuleInfor[ModuleCount].ModulePath);

				DbgPrint("拷进去:%S\r\n",((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Module);
				break;
			}*/

			if(((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Win32StartAddress>ModuleInfor->Module[ModuleCount].ModuleBase
				&&((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Win32StartAddress<ModuleInfor->Module[ModuleCount].ModuleBase+ModuleInfor->Module[ModuleCount].ModuleSize)
			{ 
				wcscpy(((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Module,ModuleInfor->Module[ModuleCount].ModulePath);

				DbgPrint("拷进去:%S\r\n",((PPROCESS_THREAD_INFORMATION)OutputBuffer)->Threads[ThreadCount].Module);
				break;
			}

			ModuleCount++;
		}

		if(ModuleCount == MAX_MODULE_COUNT)
		{
			Status = STATUS_UNSUCCESSFUL;
		}

		ModuleCount = 0;

	}

	ObfDereferenceObject(g_EThread);
	ExFreePool(ModuleInfor);
	return Status;

}
NTSTATUS EnumModule(ULONG_PTR ProcessID,PPROCESS_MODULE_INFORMATION ModuleInfor)
{

	//通过ID 获得EProcess

	NTSTATUS    Status;
	ULONG_PTR   Peb;
	PEPROCESS   EProcess = NULL;
	PETHREAD    EThread  = NULL;
	PEPROCESS   Temp     = NULL;
	ULONG_PTR   i = 0;
	KAPC_STATE  ApcState;
	PLIST_ENTRY ListHead;
	PLIST_ENTRY ListTemp;
	ULONG_PTR ldr = 0;
	WCHAR*    wzTemp = NULL;

	ULONG_PTR NumberOfModule = 0;

	Status = PsLookupProcessByProcessId((HANDLE)ProcessID,&EProcess);


	if (!NT_SUCCESS(Status))
	{
		return  FALSE;
	}

	//获得PEB  

	Peb = (ULONG_PTR)PsGetProcessPeb(EProcess);  

	if (!Peb)
	{
		goto End;
	}
	KeStackAttachProcess(EProcess, &ApcState);


	//获得PEB  ldr

	ldr = *((ULONG_PTR*)(Peb +LdrInPebOffset));

	//通过ldr 获得


	ListHead = ListTemp = (PLIST_ENTRY)((ULONG_PTR*)(ldr + ModListInLdrOffset));


	ListTemp = ListHead->Flink;
	while (ListTemp!=ListHead)
	{
		//打印信息：基址、大小、DLL路径
		DbgPrint("[MODULE]Base=%p Size=%p Path=%wZ\n",
			(PVOID)(((PLDR_DATA_TABLE_ENTRY32)ListTemp)->DllBase),
			(ULONG)(((PLDR_DATA_TABLE_ENTRY32)ListTemp)->SizeOfImage),
			&(((PLDR_DATA_TABLE_ENTRY32)ListTemp)->FullDllName));

		ModuleInfor->Module[NumberOfModule].ModuleBase = (ULONG_PTR)(((PLDR_DATA_TABLE_ENTRY32)ListTemp)->DllBase);
		ModuleInfor->Module[NumberOfModule].ModuleSize = (ULONG_PTR)(((PLDR_DATA_TABLE_ENTRY32)ListTemp)->SizeOfImage);

		UnicodeStringToWchar(&(((PLDR_DATA_TABLE_ENTRY32)ListTemp)->FullDllName),&wzTemp);
		wcscpy(ModuleInfor->Module[NumberOfModule].ModulePath,wzTemp);

		if(wzTemp!=NULL)
		{
			ExFreePool(wzTemp);
			wzTemp = NULL;
		}
		NumberOfModule++;	



		ListTemp = ListTemp->Flink;
		//测试下一个模块信息的可读性
	}



	KeUnstackDetachProcess(&ApcState);

End:



	ObDereferenceObject(EProcess);
	return TRUE;
}

BOOLEAN	UnicodeStringToWchar(UNICODE_STRING* uniSource,WCHAR** wzDest)
{
	ULONG ulSize = 0;
	WCHAR wzBuffer[256] = {0};
	if(uniSource==NULL)
	{
		return FALSE;
	}
	
	wcscpy(wzBuffer,uniSource->Buffer);

	ulSize  = (uniSource->MaximumLength);

	*wzDest =(WCHAR*) ExAllocatePool(PagedPool,ulSize);

	if(*wzDest==NULL)
	{
		return FALSE;
	}

	memset(*wzDest,0,ulSize);

	wcscpy(*wzDest,wzBuffer);

	
	return TRUE;
}