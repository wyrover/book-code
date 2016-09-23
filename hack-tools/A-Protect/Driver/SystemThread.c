#include "SystemThread.h"

NTSTATUS ReadThreadStack(PETHREAD Thread,PTHREAD_STACK StackOfThread)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	ULONG ulKernelStackOffset = 0;
	ULONG ulInitialStackOffset = 0;
	ULONG KernelStack;
	ULONG InitialStack;
	ULONG ulLookupStackSize;
	ULONG i=0,x=0;

	if (DebugOn)
		KdPrint(("Thread:%08x",Thread));
	//如果线程不可用
	if (!MmIsAddressValidEx(Thread)){
		return status;
	}
	//如果线程结束了
	if (PsIsThreadTerminating(Thread)){
		return status;
	}

	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		ulInitialStackOffset = 0x018;
		ulKernelStackOffset = 0x028;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ulInitialStackOffset = 0x018;
		ulKernelStackOffset = 0x20;
		break;
	case WINDOWS_VERSION_7_7000:
		ulInitialStackOffset = 0x028;
		ulKernelStackOffset = 0x30;
		break;
	case WINDOWS_VERSION_7_7600_UP:
		ulInitialStackOffset = 0x028;
		ulKernelStackOffset = 0x30;
		break;
	}
	if (!ulKernelStackOffset ||
		!ulInitialStackOffset){
			return status;
	}

	if (!MmIsAddressValidEx(((ULONG)Thread+ulKernelStackOffset)) ||
		!MmIsAddressValidEx(((ULONG)Thread+ulInitialStackOffset))){
		return status;
	}
	KernelStack = *(PULONG)((ULONG)Thread+ulKernelStackOffset);
	InitialStack = *(PULONG)((ULONG)Thread+ulInitialStackOffset);
	if (InitialStack < KernelStack){
		return status;
	}
	ulLookupStackSize = InitialStack;

	if (DebugOn)
		KdPrint(("InitialStack:%08x\n",InitialStack));
	if (DebugOn)
		KdPrint(("KernelStack:%08x\n",KernelStack));

	StackOfThread->ulKernelStack = ulKernelStackOffset;
	StackOfThread->KernelStack   = KernelStack;
	StackOfThread->ulKthread     = (ULONG)Thread;

	for (i=KernelStack;i<ulLookupStackSize;i+=0x10)
	{
		if (MmIsAddressRangeValid(i,0xc))
		{
			StackOfThread->StackInfo[x].ulAddress = i;
			StackOfThread->StackInfo[x].ulStack1 = *(PULONG)(i);
			StackOfThread->StackInfo[x].ulStack2 = *(PULONG)(i+4);
			StackOfThread->StackInfo[x].ulStack3 = *(PULONG)(i+8);
			StackOfThread->StackInfo[x].ulStack4 = *(PULONG)(i+0xc);
			x++;
			StackOfThread->ulCount = x;
		}
	}
}
UCHAR GetThreadStatus(PETHREAD Thread)
{
	ULONG ulStatus;
	UCHAR ulRet = 0;
	WIN_VER_DETAIL WinVer;

	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		ulStatus = 0x02d;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ulStatus = 0x04c;
		break;
	case WINDOWS_VERSION_7_7000:
	case WINDOWS_VERSION_7_7600_UP:
		ulStatus = 0x068;
		break;
	}
	if (MmIsAddressValidEx(((ULONG)Thread + ulStatus)))
		ulRet = *(UCHAR *)((ULONG)Thread + ulStatus);

	return ulRet;
}
PVOID GetPsXXXXXThread(CHAR *Func)
{
	int SSDTIndex;
	ULONG ulOldXXXXXThread;
	ULONG NtRealXXXXXThread;
	ULONG ulPsXXXXXThread;
	int count=0,count1=0;
	ULONG ulCodeSize,ulPsCodeSize;
	PUCHAR i=0,x=0;
	BOOL bGetAddr = FALSE;
	WIN_VER_DETAIL WinVer;

	if (!GetFunctionIndexByName(Func,&SSDTIndex))
		return 0;

	if (DebugOn) 
		KdPrint(("%s Index:%d\n",Func,SSDTIndex));

	if (SSDTIndex >= 0 &&
		SSDTIndex <= OriginalServiceDescriptorTable->TableSize)
	{
		NtRealXXXXXThread = OriginalServiceDescriptorTable->ServiceTable[SSDTIndex];
		if (MmIsAddressValidEx(NtRealXXXXXThread))
		{
			ulOldXXXXXThread = NtRealXXXXXThread - (ULONG)ImageModuleBase + SystemKernelModuleBase;
			if (MmIsAddressValidEx(ulOldXXXXXThread))
			{
				ulCodeSize = SizeOfProc((PVOID)ulOldXXXXXThread);

				if (DebugOn) 
					DbgPrint("ulOldNtSuspendThread:%08x\r\n",ulOldXXXXXThread);

				for (i=(ULONG)ulOldXXXXXThread;i < i+ulCodeSize;i++)
				{
					if (*i == 0xe8)
					{
						WinVer=GetWindowsVersion();
						switch (WinVer)
						{
						case WINDOWS_VERSION_XP:
						case WINDOWS_VERSION_7_7000:
						case WINDOWS_VERSION_7_7600_UP:
							if (count == 2)  //第二个call（e8）就是了
							{
								bGetAddr = TRUE;
								ulPsXXXXXThread = *(PULONG)(i+1)+(ULONG)(i+5);
								if (MmIsAddressValidEx(ulPsXXXXXThread))
								{
									if (DebugOn) 
										DbgPrint("%s:%08x\r\n",Func,ulPsXXXXXThread);

									return ulPsXXXXXThread;
								}
							}
							break;
						case WINDOWS_VERSION_2K3_SP1_SP2:
							if (count == 3)  //第二个call（e8）就是了
							{
								bGetAddr = TRUE;
								ulPsXXXXXThread = *(PULONG)(i+1)+(ULONG)(i+5);
								if (MmIsAddressValidEx(ulPsXXXXXThread))
								{
									if (DebugOn) 
										DbgPrint("%s:%08x\r\n",Func,ulPsXXXXXThread);

									return ulPsXXXXXThread;
								}
							}
							break;
						}
						count++;
					}
					if (bGetAddr)
						break;
				}
			}
		}
	}
	return 0;
}
//恢复线程运行
NTSTATUS ResumeThread(PETHREAD Thread)
{
	NTSTATUS status=STATUS_UNSUCCESSFUL;
	KPROCESSOR_MODE PreviousMode;
	PSRESUMETHREAD PsResumeThread = 0;
	KERESUMETHREAD KeResumeThread = 0;
	NTRESUMETHREAD ulNtResumeThread;
	WIN_VER_DETAIL WinVer;
	int SSDTIndex;
	HANDLE hThread;
	ULONG SuspendCount=0;
	int i=0;

	//如果线程不可用
	if (!MmIsAddressValidEx(Thread)){
		return status;
	}
	//如果线程结束了
	if (PsIsThreadTerminating(Thread)){
		return status;
	}
	IniOffsetObject();

	if (MmIsAddressValidEx(((ULONG)Thread+ulSuspendCount)))
	{
		SuspendCount = *(PULONG)((ULONG)Thread+ulSuspendCount);
	}
	//如果为0说明线程不是暂停状态
	if (!SuspendCount)
		return status;

	WinVer=GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
	case WINDOWS_VERSION_2K3_SP1_SP2:
		PsResumeThread = (PSRESUMETHREAD)GetPsXXXXXThread("NtResumeThread");
		if (PsResumeThread){
			KPROCESSOR_MODE PreviousMode = ExGetPreviousMode();

			//根据暂停次数循环恢复线程运行
			for (i=0;i<SuspendCount;i++)
			{
				status = PsResumeThread(Thread,&PreviousMode);
			}
		}
		break;
	case WINDOWS_VERSION_7_7000:
	case WINDOWS_VERSION_7_7600_UP:
		//win7下得到的是KeResumeThread
// 		KeResumeThread = (KERESUMETHREAD)GetPsXXXXXThread("NtResumeThread");
// 		if (KeResumeThread){
// 			status = KeResumeThread(Thread);
// 		}
		if (GetFunctionIndexByName("NtResumeThread",&SSDTIndex))
		{
			if (DebugOn) 
				KdPrint(("SSDTIndex:%d\n",SSDTIndex));

			if (SSDTIndex >= 0 &&
				SSDTIndex <= OriginalServiceDescriptorTable->TableSize)
			{
				ulNtResumeThread = OriginalServiceDescriptorTable->ServiceTable[SSDTIndex];
				if (MmIsAddressValidEx(ulNtResumeThread))
				{
					if (DebugOn) 
						KdPrint(("ulNtResumeThread:%08x\n",ulNtResumeThread));

					/*
					//
					//  Make sure the caller is asking for handle attributes that are
					//  valid for the given object type
					//

					if (ObjectHeader->Type->TypeInfo.InvalidAttributes & HandleAttributes) {

					if (AccessState == &LocalAccessState) {

					    SeDeleteAccessState( AccessState );
					}

					ObDereferenceObject( Object );

					return( STATUS_INVALID_PARAMETER );
					}
					*/
					//上面是wrk的检查，对Thread的type的typeinfo的InvalidAttributes做检查，因此ObOpenObjectByPointer的属性应该是 OBJ_KERNEL_HANDLE & 0（这样就具备两个属性了），不然返回句柄无效
					ReLoadNtosCALL(&RObOpenObjectByPointer,L"ObOpenObjectByPointer",SystemKernelModuleBase,ImageModuleBase);
					if (!RObOpenObjectByPointer){
						return status;
					}
					status = RObOpenObjectByPointer(
						Thread,          // Object    
						OBJ_KERNEL_HANDLE & 0,  // HandleAttributes    
						NULL,               // PassedAccessState OPTIONAL    
						THREAD_ALL_ACCESS,       // DesiredAccess    
						*PsThreadType,     // ObjectType    
						KernelMode,         // AccessMode    
						&hThread);    
					if (NT_SUCCESS(status))
					{
						if (DebugOn) 
							KdPrint(("[handle:%08x]ulNtResumeThread:%08x\n",hThread,ulNtResumeThread));

						//根据暂停次数循环恢复线程运行
						for (i=0;i<SuspendCount;i++)
						{
							status = ulNtResumeThread(hThread,0);
							if (!NT_SUCCESS(status))
							{
								if (DebugOn) 
									KdPrint(("Resume Thread failed:%d\n",RtlNtStatusToDosError(status)));
							}
						}
						ZwClose(hThread);
					}
				}
			}
		}
		break;
	}
	return status;
}
//暂停线程运行
NTSTATUS SuspendThread(PETHREAD Thread)
{
	NTSTATUS status=STATUS_UNSUCCESSFUL;
	KPROCESSOR_MODE PreviousMode;
	PSSUSPENDTHREAD PsSuspendThread = 0;

	//如果线程不可用
	if (!MmIsAddressValidEx(Thread)){
		return status;
	}
	//如果线程结束了
	if (PsIsThreadTerminating(Thread)){
		return status;
	}
	PsSuspendThread = (PSSUSPENDTHREAD)GetPsXXXXXThread("NtSuspendThread");
	if (PsSuspendThread){
		KPROCESSOR_MODE PreviousMode = ExGetPreviousMode();
		status = PsSuspendThread(Thread,&PreviousMode);
	}
	return status;
}
VOID IniOffsetObject()
{
	WIN_VER_DETAIL WinVer;

	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		ThreadProc = 0x22C;
		ThreadListHead = 0x190; 
		ulCrossThreadFlagsOffset = 0x248;
		ulHideDebug = 0x248;
		ulThreadID = 0x1f0;
		ulTEB = 0x20;
		ulThreadStart = 0x224;
		ulWin32ThreadStart = 0x228;
		ulStatus = 0x02d;
		ulPriority = 0x033;
		ulContextSwitches = 0x04c;
		ulSuspendCount = 0x1b9;
		ulActiveExWorker = 0x24c;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ThreadProc = 0x224;
		ThreadListHead = 0x180;
		ulCrossThreadFlagsOffset = 0x240;
		ulHideDebug = 0x240;
		ulThreadID = 0x1e8;
		ulTEB = 0x74;
		ulThreadStart = 0x21c;
		ulWin32ThreadStart = 0x220;
		ulStatus = 0x04c;
		ulPriority = 0x05b;
		ulContextSwitches = 0x048;
		ulSuspendCount = 0x150;
		ulActiveExWorker = 0x244;
		break;
	case WINDOWS_VERSION_7_7000:
		ThreadProc = 0x268;
		ThreadListHead = 0x180;    //<------------
		ulCrossThreadFlagsOffset = 0x280;
 		ulHideDebug = 0x280;
 		ulThreadID = 0x230;
 		ulTEB = 0x088;
 		ulThreadStart = 0x218;
		ulWin32ThreadStart = 0x260;
 		ulStatus = 0x068;
		ulPriority = 0x057;
		ulContextSwitches = 0x064;
		ulSuspendCount = 0x18c;
		ulActiveExWorker = 0x284;
		break;
	case WINDOWS_VERSION_7_7600_UP:
		ThreadProc = 0x268;
		ThreadListHead = 0x188;
		ulCrossThreadFlagsOffset = 0x280;
		ulHideDebug = 0x280;
		ulThreadID = 0x230;
		ulTEB = 0x088;
		ulThreadStart = 0x218;
		ulWin32ThreadStart = 0x260;
		ulStatus = 0x068;
		ulPriority = 0x057;
		ulContextSwitches = 0x064;
		ulSuspendCount = 0x188;
		ulActiveExWorker = 0x284;
		break;
	}
	return;
}
DWORD GetThreadModuleFromPeb(ULONG Process,ULONG ulThreadAddress,char *DllModule)
{

	DWORD PebOffset=0;
	DWORD PebLdrOffset = 0;
	DWORD Peb;
	PPEB_LDR_DATA PebLdr;
	PLDR_DATA_TABLE_ENTRY LdrTableEntry;
	PLIST_ENTRY pListHead,pListNext;
	ANSI_STRING AnsiDllModule;
	WIN_VER_DETAIL WinVer;
	BOOL bRetOK = FALSE;

	if (!MmIsAddressValidEx(Process))
	{
		return bRetOK;
	}
	__try
	{
		WinVer=GetWindowsVersion();
		switch (WinVer)
		{
		case WINDOWS_VERSION_XP:     //xp
			PebOffset=0x1b0;
			PebLdrOffset = 0x0c;
			break;
		case WINDOWS_VERSION_2K3_SP1_SP2:     //2003
			PebOffset=0x1a0;
			PebLdrOffset = 0x0c;
			break;
		case WINDOWS_VERSION_7_7000:    //win7 7000
			PebOffset=0x1a0;
			PebLdrOffset = 0x0c;
			break;
		case WINDOWS_VERSION_7_7600_UP:     //win7 7600
			PebOffset=0x1a8;
			PebLdrOffset = 0x0c;
			break;
		}
		if (PebOffset==0 ||
			PebLdrOffset==0)
		{
			bRetOK = FALSE;
			__leave;
		}
		Peb = *(DWORD*)((DWORD)Process+PebOffset);

		ProbeForRead((PVOID)Peb,4,1);//PEB是用户空间的，可能会不能访问

		if (Peb == 0 ||
			!MmIsAddressValidEx(Peb))
		{
			if (DebugOn)
				DbgPrint("Peb is null\n");
			bRetOK = FALSE;
			__leave;
		}

		PebLdr=(PPEB_LDR_DATA)*(DWORD*)(Peb+PebLdrOffset);

		ProbeForRead((PVOID)PebLdr,4,1);

		if (!MmIsAddressValidEx(PebLdr))
		{
			if (DebugOn)
				DbgPrint("PebLdr offset is null\n");
			bRetOK = FALSE;
			__leave;
		}
		pListHead=&PebLdr->InLoadOrderModuleList;
		pListNext=pListHead->Flink;
		while (pListHead!=pListNext)
		{
			LdrTableEntry=(PLDR_DATA_TABLE_ENTRY)pListNext;
			if (!MmIsAddressValidEx(LdrTableEntry))
			{
				break;
			}
			if (ValidateUnicodeString(&LdrTableEntry->FullDllName) &&
				LdrTableEntry->FullDllName.Buffer != NULL &&
				LdrTableEntry->FullDllName.Length > 0 )
			{
				if (ulThreadAddress > LdrTableEntry->DllBase &&
					ulThreadAddress < (ULONG)LdrTableEntry->DllBase + LdrTableEntry->SizeOfImage)
				{
					//如果最后的参数是true，要自己free
					RtlUnicodeStringToAnsiString(&AnsiDllModule,&LdrTableEntry->FullDllName,TRUE);

					SafeCopyMemory(
						AnsiDllModule.Buffer,
						DllModule,
						AnsiDllModule.Length
						);

					RtlFreeAnsiString(&AnsiDllModule);

					break;
				}
			}
			pListNext=pListNext->Flink;

		}
		bRetOK = TRUE;
	}
	__except(EXCEPTION_EXECUTE_HANDLER)
	{
		if (DebugOn)
			DbgPrint("GetDllNameFromPeb Error \r\n");
	}
	return bRetOK;
}
VOID QuerySystemThread(PSYSTEM_THREAD_INFO SystemThread,PEPROCESS EProcess)
{
	PETHREAD          Thread;
	PEPROCESS         Process;
	int i=0;
	ULONG ulSysModuleBase,ulSysModuleSize;

	if (!MmIsAddressValidEx(EProcess) ||
		!IsExitProcess(EProcess))
	{
		if (DebugOn)
			KdPrint(("Eprocess failed"));
		return;
	}
	IniOffsetObject();

	Process = EProcess;

	__try
	{
		for (Thread = GetNextProcessThread(Process, NULL,TRUE);
			Thread != NULL;
			Thread = GetNextProcessThread(Process, Thread,TRUE))
		{
			if (MmIsAddressValidEx(Thread))
			{
				if (MmIsAddressValidEx(((ULONG)Thread + ulStatus)))
					SystemThread->SystemThreadInfo[i].ulStatus = *(UCHAR *)((ULONG)Thread + ulStatus);

				if (MmIsAddressValidEx(((ULONG)Thread + ulHideDebug)))
					SystemThread->SystemThreadInfo[i].HideDebug = *(ULONG *)((ULONG)Thread + ulHideDebug);

				SystemThread->SystemThreadInfo[i].KernelThread = (ULONG)Thread;

				if (MmIsAddressValidEx(((ULONG)Thread + ulTEB)))
					SystemThread->SystemThreadInfo[i].TEB = *(ULONG *)((ULONG)Thread + ulTEB);

				if (MmIsAddressValidEx(((ULONG)Thread + ulThreadID)))
					SystemThread->SystemThreadInfo[i].ThreadID = *(ULONG *)((ULONG)Thread + ulThreadID);

				if (EProcess == SystemEProcess)
				{
					if (MmIsAddressValidEx(((ULONG)Thread + ulThreadStart)))
						SystemThread->SystemThreadInfo[i].ThreadStart = *(ULONG *)((ULONG)Thread + ulThreadStart);
				}else
				{
					if (MmIsAddressValidEx(((ULONG)Thread + ulWin32ThreadStart)))
						SystemThread->SystemThreadInfo[i].ThreadStart = *(ULONG *)((ULONG)Thread + ulWin32ThreadStart);
				}

				if (MmIsAddressValidEx(((ULONG)Thread + ulPriority)))
					SystemThread->SystemThreadInfo[i].ulPriority = *(ULONG *)((ULONG)Thread + ulPriority);

				if (MmIsAddressValidEx(((ULONG)Thread + ulContextSwitches)))
					SystemThread->SystemThreadInfo[i].ulContextSwitches = *(ULONG *)((ULONG)Thread + ulContextSwitches);

				memset(SystemThread->SystemThreadInfo[i].lpszThreadModule,0,sizeof(SystemThread->SystemThreadInfo[i].lpszThreadModule));

				if (EProcess == SystemEProcess)
				{
					if (!IsAddressInSystem(
						SystemThread->SystemThreadInfo[i].ThreadStart,
						&ulSysModuleBase,
						&ulSysModuleSize,
						SystemThread->SystemThreadInfo[i].lpszThreadModule))
					{
						strcat(SystemThread->SystemThreadInfo[i].lpszThreadModule,"Unknown3(隐藏线程，危险！)");
						SystemThread->SystemThreadInfo[i].ulHideType = 1;   //隐藏线程
					}
				}else{

					if (!RKeAttachProcess ||
						!RKeDetachProcess){
							ReLoadNtosCALL(&RKeAttachProcess,L"KeAttachProcess",SystemKernelModuleBase,ImageModuleBase);
							ReLoadNtosCALL(&RKeDetachProcess,L"KeDetachProcess",SystemKernelModuleBase,ImageModuleBase);
					}
					if (RKeAttachProcess &&
						RKeDetachProcess)
					{
						//挂载进程，从peb里获取线程所在的dll

						RKeAttachProcess(Process);

						GetThreadModuleFromPeb(Process,
							SystemThread->SystemThreadInfo[i].ThreadStart,
							SystemThread->SystemThreadInfo[i].lpszThreadModule
							);

						RKeDetachProcess();
					}
				}
				
				if (DebugOn)
					KdPrint(("Thread:%08x-%08x-%s\r\n",Thread,SystemThread->SystemThreadInfo[i].ThreadStart,SystemThread->SystemThreadInfo[i].lpszThreadModule));

				i++;
				SystemThread->ulCount = i;
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){
		KdPrint(("QuerySystemThread:%08x",GetExceptionCode()));
	}
}
VOID KernelTerminateThreadRoutine( 
	IN PKAPC Apc, 
	IN OUT PKNORMAL_ROUTINE *NormalRoutine, 
	IN OUT PVOID *NormalContext, 
	IN OUT PVOID *SystemArgument1, 
	IN OUT PVOID *SystemArgument2 
	) 
{ 
	PULONG ThreadFlags;
	BOOL bInit = FALSE;

	if (RExFreePool)
		RExFreePool(Apc);
	else
		ExFreePool(Apc);

	if (ulCrossThreadFlagsOffset) 
	{ 
		ThreadFlags=(ULONG *)((ULONG)(PsGetCurrentThread())+ulCrossThreadFlagsOffset); 
		*ThreadFlags=(*ThreadFlags)|PS_CROSS_THREAD_FLAGS_SYSTEM; 

		*(PULONG)((ULONG)PsGetCurrentThread()+ulActiveExWorker) = FALSE; //要结束工作队列的线程，必须设置标志
		/*
		   wrk -> PspExitThread

		   if (Thread->ActiveExWorker) {
               KeBugCheckEx (ACTIVE_EX_WORKER_THREAD_TERMINATION,
                      (ULONG_PTR)Thread,
                      0,
                      0,
                      0);
           }
		*/

		if (RPsTerminateSystemThread)
			RPsTerminateSystemThread(STATUS_SUCCESS);
		else
			PsTerminateSystemThread(STATUS_SUCCESS); //o(∩_∩)o 
	} 
	return; //never be here 
}
//
BOOL KillThread(PETHREAD Thread)
{
	PKAPC Apc=NULL; 
	BOOL blnSucceed=FALSE; 
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeInitializeApc,L"KeInitializeApc",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeInsertQueueApc,L"KeInsertQueueApc",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RPsTerminateSystemThread,L"PsTerminateSystemThread",SystemKernelModuleBase,ImageModuleBase);
	if (RExAllocatePool &&
		RExFreePool &&
		RKeInitializeApc &&
		RKeInsertQueueApc)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return blnSucceed;

	if (!MmIsAddressValidEx(Thread)) {
		return FALSE; //error. 
	}

	Apc = RExAllocatePool(NonPagedPool,sizeof(KAPC)); 
	if (!Apc){
		return blnSucceed; 
	}
	IniOffsetObject();

	RKeInitializeApc(Apc, 
		Thread, 
		0, 
		KernelTerminateThreadRoutine, 
		NULL, 
		NULL, 
		KernelMode, 
		NULL); //special apc - whether alertable or not makes no difference.. 

	blnSucceed = RKeInsertQueueApc(Apc, 
		NULL, 
		NULL, 
		0); 
	//add some code works like KeForceResumeThread here. 
	return blnSucceed; 
}