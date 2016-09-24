#include <ntifs.h>
#include <windef.h>
#include <mountmgr.h>
#include <mountdev.h>
#include <ntddvol.h>
#include <ntstrsafe.h>
#include "DPBitmap.h"
#include "DPMain.h"

//当我们判断出那个设备是需要保护的时候，会将这个指针指向这个需要保护设备的DevExt
PDP_FILTER_DEV_EXTENSION gProtectDevExt = NULL;

VOID
DPReinitializationRoutine( 
	IN	PDRIVER_OBJECT	DriverObject, 
	IN	PVOID			Context, 
	IN	ULONG			Count 
	)
{
	//返回值
	NTSTATUS ntStatus;
	//D盘的缓冲文件名
	WCHAR				SparseFilename[] = L"\\??\\E:\\temp.dat";
	UNICODE_STRING		SparseFilenameUni;
	//建立文件时的io操作状态值
	IO_STATUS_BLOCK					ios = { 0 };
	//建立文件时的对象属性变量
	OBJECT_ATTRIBUTES				ObjAttr = { 0 };
	//设置文件大小的时候使用的文件结尾描述符
	FILE_END_OF_FILE_INFORMATION    FileEndInfo = { 0 };

	//打开我们将要用来做转储的文件
	//初始化要打开的文件名
	RtlInitUnicodeString(&SparseFilenameUni,SparseFilename);
	//初始化文件名对应的对象名，这里需要将其初始化为内核对象，并且大小写不敏感
	InitializeObjectAttributes(
		&ObjAttr, 
		&SparseFilenameUni,
		OBJ_KERNEL_HANDLE|OBJ_CASE_INSENSITIVE,
		NULL,
		NULL);
	//建立文件，这里需要注意的是，要加入FILE_NO_INTERMEDIATE_BUFFERING选项，避免文件系统再缓存这个文件
	ntStatus = ZwCreateFile(
		&gProtectDevExt->TempFile,
		GENERIC_READ | GENERIC_WRITE,
		&ObjAttr,
		&ios,
		NULL,
		FILE_ATTRIBUTE_NORMAL,
		0,
		FILE_OVERWRITE_IF,
		FILE_NON_DIRECTORY_FILE |
		FILE_RANDOM_ACCESS |
		FILE_SYNCHRONOUS_IO_NONALERT |
		FILE_NO_INTERMEDIATE_BUFFERING,
		NULL,
		0);
	if(!NT_SUCCESS(ntStatus))
	{
		goto ERROUT;
	}
	//设置这个文件为稀疏文件
	ntStatus = ZwFsControlFile(
		gProtectDevExt->TempFile,
		NULL,
		NULL,
		NULL,
		&ios,
		FSCTL_SET_SPARSE,
		NULL,
		0,
		NULL,
		0);
	if(!NT_SUCCESS(ntStatus))
	{
		goto ERROUT;
	}
	//设置这个文件的大小为"D"盘的大小并且留出10m的保护空间
	FileEndInfo.EndOfFile.QuadPart = gProtectDevExt->TotalSizeInByte.QuadPart + 10*1024*1024;
	ntStatus = ZwSetInformationFile(
		gProtectDevExt->TempFile,
		&ios,
		&FileEndInfo,
		sizeof(FILE_END_OF_FILE_INFORMATION),
		FileEndOfFileInformation
		);
	if (!NT_SUCCESS(ntStatus))
	{
		goto ERROUT;
	}
	//如果成功初始化就将这个卷的保护标志设置为在保护状态
	gProtectDevExt->Protect = TRUE;
	return;
ERROUT:
	KdPrint(("error create temp file!\n"));
	return;
}
  
NTSTATUS
DriverEntry(
    IN	PDRIVER_OBJECT	DriverObject,
    IN	PUNICODE_STRING	RegistryPath
    )
{
	int i;

	//KdBreakPoint();

	for (i = 0; i <= IRP_MJ_MAXIMUM_FUNCTION; i++)
	{
		//初始化这个驱动所有的分发函数，默认值是初始化为DPDispatchAny
		DriverObject->MajorFunction[i] = DPDispatchAny;
	}
    
	//下面将我们特殊关注的分发函数重新赋值为我们自己的处理函数
    DriverObject->MajorFunction[IRP_MJ_POWER] = DPDispatchPower;
    DriverObject->MajorFunction[IRP_MJ_PNP] = DPDispatchPnp;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = DPDispatchDeviceControl;
    DriverObject->MajorFunction[IRP_MJ_READ] = DPDispatchReadWrite;
    DriverObject->MajorFunction[IRP_MJ_WRITE] = DPDispatchReadWrite;

	//将这个驱动的AddDevice函数初始化为DpAddDevice函数
    DriverObject->DriverExtension->AddDevice = DPAddDevice;
	//将这个驱动的unload函数初始化为DpUnload函数
    DriverObject->DriverUnload = DPUnload;
    
	//注册一个boot驱动结束回调，这个回调函数会在所有的boot型驱动都运行完毕之后再去执行
	IoRegisterBootDriverReinitialization(
		DriverObject,
		DPReinitializationRoutine,
		NULL
		);

	//作为一个过滤驱动，无论如何都要返回成功
    return STATUS_SUCCESS;
}

