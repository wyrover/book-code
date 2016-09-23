#include "ProcessDriver.h"

// 通过搜索PsLookupProcessByProcessId函数，获取PspCidTable这个全局变量的地址，
// 再返回PspCidTable这个全局变量的值，即_HANDLE_TABLE的地址
ULONG GetTableAddress()
{
	UNICODE_STRING functionName;  // 函数名称	
	PUCHAR pfunction;  // 指向函数起始处的指针
	ULONG tableAddress;  // _HANDLE_TABLE地址
	PUCHAR p;  // 地址遍历指针

	RtlInitUnicodeString(&functionName, L"PsLookupProcessByProcessId");	
	pfunction = (PUCHAR)MmGetSystemRoutineAddress(&functionName);  // 通过函数名获得指向函数起始地址的指针
	for (p = pfunction; p < pfunction + PAGE_SIZE; p++)  // PAGE_SIZE为4KB
	{
		if((*(PUSHORT)p == 0x35ff) && (*(p + 6) == 0xe8))  // 特征串
		{
			tableAddress = *(PULONG)(*(PULONG)(p + 2));  // 全局变量PspCidTable的值，即_HANDLE_TABLE的地址
			return tableAddress;
		}
	}

	return 0;
}


// 通过当前进程获取进程对象的类型
ULONG GetProcessType()
{
	ULONG objectHeaderAddress;  // 对象头地址
	ULONG type;  // 对象类型

	objectHeaderAddress = (ULONG)PsGetCurrentProcess();  // 返回指向当前进程的EPROCESS指针，再转为地址(对象体)
	objectHeaderAddress = (ULONG)OBJECT_TO_OBJECT_HEADER(objectHeaderAddress);  // 从对象体地址转到对象头地址
	type = *(PULONG)(objectHeaderAddress + TYPE);  // 类型

	return type;
}


// 使用单向链表记录进程对象体地址
VOID RecordProcessAddress(ULONG objectAddress)
{
	PProcessInfo pProcessInfo = NULL;  // 进程信息结构指针变量
	if(head == NULL)
	{
		head = (PProcessInfo)ExAllocatePool(NonPagedPool, sizeof(ProcessInfo));  // 在堆中申请指定大小非分页内存
		if(head == NULL)
		{
			return;
		}

		head->address = objectAddress;
		head->next = NULL;
		pCurrent = head;
	}
	else
	{
		pProcessInfo = (PProcessInfo)ExAllocatePool(NonPagedPool, sizeof(ProcessInfo));  // 在堆中申请指定大小非分页内存
		if(pProcessInfo == NULL)
		{
			return;
		}
		
		pCurrent->next = pProcessInfo;
		pProcessInfo->address = objectAddress;
		pProcessInfo->next = NULL;
		pCurrent = pProcessInfo;
	}
}


// 从1级表开始遍历
ULONG BrowseTableL1(ULONG tableAddr)
{
	ULONG objBodyAddr = 0;  // 对象体地址变量
	ULONG ItemCount = 512;  // 一个基本表包含的项数
	ULONG objHeaderAddr = 0;  // 对象头地址
	ULONG flags;

	do 
	{
		objBodyAddr = *(PULONG)tableAddr;  // 得到对象体地址
		objBodyAddr &= 0xFFFFFFF8;  // 对象体地址最后三位置0
        tableAddr += 8;  // 移到下一表项，每个表项8字节 

		if (objBodyAddr == 0)
		{
			continue;
		}

		objHeaderAddr = (ULONG)OBJECT_TO_OBJECT_HEADER(objBodyAddr);  // 对象体地址转对象头地址
		if ( GetProcessType() == *(PULONG)(objHeaderAddr + TYPE) )  // 判断对象类型是否为进程
		{
			flags = *(PULONG)(objBodyAddr + FLAGS);  // 进程对象的对象体就是EPROCESS
			if( (flags & 0x0000000C) != 0x0000000C)  // 判断进程是否为“过期”的
			{
				RecordProcessAddress(objBodyAddr);  // 使用单向链表记录进程对象体地址	
			}
		}    
	} while (--ItemCount > 0);

	return 0;
}


// 从2级表开始遍历
ULONG BrowseTableL2(ULONG tableAddr)
{
	do 
	{
		BrowseTableL1(*(PULONG)tableAddr);  // 转到基本表
		tableAddr += 4;  // 移至2级表的下一个表项，每项4个字节
	} while ((*(PULONG)tableAddr) != 0);

	return 0;
}


