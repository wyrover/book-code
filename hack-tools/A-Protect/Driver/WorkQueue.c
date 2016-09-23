#include "WorkQueue.h"

VOID QueryWorkQueue(PWORKQUEUE WorkQueueThread)
{
	ULONG ulBasePriority;
	ULONG ulKernelStack;

	ULONG ulSizeOfSystemWorkThread;
	ULONG ulSizeOfOtherWorkThread;

	ULONG ulSizeOfSystemWorkSuspendThread;
	ULONG ulSizeOfOtherWorkSuspendThread;

	ULONG ulSuspendCount;  //暂停的情况下kernelstack又不一样的
	ULONG SuspendCount;

	ULONG i=0,x=0;
	PEPROCESS Process;
	PETHREAD  Thread;
	BOOL bIsWorkTrue = FALSE;

	PUCHAR KernelStack = NULL;

	ULONG ulModuleBase;
	ULONG ulModuleSize;
	ULONG ulText;
	WIN_VER_DETAIL WinVer;
	PUCHAR BasePriority = 0;
	BYTE WorkThreadCode[1] = {0x6a};
	BYTE WorkThreadCode1[1] = {0x8b};
	int Index=0;

	//ThreadProc ThreadListHead
	IniOffsetObject();

	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_XP:
		ulBasePriority = 0x06c;
		ulKernelStack = 0x028;
		ulSuspendCount = 0x1b9;

		ulSizeOfSystemWorkThread = 0x08c;
		ulSizeOfOtherWorkThread = 0x0a8;

		ulSizeOfSystemWorkSuspendThread = 0x12c;
	    ulSizeOfOtherWorkSuspendThread = 0x148;
		break;
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ulBasePriority = 0x121;
		ulKernelStack = 0x20;
		ulSuspendCount = 0x150;

		ulSizeOfSystemWorkThread = 0x0a8;
		ulSizeOfOtherWorkThread = 0x0c0;

		ulSizeOfSystemWorkSuspendThread = 0x170;
	    ulSizeOfOtherWorkSuspendThread = 0x188;
		break;
	case WINDOWS_VERSION_7_7000:
		ulBasePriority = 0x139;
		ulKernelStack = 0x30;
		ulSuspendCount = 0x18c;

		ulSizeOfSystemWorkThread = 0x182;
		ulSizeOfOtherWorkThread = 0x184;

		ulSizeOfSystemWorkSuspendThread = 0x364;
	    ulSizeOfOtherWorkSuspendThread = 0x364;
		break;
	case WINDOWS_VERSION_7_7600_UP:
		ulBasePriority = 0x135;
		ulKernelStack = 0x30;
		ulSuspendCount = 0x188;

		ulSizeOfSystemWorkThread = 0x128;
		ulSizeOfOtherWorkThread = 0x130;

		ulSizeOfSystemWorkSuspendThread = 0x270;
	    ulSizeOfOtherWorkSuspendThread = 0x278;
		break;
	}
	Process = SystemEProcess;

	i=0;

	for (Thread = GetNextProcessThread(Process, NULL,TRUE);
		Thread != NULL;
		Thread = GetNextProcessThread(Process, Thread,TRUE))
	{
		BasePriority = (ULONG)Thread+ulBasePriority;

		if (!PsIsThreadTerminating(Thread) &&
			MmIsAddressValidEx(BasePriority) &&
			MmIsAddressValidEx(((ULONG)Thread+ulSuspendCount)))
		{
			bIsWorkTrue = FALSE;

			if (*BasePriority == 12 ||
				*BasePriority == 13 ||
				*BasePriority == 15)
			{
				bIsWorkTrue = TRUE;
			}
			if (!bIsWorkTrue){
				continue;
			}
			//获取线程暂停状态 0 为正常 其他的为暂停次数
			SuspendCount = *(PULONG)((ULONG)Thread+ulSuspendCount);

			if (DebugOn)
				KdPrint(("[%08x]SuspendCount:%d\n",Thread,SuspendCount));

			WorkQueueThread->WorkQueueInfo[i].ulBasePriority = *BasePriority;
			WorkQueueThread->WorkQueueInfo[i].ulEthread = Thread;
			WorkQueueThread->WorkQueueInfo[i].SuspendCount = SuspendCount;

			KernelStack = *(PULONG)((ULONG)Thread+ulKernelStack);

			//获取系统的工作线程
			if (SuspendCount){
				ulText = KernelStack + ulSizeOfSystemWorkSuspendThread;  //这里是硬编码，暂停线程的时候work入口地址偏移
			}else{
				ulText = KernelStack + ulSizeOfSystemWorkThread;  //这里是硬编码，线程正常的时候work入口地址偏移
			}
			//好像这里用RMmIsAddressValidEx判断不准确蓝屏，无语~~
			//（蓝屏其实就是没有判断VCS_TRANSITION类型，这个类型也是无法访问）
			if (MmIsAddressValidEx(ulText))
			{
				x = *(PULONG)(ulText);
				if (MmIsAddressValidEx(x))
				{
					//按照下面的规则，再做进一步的检查
					/*
					lkd> u 83CEB1D7
						nt!CcWorkerThread:
					83ceb1d7 6a2c            push    2Ch
					*/
					/*
					lkd> u 8AB06510
					8ab06510 8bff            mov     edi,edi
					8ab06512 55              push    ebp
					8ab06513 8bec            mov     ebp,esp
                    */
					if (memcmp(x,WorkThreadCode,1) == 0 ||
						memcmp(x,WorkThreadCode1,1) == 0)
					{
						//填充

						WorkQueueThread->WorkQueueInfo[i].ulWorkerRoutine = x;
						memset(WorkQueueThread->WorkQueueInfo[i].lpszModule,0,sizeof(WorkQueueThread->WorkQueueInfo[i].lpszModule));
						if (!IsAddressInSystem(
							x,
							&ulModuleBase,
							&ulModuleSize,
							WorkQueueThread->WorkQueueInfo[i].lpszModule))
						{
							strcat(WorkQueueThread->WorkQueueInfo[i].lpszModule,"Unknown");
						}
					}
				}
			}

			//获取其他的驱动的
		    //线程暂停，这个时候大小就变了。
			if (SuspendCount){
				ulText = KernelStack + ulSizeOfOtherWorkSuspendThread;  //这里是硬编码，暂停线程的时候work入口地址偏移
			}else
				ulText = KernelStack + ulSizeOfOtherWorkThread;  //这里是硬编码，线程正常的时候work入口地址偏移

			//好像这里用RMmIsAddressValidEx判断不准确蓝屏，无语~~
			//（蓝屏其实就是没有判断VCS_TRANSITION类型，这个类型也是无法访问）
			if (MmIsAddressValidEx(ulText))
			{
				x = *(PULONG)(ulText);
				if (MmIsAddressValidEx(x))
				{
					//按照下面的规则，再做进一步的检查
					/*
					lkd> u 83CEB1D7
						nt!CcWorkerThread:
					83ceb1d7 6a2c            push    2Ch
					*/
					/*
					lkd> u 8AB06510
					8ab06510 8bff            mov     edi,edi
					8ab06512 55              push    ebp
					8ab06513 8bec            mov     ebp,esp
                    */
					if (memcmp(x,WorkThreadCode,1) == 0 ||
						memcmp(x,WorkThreadCode1,1) == 0)
					{
							WorkQueueThread->WorkQueueInfo[i].ulWorkerRoutine = x;
							memset(WorkQueueThread->WorkQueueInfo[i].lpszModule,0,sizeof(WorkQueueThread->WorkQueueInfo[i].lpszModule));
							if (!IsAddressInSystem(
								x,
								&ulModuleBase,
								&ulModuleSize,
								WorkQueueThread->WorkQueueInfo[i].lpszModule))
							{
								strcat(WorkQueueThread->WorkQueueInfo[i].lpszModule,"Unknown");
							}
					}
				}
			}
			i++;
			WorkQueueThread->ulCount = i;
		}
	}
	if (DebugOn){
		for (i=0;i<WorkQueueThread->ulCount;i++)
		{
			KdPrint(("[%d]工作队列线程\r\n"
				"EHTREAD：%08X\r\n"
				"类型：%d\r\n"
				"函数入口：%08X\r\n"
				"函数入口所在模块：%s\r\n\r\n",
				i,
				WorkQueueThread->WorkQueueInfo[i].ulEthread,
				WorkQueueThread->WorkQueueInfo[i].ulBasePriority,
				WorkQueueThread->WorkQueueInfo[i].ulWorkerRoutine,
				WorkQueueThread->WorkQueueInfo[i].lpszModule));
		}
	}
	return;
}