NTSTATUS
DPCompleteRequest(
	IN	PIRP			Irp,
	IN	NTSTATUS		Status,
	IN	CCHAR			Priority
	)
{	
	//将IRP的io状态赋值为传入的参数
	Irp->IoStatus.Status = Status;
	//调用IoCompleteRequest来完成这个Irp
	IoCompleteRequest(Irp, Priority);
	return STATUS_SUCCESS;
}

NTSTATUS
DPSendToNextDriver(
	IN	PDEVICE_OBJECT	TgtDevObj,
	IN	PIRP			Irp
	)
{	
	//跳过当前的irp stack
	IoSkipCurrentIrpStackLocation(Irp);
	//调用目标设备来处理这个irp
	return IoCallDriver(TgtDevObj, Irp);
}


NTSTATUS
DPIrpCompletionRoutine(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp,
	IN	PVOID			Context
	)
{
	//将传入的上下文参数转化为一个内核事件对象
	PKEVENT Event = (PKEVENT) Context;
	//忽略掉传入的不需要的参数
	UNREFERENCED_PARAMETER(DeviceObject);
	UNREFERENCED_PARAMETER(Irp);
	//设置这个对象，唤醒等待它的进程
	KeSetEvent(Event, IO_NO_INCREMENT, FALSE);
	//返回
	return STATUS_MORE_PROCESSING_REQUIRED;
}


NTSTATUS
DPForwardIrpSync(
	IN PDEVICE_OBJECT TgtDevObj,
	IN PIRP Irp
	)
{
	//用来等待的事件
	KEVENT event;
	//返回值
	NTSTATUS status;
	//初始化等待事件
	KeInitializeEvent(&event, NotificationEvent, FALSE);
	//拷贝一份irp stack
	IoCopyCurrentIrpStackLocationToNext(Irp);
	//设置完成函数，并且将等待事件设置为上面初始化的事件，如果完成函数被调用，这个事件将会被设置，同时也由此获知这个irp处理完成了
	IoSetCompletionRoutine(
		Irp, 
		DPIrpCompletionRoutine,
		&event, 
		TRUE, 
		TRUE, 
		TRUE);
	//调用目标设备去处理这个irp
	status = IoCallDriver(TgtDevObj, Irp);
	//如果调用返回的是STATUS_PENDING，说明目标设备在处理这个irp的时候需要更多的时间，我们就开始等待，直到它处理完毕为止
	if (status == STATUS_PENDING)
	{
		KeWaitForSingleObject(
			&event, 
			Executive, 
			KernelMode, 
			FALSE, 
			NULL);
		//等到了之后赋值状态变量
		status = Irp->IoStatus.Status;
	}
	//返回状态变量
	return status;
}