// 从3级表开始遍历
ULONG BrowseTableL3(ULONG tableAddr)
{
	do 
	{
		BrowseTableL2(*(PULONG)tableAddr);  // 转到2级表
		tableAddr += 4;  // 移至3级表的下一个表项，每项4个字节
	} while ((*(PULONG)tableAddr) != 0);

	return 0;
}


// 遍历PspCidTable引出的句柄表
VOID EnumPspCidTable()
{
	ULONG tableAddress = GetTableAddress();  // 得到句柄表地址
	ULONG tableCode = *(PULONG)tableAddress;  // 得到tableCode 
	ULONG flag = tableCode & 3;  // 得到标志，用来判断几层表
	tableCode &= 0xFFFFFFFC;  // tableCode后两位置0  

	switch (flag)
	{
	case 0:  // 一层表
		BrowseTableL1(tableCode);
		break;
	case 1:  // 两层表
		BrowseTableL2(tableCode);
		break;
	case 2:  // 三层表
		BrowseTableL3(tableCode);
		break;    
	}
}


// 得到当前活动进程的数量
int GetProcessNumber()
{
	int count = 0;  // 计数变量
	// 遍历进程单链表
	for(pCurrent = head; pCurrent != NULL; pCurrent = pCurrent->next)
	{
		count++;
	}

	return count;
}


// 释放进程链表
VOID FreeProcessList()
{
	PProcessInfo  p = NULL;
	pCurrent = head;
	p = pCurrent->next;
	while(p != NULL)
	{
		ExFreePool(pCurrent);  // 回收堆内存
		pCurrent = p;
		p = pCurrent->next;
	}
	ExFreePool(pCurrent);  // 回收堆内存
	head = NULL;
	pCurrent = NULL;
}


// 通过进程对象的地址得到进程对象的ID和名字
VOID GetProcessInfoByAddr(PProcessInfo buffer)
{
	int i = 0;

	for(pCurrent = head; pCurrent != NULL; pCurrent = pCurrent->next)
	{
		// PID
		if (pCurrent->address == 0)
		{
			pCurrent->pid = 0;
		}
		else if (pCurrent->address == 4)
		{
			pCurrent->pid = 4;
		}
		else
		{
			pCurrent->pid = *(int *)(pCurrent->address + UNIQUEPROCESSID);  
			if (pCurrent->pid < 0)
			{
				pCurrent->pid = 0;
			}
		}
    	
		// ImageFileName
		if (pCurrent->pid == 0)
		{
			PCHAR pStr = "System Idle";
			memcpy(pCurrent->name, pStr, sizeof(pCurrent->name));
		}
		else if (pCurrent->pid == 4)
		{
			PCHAR pStr = "System";
			memcpy(pCurrent->name, pStr, sizeof(pCurrent->name));
		}
		else
		{
			strcpy((PCHAR)pCurrent->name, (PCHAR)(pCurrent->address + IMAGEFILENAME));  
		}

		memcpy(buffer+i, pCurrent, sizeof(ProcessInfo));
		i++;
	}

	FreeProcessList();  // 释放进程链表
}