VOID
DPReadWriteThread (
	IN PVOID Context
	)
{
	//NTSTATUS类型的函数返回值
	NTSTATUS					ntStatus = STATUS_SUCCESS;
	//用来指向过滤设备的设备扩展的指针
	PDP_FILTER_DEV_EXTENSION	DevExt = (PDP_FILTER_DEV_EXTENSION)Context;
	//请求队列的入口
	PLIST_ENTRY			ReqEntry = NULL;
	//irp指针
	PIRP				Irp = NULL;
	//irp stack指针
	PIO_STACK_LOCATION	Irpsp = NULL;
	//irp中包括的数据地址
	PBYTE				sysBuf = NULL;
	//irp中的数据长度
	ULONG				length = 0;
	//irp要处理的偏移量
	LARGE_INTEGER		offset = { 0 };
	//文件缓冲指针
	PBYTE				fileBuf = NULL;
	//设备缓冲指针
	PBYTE				devBuf = NULL;
	//io操作状态
	IO_STATUS_BLOCK		ios;

	//设置这个线程的优先级
	KeSetPriorityThread(KeGetCurrentThread(), LOW_REALTIME_PRIORITY);
	//下面是线程的实现部分，这个循环永不退出
	for (;;)
	{	
		//先等待请求队列同步事件，如果队列中没有irp需要处理，我们的线程就等待在这里，让出cpu时间给其它线程
		KeWaitForSingleObject(
			&DevExt->ReqEvent,
			Executive,
			KernelMode,
			FALSE,
			NULL
			);
		//如果有了线程结束标志，那么就在线程内部自己结束自己
		if (DevExt->ThreadTermFlag)
		{
			//这是线程的唯一退出地点
			PsTerminateSystemThread(STATUS_SUCCESS);
			return;
		}
		//从请求队列的首部拿出一个请求来准备处理，这里使用了自旋锁机制，所以不会有冲突
		while (ReqEntry = ExInterlockedRemoveHeadList(
			&DevExt->ReqList,
			&DevExt->ReqLock
			))
		{
			//从队列的入口里找到实际的irp的地址
			Irp = CONTAINING_RECORD(ReqEntry, IRP, Tail.Overlay.ListEntry);
			//取得irp stack
			Irpsp = IoGetCurrentIrpStackLocation(Irp);
			//获取这个irp其中包含的缓存地址，这个地址可能来自mdl，也可能就是直接的缓冲，这取决于我们当前设备的io方式是buffer还是direct方式
			if (NULL == Irp->MdlAddress)
				sysBuf = (PBYTE)Irp->UserBuffer;
			else
				sysBuf = (PBYTE)MmGetSystemAddressForMdlSafe(Irp->MdlAddress, NormalPagePriority);

			if (IRP_MJ_READ == Irpsp->MajorFunction)
			{
				//如果是读的irp请求，我们在irp stack中取得相应的参数作为offset和length
				offset = Irpsp->Parameters.Read.ByteOffset;
				length = Irpsp->Parameters.Read.Length;
			}
			else if (IRP_MJ_WRITE == Irpsp->MajorFunction)
			{
				//如果是写的irp请求，我们在irp stack中取得相应的参数作为offset和length
				offset = Irpsp->Parameters.Write.ByteOffset;
				length = Irpsp->Parameters.Write.Length;
			}
			else
			{
				//除此之外，offset和length都是0
				offset.QuadPart = 0;
				length = 0;
			}
			if (NULL == sysBuf || 0 == length)
			{
				//如果传下来的irp没有系统缓冲或者缓冲的长度是0，那么我们就没有必要处理这个irp，直接下发给下层设备就行了
				goto ERRNEXT;
			}
			//下面是转储的过程了
			if (IRP_MJ_READ == Irpsp->MajorFunction)
			{
				//这里是读的处理
				//首先根据bitmap来判断这次读操作读取的范围是全部为转储空间，还是全部为未转储空间，或者兼而有之
				long tstResult = DPBitmapTest(DevExt->Bitmap, offset, length);
				switch (tstResult)
				{
				case BITMAP_RANGE_CLEAR: 
					//这说明这次读取的操作全部是读取未转储的空间，也就是真正的磁盘上的内容，我们直接发给下层设备去处理
					goto ERRNEXT;
				case BITMAP_RANGE_SET: 
					//这说明这次读取的操作全部是读取已经转储的空间，也就是缓冲文件上的内容，我们从文件中读取出来，然后直接完成这个irp
					//分配一个缓冲区用来从缓冲文件中读取
					if (NULL == (fileBuf = (PBYTE)ExAllocatePoolWithTag(NonPagedPool, length, 'xypD')))
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						Irp->IoStatus.Information = 0;
						goto ERRERR;
					}
					RtlZeroMemory(fileBuf,length);
					ntStatus = ZwReadFile(
						DevExt->TempFile,
						NULL,
						NULL,
						NULL,
						&ios,
						fileBuf,
						length,
						&offset,
						NULL);
					if (NT_SUCCESS(ntStatus))
					{
						Irp->IoStatus.Information = length;
						RtlCopyMemory(sysBuf,fileBuf,Irp->IoStatus.Information);
						goto ERRCMPLT;
					}
					else
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						Irp->IoStatus.Information = 0;
						goto ERRERR;
					}
					break;

				case BITMAP_RANGE_BLEND:
					//这说明这次读取的操作是混合的，我们也需要从下层设备中读出，同时从文件中读出，然后混合并返回
					//分配一个缓冲区用来从缓冲文件中读取
					if (NULL == (fileBuf = (PBYTE)ExAllocatePoolWithTag(NonPagedPool, length, 'xypD')))
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						Irp->IoStatus.Information = 0;
						goto ERRERR;
					}
					RtlZeroMemory(fileBuf,length);
					//分配一个缓冲区用来从下层设备中读取
					if (NULL == (devBuf = (PBYTE)ExAllocatePoolWithTag(NonPagedPool, length, 'xypD')))
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						Irp->IoStatus.Information = 0;
						goto ERRERR;
					}
					RtlZeroMemory(devBuf,length);
					ntStatus = ZwReadFile(
						DevExt->TempFile,
						NULL,
						NULL,
						NULL,
						&ios,
						fileBuf,
						length,
						&offset,
						NULL);
					if (!NT_SUCCESS(ntStatus))
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						Irp->IoStatus.Information = 0;
						goto ERRERR;
					}
					//把这个irp发给下层设备去获取需要从设备上读取的信息
					ntStatus = DPForwardIrpSync(DevExt->LowerDevObj,Irp);
					if (!NT_SUCCESS(ntStatus))
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						Irp->IoStatus.Information = 0;
						goto ERRERR;
					}
					//将从下层设备获取到的数据存储到devBuf中
					memcpy(devBuf, sysBuf, Irp->IoStatus.Information);
					//把从文件获取到的数据和从设备获取到的数据根据相应的bitmap值来进行合并，合并的结果放在devBuf中
					ntStatus = DPBitmapGet(
						DevExt->Bitmap,
						offset,
						length,
						devBuf,
						fileBuf
						);
					if (!NT_SUCCESS(ntStatus))
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						Irp->IoStatus.Information = 0;
						goto ERRERR;
					}
					//把合并完的数据存入系统缓冲并完成irp
					memcpy(sysBuf, devBuf, Irp->IoStatus.Information);
					goto ERRCMPLT;
				default:
					ntStatus = STATUS_INSUFFICIENT_RESOURCES;
					goto ERRERR;
				}
			}
			else
			{
				//这里是写的过程
				//对于写，我们直接写缓冲文件，而不会写磁盘数据，这就是所谓的转储，但是转储之后需要在bitmap中做相应的标记
				ntStatus = ZwWriteFile(
					DevExt->TempFile,
					NULL,
					NULL,
					NULL,
					&ios,
					sysBuf,
					length,
					&offset,
					NULL);
				if(!NT_SUCCESS(ntStatus))
				{
					ntStatus = STATUS_INSUFFICIENT_RESOURCES;
					goto ERRERR;
				}
				else
				{
					if (NT_SUCCESS(ntStatus = DPBitmapSet(DevExt->Bitmap, offset, length)))
					{
						goto ERRCMPLT;
					}
					else
					{
						ntStatus = STATUS_INSUFFICIENT_RESOURCES;
						goto ERRERR;
					}
				}
			}
ERRERR:
			if (NULL != fileBuf)
			{
				ExFreePool(fileBuf);
				fileBuf = NULL;
			}
			if (NULL != devBuf)
			{
				ExFreePool(devBuf);
				devBuf = NULL;
			}
			DPCompleteRequest(
				Irp,
				ntStatus,
				IO_NO_INCREMENT
				);
			continue;
ERRNEXT:
			if (NULL != fileBuf)
			{
				ExFreePool(fileBuf);
				fileBuf = NULL;
			}
			if (NULL != devBuf)
			{
				ExFreePool(devBuf);
				devBuf = NULL;
			}	
			DPSendToNextDriver(
				DevExt->LowerDevObj,
				Irp);
			continue;
ERRCMPLT:
			if (NULL != fileBuf)
			{
				ExFreePool(fileBuf);
				fileBuf = NULL;
			}
			if (NULL != devBuf)
			{
				ExFreePool(devBuf);
				devBuf = NULL;
			}
			DPCompleteRequest(
				Irp,
				STATUS_SUCCESS,
				IO_DISK_INCREMENT
				);
			continue;
			
		}
	}

}



NTSTATUS
DPAddDevice(
    IN	PDRIVER_OBJECT	DriverObject,
    IN	PDEVICE_OBJECT	PhysicalDeviceObject
    )
{
	//NTSTATUS类型的函数返回值
	NTSTATUS					ntStatus = STATUS_SUCCESS;
    //用来指向过滤设备的设备扩展的指针
	PDP_FILTER_DEV_EXTENSION	DevExt = NULL;
	//过滤设备的下层设备的指针对象
	PDEVICE_OBJECT				LowerDevObj = NULL;
	//过滤设备的设备指针的指针对象
	PDEVICE_OBJECT				FltDevObj = NULL;
	//过滤设备的处理线程的线程句柄
	HANDLE						ThreadHandle = NULL;

	//建立一个过滤设备，这个设备是FILE_DEVICE_DISK类型的设备并且具有DP_FILTER_DEV_EXTENSION类型的设备扩展
	ntStatus = IoCreateDevice(
		DriverObject,
		sizeof(DP_FILTER_DEV_EXTENSION),
		NULL,
		FILE_DEVICE_DISK,
		FILE_DEVICE_SECURE_OPEN,
		FALSE,
		&FltDevObj);
	if (!NT_SUCCESS(ntStatus)) 
		goto ERROUT;
	//将DevExt指向过滤设备的设备扩展指针
	DevExt = FltDevObj->DeviceExtension;
	//清空过滤设备的设备扩展
	RtlZeroMemory(DevExt,sizeof(DP_FILTER_DEV_EXTENSION));

	//将刚刚建立的过滤设备附加到这个卷设备的物理设备上
	LowerDevObj = IoAttachDeviceToDeviceStack(
		FltDevObj, 
		PhysicalDeviceObject);
	if (NULL == LowerDevObj)
	{
		ntStatus = STATUS_NO_SUCH_DEVICE;
		goto ERROUT;
	}

	//初始化这个卷设备的分页路径计数的计数事件
	KeInitializeEvent(
		&DevExt->PagingPathCountEvent,
		NotificationEvent, 
		TRUE);

	//对过滤设备的设备属性进行初始化，过滤设备的设备属性应该和它的下层设备相同
	FltDevObj->Flags = LowerDevObj->Flags;
	//给过滤设备的设备属性加上电源可分页的属性
	FltDevObj->Flags |= DO_POWER_PAGABLE;
	//对过滤设备进行设备初始化
	FltDevObj->Flags &= ~DO_DEVICE_INITIALIZING;

	//将过滤设备对应的设备扩展中的相应变量进行初始化
	//卷设备的过滤设备对象
	DevExt->FltDevObj = FltDevObj;
	//卷设备的物理设备对象
	DevExt->PhyDevObj = PhysicalDeviceObject;
	//卷设备的下层设备对象
	DevExt->LowerDevObj = LowerDevObj;

	//初始化这个卷的请求处理队列
	InitializeListHead(&DevExt->ReqList);
	//初始化请求处理队列的锁
	KeInitializeSpinLock(&DevExt->ReqLock);
	//初始化请求处理队列的同步事件
	KeInitializeEvent(
		&DevExt->ReqEvent,
		SynchronizationEvent,
		FALSE
		);

	//初始化终止处理线程标志
	DevExt->ThreadTermFlag = FALSE;
	//建立用来处理这个卷的请求的处理线程，线程函数的参数则是设备扩展
	ntStatus = PsCreateSystemThread(
		&ThreadHandle,
		(ACCESS_MASK)0L,
		NULL,
		NULL,
		NULL,
		DPReadWriteThread,
		DevExt
		);
	if (!NT_SUCCESS(ntStatus))
		goto ERROUT;

	//获取处理线程的对象
	ntStatus = ObReferenceObjectByHandle(
		ThreadHandle,
		THREAD_ALL_ACCESS,
		NULL,
		KernelMode,
		&DevExt->ThreadHandle,
		NULL
		);
	if (!NT_SUCCESS(ntStatus))
	{
		DevExt->ThreadTermFlag = TRUE;
		KeSetEvent(
			&DevExt->ReqEvent,
			(KPRIORITY)0,
			FALSE
			);
		goto ERROUT;
	}

ERROUT:
	if (!NT_SUCCESS(ntStatus))
	{	
		//如果上面有不成功的地方，首先需要解除可能存在的附加
		if (NULL != LowerDevObj)
		{
			IoDetachDevice(LowerDevObj);
			DevExt->LowerDevObj = NULL;
		}
		//然后删除可能建立的过滤设备
		if (NULL != FltDevObj)
		{
			IoDeleteDevice(FltDevObj);
			DevExt->FltDevObj = NULL;
		}
	}
	//关闭线程句柄，我们今后不会用到它，所有对线程的引用都通过线程对象来进行了
	if (NULL != ThreadHandle)
		ZwClose(ThreadHandle);
	//返回状态值
    return ntStatus;
}