// 利用ZwQuerySystemInformation依据SystemModuleInformation获取ntoskrnl.exe基址和镜像大小，然后特征码搜索定位
ULONG GetPspTerminateThreadByPointer()
{
	NTSTATUS status;
	PVOID buffer;
	ULONG code1 = 0x8B55FF8B, code2 = 0x0CEC83EC, code3 = 0xFFF84D83, code4 = 0x7D8B5756;  // PspTerminateThreadByPointer特征码SP3
	ULONG address;  // PspTerminateThreadByPointer地址
	ULONG ntoskrnlBase;  // ntoskrnl.exe加载基址
	ULONG ntoskrnlEndAddress;  // ntoskrnl.exe结束地址
	ULONG i;  // 遍历
	PSYSTEM_MODULE_INFORMATION_ENTRY pModule;
	ULONG size;  // 动态分配大小

	// 试探一下所需内存大小，返回需要size大小
	ZwQuerySystemInformation(SystemModuleInformation, &size, 0, &size);  
	KdPrint(("[PspTerminateThreadByPointer] size:0x%x\n", size));

	// 分配堆内存
	buffer = ExAllocatePool(NonPagedPool, size);  
	if(buffer == NULL)
	{
		KdPrint(("[PspTerminateThreadByPointer] malloc memory failed\n"));
		return 0;
	}

	// 这次真的去取信息
	status = ZwQuerySystemInformation(SystemModuleInformation, buffer, size, 0);
	if(!NT_SUCCESS(status))
	{
		KdPrint(("[PspTerminateThreadByPointer] query failed\n"));
		return status;
	}

	pModule = (PSYSTEM_MODULE_INFORMATION_ENTRY)((PULONG)buffer + 1);
	ntoskrnlBase = (ULONG)pModule->Base;  // 得到ntoskrnl.exe加载基址
	ntoskrnlEndAddress = (ULONG)pModule->Base + (ULONG)pModule->Size;  // 得到ntoskrnl.exe结束地址
	KdPrint(("[PspTerminateThreadByPointer] Size :0x%x\n", (ULONG)pModule->Size));  
	KdPrint(("[PspTerminateThreadByPointer] NtoskrnlBase :0x%x\n", ntoskrnlBase));  
	KdPrint(("[PspTerminateThreadByPointer] NtoskrnlEndAddress :0x%x\n", ntoskrnlEndAddress));

	ExFreePool(buffer);  // 释放堆内存

	// 搜索PspTerminateThreadByPointer特征码
	for(i = ntoskrnlBase; i <= ntoskrnlEndAddress; i++)
	{
		if(MmIsAddressValid((PULONG)i) && MmIsAddressValid((PULONG)(i+4)) 
			&& MmIsAddressValid((PULONG)(i+8)) && MmIsAddressValid((PULONG)(i+12)))  // 判断是否产生缺页中断
		{
			if((*(PULONG)i == code1) && (*(PULONG)(i+4) == code2) && (*(PULONG)(i+8) == code3) && (*(PULONG)(i+12) == code4))
			{
				address = (ULONG)i;
				KdPrint(("[PspTerminateThreadByPointer] address :0x%x\n", address));
				return address;
			}
		}
	}

	return 0;
}


// 获得指定进程里的下一个线程
PETHREAD GetNextProcessThread(IN PEPROCESS Process, IN PETHREAD Thread OPTIONAL)  
{  
	PETHREAD FoundThread = NULL;  
	PLIST_ENTRY ListHead, Entry;  
	PAGED_CODE();  

	if (Thread)  
	{  
		  //Entry = Thread->ThreadListEntry.Flink;//   +0x22c ThreadListEntry  : _LIST_ENTRY  
		Entry = (PLIST_ENTRY)((ULONG)(Thread) + THREADLISTENTRY);  
		Entry = Entry->Flink;  
	}  
	else 
	{  
		Entry = (PLIST_ENTRY)((ULONG)(Process) + THREADLISTHEAD);//+0x190 ThreadListHead   : _LIST_ENTRY  
		Entry = Entry->Flink;  
	}  
	// ListHead = &rocess->ThreadListHead;  
	ListHead = (PLIST_ENTRY)((ULONG)Process + THREADLISTHEAD);  
	while (ListHead != Entry)  
	{  
		//   FoundThread = CONTAINING_RECORD(Entry, ETHREAD, ThreadListEntry);  
		FoundThread = (PETHREAD)((ULONG)Entry - THREADLISTENTRY);  
		if ( ObReferenceObject(FoundThread) ) 
		{
			break;
		}
		FoundThread = NULL;  
		Entry = Entry->Flink;  
	}  
	if (Thread) 
	{
		ObDereferenceObject(Thread);  
	}

	return FoundThread;  
}  


// 调用PspTerminateThreadByPointer结束进程
NTSTATUS KillProcessWithPsp(ULONG pid)  
{
	PEPROCESS pEProcess;
	PsLookupProcessByProcessId(pid, &pEProcess);  // 根据进程PID得到PEPROCESS 

	typedef NTSTATUS (*PSPTERMINATETHREADBYPOINTER) (PETHREAD, NTSTATUS);  // 函数指针类型定义  
	PSPTERMINATETHREADBYPOINTER myPspTerminateThreadByPointer;  // 函数指针 

	ULONG address = GetPspTerminateThreadByPointer();  // PspTerminateThreadByPointer的地址
	myPspTerminateThreadByPointer = (PSPTERMINATETHREADBYPOINTER)address;  // 定位PspTerminateThreadByPointer

	NTSTATUS status = STATUS_SUCCESS;  
	PETHREAD pEThread;  

	_try
	{
		// 遍历指定进程里的所有线程
		for ( pEThread = GetNextProcessThread(pEProcess, NULL); pEThread != NULL; pEThread = GetNextProcessThread(pEProcess, pEThread) )  
		{  
			status = (*myPspTerminateThreadByPointer) (pEThread, 0);  // 结束指定进程里的线程  
		}
	}
	_except(EXCEPTION_EXECUTE_HANDLER)
	{
		status = GetExceptionCode();
	}

	return status;  
}  