VOID
DPUnload(
	IN	PDRIVER_OBJECT	DriverObject
	)
{
	//这个驱动将会工作到系统关机，所以我们不会在驱动卸载的时候做任何清理动作，因为之后系统马上就关闭了
	UNREFERENCED_PARAMETER(DriverObject);
	return;
}

NTSTATUS
DPDispatchAny(
    IN	PDEVICE_OBJECT	DeviceObject,
    IN	PIRP			Irp
    )
{
	//NTSTATUS类型的函数返回值
	NTSTATUS					ntStatus = STATUS_SUCCESS;
	//用来指向过滤设备的设备扩展的指针
	PDP_FILTER_DEV_EXTENSION	DevExt = DeviceObject->DeviceExtension;
    //对于我们不感兴趣的irp请求，标准的处理方式直接下发给下层设备去处理
	return DPSendToNextDriver(
		DevExt->LowerDevObj,
		Irp);
}

NTSTATUS
DPDispatchPower(
    IN	PDEVICE_OBJECT	DeviceObject,
    IN	PIRP			Irp
    )
{
	//用来指向过滤设备的设备扩展的指针
	PDP_FILTER_DEV_EXTENSION	DevExt = DeviceObject->DeviceExtension;
#if (NTDDI_VERSION < NTDDI_VISTA)
	//如果是vista以前的版本的windows，需要使用特殊的向下层设备转发的函数
	PoStartNextPowerIrp(Irp);
	IoSkipCurrentIrpStackLocation(Irp);
	return PoCallDriver(DevExt->LowerDevObj, Irp);
#else
	//如果是vista系统，可以使用和一般下发irp一样的方法来下发
	return DPSendToNextDriver(
		DevExt->LowerDevObj,
		Irp);
#endif  
}

NTSTATUS	
DPDispatchPnp(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp
	)
{
	//用来指向过滤设备的设备扩展的指针
	PDP_FILTER_DEV_EXTENSION	DevExt = DeviceObject->DeviceExtension;
	//返回值
	NTSTATUS ntStatus = STATUS_SUCCESS;
	//用来指向irp stack的指针
	PIO_STACK_LOCATION  irpsp = IoGetCurrentIrpStackLocation(Irp);

	switch(irpsp->MinorFunction) 
	{
	case IRP_MN_REMOVE_DEVICE:
		//如果是PnP manager发过来的移除设备的irp，将进入这里
		{
			//这里主要做一些清理工作
			if (DevExt->ThreadTermFlag != TRUE && NULL != DevExt->ThreadHandle)
			{
				//如果线程还在运行的话需要停止它，这里通过设置线程停止运行的标志并且发送事件信息，让线程自己终止运行
				DevExt->ThreadTermFlag = TRUE;
				KeSetEvent(
					&DevExt->ReqEvent,
					(KPRIORITY) 0,
					FALSE
					);
				//等待线程结束
				KeWaitForSingleObject(
					DevExt->ThreadHandle,
					Executive,
					KernelMode,
					FALSE,
					NULL
					);
				//解除引用线程对象
				ObDereferenceObject(DevExt->ThreadHandle);
			}
	 		if (NULL != DevExt->Bitmap)
	 		{
				//如果还有位图，就释放
	 			DPBitmapFree(DevExt->Bitmap);
	 		}
			if (NULL != DevExt->LowerDevObj)
			{
				//如果存在着下层设备，就先去掉挂接
				IoDetachDevice(DevExt->LowerDevObj);
			}
	 		if (NULL != DevExt->FltDevObj)
	 		{
				//如果存在过滤设备，就要删除它
	 			IoDeleteDevice(DevExt->FltDevObj);
	 		}
			break;
		}
	//这个是PnP 管理器用来询问设备能否支持特殊文件的irp，作为卷的过滤驱动，我们必须处理
	case IRP_MN_DEVICE_USAGE_NOTIFICATION:
		{
			BOOLEAN setPagable;
			//如果是询问是否支持休眠文件和dump文件，则直接下发给下层设备去处理
			if (irpsp->Parameters.UsageNotification.Type != DeviceUsageTypePaging) 
			{
				ntStatus = DPSendToNextDriver(
					DevExt->LowerDevObj,
					Irp);
				return ntStatus; 
			}
			//这里等一下分页计数事件
			ntStatus = KeWaitForSingleObject(
				&DevExt->PagingPathCountEvent,
				Executive, 
				KernelMode,
				FALSE, 
				NULL);

			//setPagable初始化为假，是没有设置过DO_POWER_PAGABLE的意思
			setPagable = FALSE;
			if (!irpsp->Parameters.UsageNotification.InPath &&
				DevExt->PagingPathCount == 1 ) 
			{
				//如果是PnP manager通知我们将要删去分页文件，且我们目前只剩下最后一个分页文件的时候会进入这里
				if (DeviceObject->Flags & DO_POWER_INRUSH)
				{} 
				else 
				{
					//到这里说明没有分页文件在这个设备上了，需要设置DO_POWER_PAGABLE这一位了
					DeviceObject->Flags |= DO_POWER_PAGABLE;
					setPagable = TRUE;
				}
			}
			//到这里肯定是关于分页文件的是否可建立查询，或者是删除的通知，我们交给下层设备去做。这里需要用同步的方式给下层设备，也就是说要等待下层设备的返回
			ntStatus = DPForwardIrpSync(DevExt->LowerDevObj,Irp);

			if (NT_SUCCESS(ntStatus)) 
			{
				//如果发给下层设备的请求成功了，说明下层设备支持这个操作，会执行到这里
				//在成功的条件下我们来改变我们自己的计数值，这样就能记录我们现在这个设备上到底有多少个分页文件
				IoAdjustPagingPathCount(
					&DevExt->PagingPathCount,
					irpsp->Parameters.UsageNotification.InPath);
				if (irpsp->Parameters.UsageNotification.InPath) 
				{
					if (DevExt->PagingPathCount == 1) 
					{
						//如果这个请求是一个建立分页文件的查询请求，并且下层设备支持这个请求，而且这是第一个在这个设备上的分页文件，那么我们需要清除DO_POWER_PAGABLE位
						DeviceObject->Flags &= ~DO_POWER_PAGABLE;
					}
				}
			}
			else 
			{
				//到这里说明给下层设备发请求失败了，下层设备不支持这个请求，这时候我们需要把之前做过的操作还原
				if (setPagable == TRUE) 
				{
					//根据setPagable变量的值来判断我们之前是否做过对DO_POWER_PAGABLE的设置，如果有的话就清楚这个设置
					DeviceObject->Flags &= ~DO_POWER_PAGABLE;
					setPagable = FALSE;
				}
			}
			//设置分页计数事件
			KeSetEvent(
				&DevExt->PagingPathCountEvent,
				IO_NO_INCREMENT, 
				FALSE
				);
			//到这里我们就可以完成这个irp请求了
			IoCompleteRequest(Irp, IO_NO_INCREMENT);
			return ntStatus;
		}		
	default:
		break;
	}
	return DPSendToNextDriver(
		DevExt->LowerDevObj,
		Irp);

}

NTSTATUS DPQueryVolumeInformationCompletionRoutine(
	IN	PDEVICE_OBJECT		pDeviceObject,
	IN	PIRP				pIrp,
	IN	PVOID				Context
	)
{
	KeSetEvent((PKEVENT)Context, IO_NO_INCREMENT, FALSE);
	return STATUS_MORE_PROCESSING_REQUIRED;
}