// 通过EPROCESS中的ActiveProcessLinks遍历，返回进程数
//
ULONG EnumActiveProcessLinks(PProcessInfo buffer)
{
	KdPrint(("EnumActiveProcessLinks:"));

	ULONG eproc = (ULONG)PsGetCurrentProcess();  // 进程对象体地址
	LONG startPID = *( PLONG(eproc + UNIQUEPROCESSID) );  // 起始PID
	LONG currentPID = startPID;  // 当前PID
	ULONG count = 0;  // 进程数
	while(1)
	{
		if((count >= 1)&&(startPID == currentPID))
		{
			break;
		}
		else
		{
			PLIST_ENTRY pListActiveProcs = (PLIST_ENTRY)(eproc + ACTIVEPROCESSLINKS);
			eproc = (ULONG)pListActiveProcs->Flink;
			eproc -= ACTIVEPROCESSLINKS;  // 下一个进程对象体地址
			RecordProcessAddress(eproc);  // 使用单向链表记录进程对象体地址

			currentPID = *( PLONG(eproc + UNIQUEPROCESSID) );
			if (currentPID < 0)
			{
				currentPID = 0;
			}
			KdPrint(("process id %4d,address %8x,image %s", currentPID, eproc, PCHAR(eproc+IMAGEFILENAME) ));

			count++;
		}
	}
	KdPrint(("Total number is %d", count));

	if (buffer != NULL)  // 非试探
	{
		GetProcessInfoByAddr(buffer);  // 得到进程对象信息，放入系统提供的缓冲区
	}
	else  // 试探
	{
		FreeProcessList();  // 释放进程链表
	}

	return count;
}


// 通过EPROCESS中的ObjectTable遍历，返回进程数
//
ULONG EnumObjectTable(PProcessInfo buffer)
{
	KdPrint(("EnumObjectTable:"));
	ULONG eproc = (ULONG)PsGetCurrentProcess();
	PLIST_ENTRY pHandleTableList = (PLIST_ENTRY)(*(PULONG)( (ULONG)eproc + OBJECTTABLE ) + HANDLETABLELIST);
	PLIST_ENTRY pStartList = pHandleTableList;
	ULONG addr;
	int count = 0;
	do 
	{
		int pid = *(PULONG)( (ULONG)pHandleTableList + UNIQUE_PROCESSID - HANDLETABLELIST );  // 得到PID
		addr = *(PULONG)( (ULONG)pHandleTableList + QUOTAPROCESS - HANDLETABLELIST );  // 得到进程对象体地址
		KdPrint(("process id %4d,address %8x", pid, addr));

		// PID为0和4的系统进程无法由此种方法得到对象体地址
		if (pid == 0)  
		{
			RecordProcessAddress(0);
		}
		else if (pid == 4)
		{
			RecordProcessAddress(4);
		}
		else
		{
			RecordProcessAddress(addr);
		}

		count++;
		pHandleTableList = pHandleTableList->Flink;

	} while(pStartList != pHandleTableList);

	KdPrint(("Total number is %d", count));

	if (buffer != NULL)  // 非试探
	{
		GetProcessInfoByAddr(buffer);  // 得到进程对象信息，放入系统提供的缓冲区
	}
	else  // 试探
	{
		FreeProcessList();  // 释放进程链表
	}

	return count;
}


/*----------------------------------------------------------------    驱     动  -------------------------------------------------------------------------*/

NTSTATUS ProcessDriverDispatchRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	KdPrint(("Enter ProcessDriverDispatchRoutine\n"));
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;  // bytes xfered
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	KdPrint(("Leave ProcessDriverDispatchRoutine\n"));

	return status;
}