NTSTATUS DPQueryVolumeInformation(
	PDEVICE_OBJECT			DevObj,
	LARGE_INTEGER *			TotalSize,
	DWORD *					ClusterSize,
	DWORD *					SectorSize
	)
{
#define _FileSystemNameLength	64
//定义FAT16文件系统签名的偏移量
#define FAT16_SIG_OFFSET	54
//定义FAT32文件系统签名的偏移量
#define FAT32_SIG_OFFSET	82
//定义NTFS文件系统签名的偏移量
#define NTFS_SIG_OFFSET		3
	//这是FAT16文件系统的标志
	const UCHAR FAT16FLG[4] = {'F','A','T','1'};
	//这是FAT32文件系统的标志
	const UCHAR FAT32FLG[4] = {'F','A','T','3'};
	//这是NTFS文件系统的标志
	const UCHAR NTFSFLG[4] = {'N','T','F','S'};
	//返回值
	NTSTATUS ntStatus = STATUS_SUCCESS;
	//用来读取卷DBR扇区的数据缓冲区
	BYTE DBR[512] = { 0 };
	//DBR扇区有512个bytes大小
	ULONG DBRLength = 512;
	//以下是三个指针，统一指向读取的DBR数据，但是这三个指针的类型分别代表FAT16，FAT32和NTFS类型文件系统的DBR数据结构
	PDP_NTFS_BOOT_SECTOR pNtfsBootSector = (PDP_NTFS_BOOT_SECTOR)DBR;
	PDP_FAT32_BOOT_SECTOR pFat32BootSector = (PDP_FAT32_BOOT_SECTOR)DBR;
	PDP_FAT16_BOOT_SECTOR pFat16BootSector = (PDP_FAT16_BOOT_SECTOR)DBR;
	//读取的偏移量，对于DBR来说是卷的起始位置，所以偏移量为0
	LARGE_INTEGER readOffset = { 0 };
	//读取时的io操作状态
	IO_STATUS_BLOCK ios;
 	//为了同步读取所设置的同步事件
 	KEVENT Event;
 	//为了同步读取所需要构建的irp指针
 	PIRP   pIrp	= NULL;

	//下面我们首先从指定的卷设备上读取偏移量为0的一个扇区，也就是这个卷的DBR扇区，准备加以分析
 	//因为我们要同步读取，所以先初始化一个为了同步读取设置的事件
 	KeInitializeEvent(&Event, NotificationEvent, FALSE);
 	//构造一个irp用来发给卷设备来读取信息
 	pIrp = IoBuildAsynchronousFsdRequest(
 		IRP_MJ_READ,
 		DevObj,
 		DBR,
 		DBRLength,
 		&readOffset,
 		&ios
 		);
 	if (NULL == pIrp)
 	{
 		goto ERROUT;
 	}
 	//设置完成函数，并且将同步事件作为完成函数的参数传入
 	IoSetCompletionRoutine(
 		pIrp,
 		DPQueryVolumeInformationCompletionRoutine,
 		&Event,
 		TRUE,
 		TRUE,
 		TRUE
 		);
 	//调用目标设备去处理这个irp
 	ntStatus = IoCallDriver(DevObj, pIrp);
 	if(ntStatus = STATUS_PENDING)
 	{
 		//如果下层设备一时不能完成这个irp请求，我们就等
 		ntStatus = KeWaitForSingleObject(
 			&Event,
 			Executive,
 			KernelMode,
 			FALSE,
 			NULL
 			);
 		//将返回值设置为这个io操作的状态
 		ntStatus = pIrp->IoStatus.Status;
 		if (!NT_SUCCESS(ntStatus))
 		{
 			goto ERROUT;
 		}
 	}
	
	if (*(DWORD*)NTFSFLG == *(DWORD*)&DBR[NTFS_SIG_OFFSET])
	{
		//通过比较标志发现这个卷是一个ntfs文件系统的卷，下面根据ntfs卷的DBR定义来对各种需要获取的值进行赋值操作
		*SectorSize = (DWORD)(pNtfsBootSector->BytesPerSector);
		*ClusterSize = (*SectorSize) * (DWORD)(pNtfsBootSector->SectorsPerCluster);    
		TotalSize->QuadPart = (LONGLONG)(*SectorSize) * (LONGLONG)pNtfsBootSector->TotalSectors;
	}
	else if (*(DWORD*)FAT32FLG == *(DWORD*)&DBR[FAT32_SIG_OFFSET])
	{
		//通过比较标志发现这个卷是一个ntfs文件系统的卷，下面根据ntfs卷的DBR定义来对各种需要获取的值进行赋值操作
		*SectorSize = (DWORD)(pFat32BootSector->BytesPerSector);
		*ClusterSize = (*SectorSize) * (DWORD)(pFat32BootSector->SectorsPerCluster);    
		TotalSize->QuadPart = (LONGLONG)(*SectorSize) * 
			(LONGLONG)(pFat32BootSector->LargeSectors + pFat32BootSector->Sectors);
	}
	else if (*(DWORD*)FAT16FLG == *(DWORD*)&DBR[FAT16_SIG_OFFSET])
	{
		//通过比较标志发现这个卷是一个ntfs文件系统的卷，下面根据ntfs卷的DBR定义来对各种需要获取的值进行赋值操作
		*SectorSize = (DWORD)(pFat16BootSector->BytesPerSector);
		*ClusterSize = (*SectorSize) * (DWORD)(pFat16BootSector->SectorsPerCluster);    
		TotalSize->QuadPart = (LONGLONG)(*SectorSize) * 
			(LONGLONG)(pFat16BootSector->LargeSectors + pFat16BootSector->Sectors);
	}
	else
	{
		//走到这里，可能是其它任何文件系统，但是不是windows认识的文件系统，我们统一返回错
		ntStatus = STATUS_UNSUCCESSFUL;
	}
ERROUT:
 	if (NULL != pIrp)
 	{
 		IoFreeIrp(pIrp);
 	}
	return ntStatus;
}

NTSTATUS
DPVolumeOnLineCompleteRoutine(
	IN PDEVICE_OBJECT  DeviceObject,
	IN PIRP  Irp,
	IN PVOLUME_ONLINE_CONTEXT  Context
	)
{
	//返回值
	NTSTATUS ntStatus = STATUS_SUCCESS;
	//这个卷设备的dos名字，也就是C，D等
	UNICODE_STRING		DosName = { 0 };

	//在这里Context是不可能为空的，为空就是出错了
	ASSERT(Context!=NULL);
	//下面调用我们自己的VolumeOnline处理
	//获取这个卷的dos名字
	ntStatus = IoVolumeDeviceToDosName(Context->DevExt->PhyDevObj, &DosName);
	if (!NT_SUCCESS(ntStatus))
		goto ERROUT;
	//将dos名字变成大写形式
	Context->DevExt->VolumeLetter = DosName.Buffer[0];
	if (Context->DevExt->VolumeLetter > L'Z')
		Context->DevExt->VolumeLetter -= (L'a' - L'A');
	//我们只保护“D”盘
	if (Context->DevExt->VolumeLetter == L'D')
	{
		//获取这个卷的基本信息
		ntStatus = DPQueryVolumeInformation(
			Context->DevExt->PhyDevObj,
			&(Context->DevExt->TotalSizeInByte),
			&(Context->DevExt->ClusterSizeInByte),
			&(Context->DevExt->SectorSizeInByte));
		if (!NT_SUCCESS(ntStatus))
		{
			goto ERROUT;
		}
		//建立这个卷对应的位图
		ntStatus = DPBitmapInit(
			&Context->DevExt->Bitmap,
			Context->DevExt->SectorSizeInByte,
			8,
			25600,
			(DWORD)(Context->DevExt->TotalSizeInByte.QuadPart / 
			(LONGLONG)(25600 * 8 * Context->DevExt->SectorSizeInByte)) + 1);
		if (!NT_SUCCESS(ntStatus))
			goto ERROUT;
		//对全局量赋值，说明我们找到需要保护的那个设备了
		gProtectDevExt = Context->DevExt;
	}
	
ERROUT:
	if (!NT_SUCCESS(ntStatus))
	{
		if (NULL != Context->DevExt->Bitmap)
		{
			DPBitmapFree(Context->DevExt->Bitmap);
		}
		if (NULL != Context->DevExt->TempFile)
		{
			ZwClose(Context->DevExt->TempFile);
		}
	}
	if (NULL != DosName.Buffer)
	{
		ExFreePool(DosName.Buffer);
	}
	//设置等待同步事件，这样可以让我们等待的DeviceIoControl处理过程继续运行
	KeSetEvent(
		Context->Event,
		0,
		FALSE);
	return STATUS_SUCCESS;
}

NTSTATUS
DPDispatchDeviceControl(
	IN	PDEVICE_OBJECT	DeviceObject,
	IN	PIRP			Irp
	)
{
	//用来指向过滤设备的设备扩展的指针
	PDP_FILTER_DEV_EXTENSION	DevExt = DeviceObject->DeviceExtension;
	//返回值
	NTSTATUS ntStatus = STATUS_SUCCESS;
	//用来指向irp stack的指针
	PIO_STACK_LOCATION  irpsp = IoGetCurrentIrpStackLocation(Irp);
	//用来同步IOCTL_VOLUME_ONLINE处理的事件
	KEVENT					Event;
	//用来传给IOCTL_VOLUME_ONLINE的完成函数的上下文
	VOLUME_ONLINE_CONTEXT	context;

	switch (irpsp->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_VOLUME_ONLINE:
		{
			//如果是卷设备的IOCTL_VOLUME_ONLINE，会进入到这里
			//我们打算自己处理这个irp请求，这里先初始化一个事件用来在这个请求的完成函数里面做同步信号
			KeInitializeEvent(&Event, NotificationEvent, FALSE);
			//给这个请求的完成函数初始化参数
			context.DevExt = DevExt;
			context.Event = &Event;
			//这里copy一份irp stack
			IoCopyCurrentIrpStackLocationToNext(Irp);
			//设置完成函数
			IoSetCompletionRoutine(
				Irp, 
				DPVolumeOnLineCompleteRoutine, 
				&context,
				TRUE,
				TRUE,
				TRUE);
			//调用下层设备来处理这个irp
			ntStatus = IoCallDriver(DevExt->LowerDevObj, Irp);
			//等待下层设备处理结束这个irp
			KeWaitForSingleObject(
				&Event,
				Executive,
				KernelMode,
				FALSE,
				NULL);
			//返回
			return ntStatus;
		}
	default:
		//对于其它DeviceIoControl，我们一律调用下层设备去处理
		break;
	}
	return DPSendToNextDriver(DevExt->LowerDevObj,Irp);		
}

NTSTATUS
DPDispatchReadWrite(
    IN	PDEVICE_OBJECT	DeviceObject,
    IN	PIRP			Irp
    )
{	
	//用来指向过滤设备的设备扩展的指针
	PDP_FILTER_DEV_EXTENSION	DevExt = DeviceObject->DeviceExtension;
	//返回值
	NTSTATUS ntStatus = STATUS_SUCCESS;

	if (DevExt->Protect)
	{
		//这个卷在保护状态，
		//我们首先把这个irp设为pending状态
		IoMarkIrpPending(Irp);
		//然后将这个irp放进相应的请求队列里
		ExInterlockedInsertTailList(
			&DevExt->ReqList,
			&Irp->Tail.Overlay.ListEntry,
			&DevExt->ReqLock
			);
		//设置队列的等待事件，通知队列对这个irp进行处理
		KeSetEvent(
			&DevExt->ReqEvent, 
			(KPRIORITY)0, 
			FALSE);
		//返回pending状态，这个irp就算处理完了
		return STATUS_PENDING;
	}
	else
	{
		//这个卷不在保护状态，直接交给下层设备进行处理
		return DPSendToNextDriver(
			DevExt->LowerDevObj,
			Irp);
	}
}