NTSTATUS ProcessDriverDispatchDeviceControl(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp)
{
	KdPrint(("Enter ProcessDriverDeviceIOControl\n"));

	NTSTATUS status = STATUS_SUCCESS;
   
    PVOID ioBuf = pIrp->AssociatedIrp.SystemBuffer;  // 系统提供的缓冲区

	PIO_STACK_LOCATION stack = IoGetCurrentIrpStackLocation(pIrp);  // 得到当前堆栈
    ULONG inBufLength = stack->Parameters.DeviceIoControl.InputBufferLength;  // 得到输入缓冲区大小
    ULONG outBufLength = stack->Parameters.DeviceIoControl.OutputBufferLength;  // 得到输出缓冲区大小
    ULONG ioControlCode = stack->Parameters.DeviceIoControl.IoControlCode;  // 得到IOCTL码

    switch (ioControlCode)
    {
	case IOCTL_PROCESSDRIVER_PSPCIDTABLE_GETPROCESSNUMBER:
		{
			EnumPspCidTable();
			*(int *)ioBuf = GetProcessNumber();  // 得到进程数目，把值放入系统提供的缓冲区
			pIrp->IoStatus.Information = sizeof(int);
            break;
		}
	case IOCTL_PROCESSDRIVER_PSPCIDTABLE_GETPROCESSINFO:
		{
			GetProcessInfoByAddr((PProcessInfo)ioBuf);  // 得到进程对象信息，放入系统提供的缓冲区
			pIrp->IoStatus.Information = inBufLength;
            break;
		}
	case IOCTL_PROCESSDRIVER_TERMINATEPROCESS:
		{
			KillProcessWithPsp(*(PULONG)ioBuf);
			pIrp->IoStatus.Information = 0;
            break;
		}
	case IOCTL_PROCESSDRIVER_ACTIVEPROCESSLINKS:
		{	
			pIrp->IoStatus.Information = EnumActiveProcessLinks((PProcessInfo)ioBuf) * sizeof(ProcessInfo);  // 返回所需内存
			break;
		}
	case IOCTL_PROCESSDRIVER_OBJECTTABLE:
		{
			pIrp->IoStatus.Information = EnumObjectTable((PProcessInfo)ioBuf) * sizeof(ProcessInfo);
			break;
		}
    default:
        status = STATUS_INVALID_PARAMETER;
		pIrp->IoStatus.Information = 0;
        KdPrint(("[ProcessDriver] unknown IRP_MJ_DEVICE_CONTROL \n"));
	}

	// 完成IRP
    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

	KdPrint(("Leave ProcessDriverDeviceIOControl\n"));

    return status;
}


VOID ProcessDriverUnload(IN PDRIVER_OBJECT pDriverObject)
{
	KdPrint(("Enter DriverUnload\n"));

    UNICODE_STRING symLinkName;
    RtlInitUnicodeString(&symLinkName, L"\\??\\ProcessDriver");

    IoDeleteSymbolicLink(&symLinkName);  // 删除符号链接
    IoDeleteDevice(pDriverObject->DeviceObject);  // 删除设备
}


NTSTATUS CreateDevice(IN PDRIVER_OBJECT	pDriverObject) 
{
	// 创建设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName, L"\\Device\\ProcessDriver");
	
	// 创建设备
	PDEVICE_OBJECT pDevObj;
	NTSTATUS status = IoCreateDevice(pDriverObject,
		sizeof(DEVICE_EXTENSION),
		&devName,
		FILE_DEVICE_UNKNOWN,
		0, 
		TRUE,
		&pDevObj);
	if (!NT_SUCCESS(status))
	{
		return status;
	}

	PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;

	//创建符号链接
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName, L"\\??\\ProcessDriver");
	status = IoCreateSymbolicLink(&symLinkName, &devName);
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice(pDevObj);
		return status;
	}
	return STATUS_SUCCESS;
}


extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT pDriverObject, IN PUNICODE_STRING pRegistryPath)
{    
	KdPrint(("Enter DriverEntry\n"));
	
	// 注册其他驱动调用函数入口
	pDriverObject->DriverUnload = ProcessDriverUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = ProcessDriverDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = ProcessDriverDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ProcessDriverDispatchDeviceControl;

	NTSTATUS status = CreateDevice(pDriverObject);  // 创建驱动设备对象

	KdPrint(("DriverEntry end\n"));
	return status;
}
