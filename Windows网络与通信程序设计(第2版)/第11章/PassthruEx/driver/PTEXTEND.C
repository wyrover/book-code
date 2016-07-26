/////////////////////////////////////////////////
// PTEXTEND.c文件


#include "precomp.h"
#pragma hdrstop
#include "iocommon.h"

extern NDIS_SPIN_LOCK	GlobalLock;
extern PADAPT pAdaptList;


//////////////////////////////////////////
// 派遣例程

// 这是处理IRP_MJ_CREATE的派遣例程，我们仅简单的返回成功
NTSTATUS DevOpen(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIrpStack;

	// 初始化这个新的文件对象

	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	pIrpStack->FileObject->FsContext = NULL;
	pIrpStack->FileObject->FsContext2 = NULL;


	DBGPRINT(("  DevOpen: FileObject %p\n", pIrpStack->FileObject));

	// 完成此IRP请求
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}

// 这是处理IRP_MJ_CLEANUP的派遣例程
NTSTATUS DevCleanup(PDEVICE_OBJECT pDeviceObject,PIRP pIrp)
{
    PIO_STACK_LOCATION  pIrpSp;
    NTSTATUS            status = STATUS_SUCCESS;
    POPEN_CONTEXT       pOpenContext;
	
	// 取得句柄
    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);
    pOpenContext = pIrpSp->FileObject->FsContext;
	
    if(pOpenContext)
    {
		// 在这里取消所有未决的IRP。这个例子里没有。
    }

    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	
    return status;
} 

// 这是处理IRP_MJ_CLOSE的派遣例程，如果是适配器句柄，我们要减小对打开环境的引用
NTSTATUS DevClose(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	NTSTATUS status = STATUS_SUCCESS;
	PIO_STACK_LOCATION pIrpStack;
	POPEN_CONTEXT pOpenContext;

	pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
	pOpenContext = (POPEN_CONTEXT)pIrpStack->FileObject->FsContext;


	pIrpStack->FileObject->FsContext = NULL;
	pIrpStack->FileObject->FsContext2 = NULL;

	if(pOpenContext != NULL) // 关闭的是一个适配器句柄
	{
		if(pOpenContext->pAdapt != NULL)
		{
			NdisAcquireSpinLock(&(pOpenContext->pAdapt)->Lock);
			(pOpenContext->pAdapt)->pOpenContext = NULL;
			NdisReleaseSpinLock(&(pOpenContext->pAdapt)->Lock);
		}
		DevDerefOpenContext(pOpenContext);
	}

	// 完成此IRP请求
	pIrp->IoStatus.Information = 0;
	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}


// 这是处理IRP_MJ_DEVICE_CONTROL的派遣例程，如果是适配器句柄，我们要减小对打开环境的引用
NTSTATUS DevIoControl(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	// 假设失败
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	// 取得此IRP（pIrp）的I/O堆栈指针
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// 取得I/O控制代码
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// 取得I/O缓冲区指针和它的长度
	PVOID pIoBuffer = pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;

	ULONG uTransLen = 0;

	DBGPRINT((" DevIoControl... \n"));

	switch(uIoControlCode)
	{
	case IOCTL_PTUSERIO_ENUMERATE:
		{
			status = DevGetBindingList(pIoBuffer, uOutSize, &uTransLen);
		}
		break;
	case IOCTL_PTUSERIO_OPEN_ADAPTER: // 打开一个适配器。实际上是为适配器关联一个OPEN_CONTEXT结构
		{
			POPEN_CONTEXT pOpenContext;
			PADAPT pAdapt = LookupAdaptByName((PUCHAR)pIoBuffer, uInSize);
			if(pAdapt == NULL)
			{
				status = STATUS_OBJECT_NAME_NOT_FOUND;
				break;
			}

			
			// 如果正在Unbind，则失败
			NdisAcquireSpinLock(&pAdapt->Lock);  
			if(pAdapt->UnbindingInProcess)
			{
				NdisReleaseSpinLock(&pAdapt->Lock);
				PtDerefAdapter(pAdapt);
				status = STATUS_INVALID_DEVICE_STATE;
				break;
			}
			NdisReleaseSpinLock(&pAdapt->Lock);

			// 如果适配器已经打开，则失败
			if(pAdapt->pOpenContext != NULL)
			{
				PtDerefAdapter(pAdapt);
				status = STATUS_DEVICE_BUSY;
				break;
			}

			// 为新的OPEN_CONTEXT结构申请内存空间
			pOpenContext = DevAllocateOpenContext(pAdapt);
			if(pOpenContext == NULL)
			{
				PtDerefAdapter(pAdapt);
				status = STATUS_INSUFFICIENT_RESOURCES;
				break;
			}


				// 在ADAPT结构中保存pOpenContext指针
			// InterlockedXXX函数执行原子操作：首先它将pAdapt->pOpenContext
			// 与NULL检查，如果它们相等，这个函数将pOpenContext放入pAdapt->pOpenContext，
			// 返回NULL。否则，它仅返回现存的Adapt->pOpenContext，不改变任何值。
	
			/* 功能上相当于 
			if(pAdapt->pOpenContext == NULL)
			{
				pAdapt->pOpenContext = pOpenContext;
			}
			else
			{
				// error 
			}*/
			if(InterlockedCompareExchangePointer(&(pAdapt->pOpenContext), 
													pOpenContext, NULL) != NULL)
			{
				PtDerefAdapter(pAdapt);
				status = STATUS_DEVICE_BUSY;
				break;
			}

			// 将打开环境与句柄关联
			pIrpStack->FileObject->FsContext = pOpenContext;

			status = STATUS_SUCCESS;
		}
		break;
	case IOCTL_PTUSERIO_QUERY_OID:
	case IOCTL_PTUSERIO_SET_OID: 
		{
			return DevHandleOidRequest(pDeviceObject, pIrp);
		}
		break;
	default:
		return FltDevIoControl(pDeviceObject, pIrp);
	}


	if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uTransLen;
	else
		pIrp->IoStatus.Information = 0;

	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}


//////////////////////////////////
// 处理IOCTL时要使用的函数

// 处理用户的OID请求
NTSTATUS DevHandleOidRequest(PDEVICE_OBJECT pDeviceObject, PIRP pIrp)
{
	// 假设失败
	NTSTATUS status = STATUS_INVALID_DEVICE_REQUEST;

	// 取得此IRP（pIrp）的I/O堆栈指针
	PIO_STACK_LOCATION pIrpStack = IoGetCurrentIrpStackLocation(pIrp);

	// 取得I/O控制代码
	ULONG uIoControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
	// 取得I/O缓冲区指针和它的长度
	PPTUSERIO_OID_DATA pOidData = (PPTUSERIO_OID_DATA)pIrp->AssociatedIrp.SystemBuffer;
	ULONG uInSize = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
	ULONG uOutSize = pIrpStack->Parameters.DeviceIoControl.OutputBufferLength;
	ULONG uTransLen = 0;

	POPEN_CONTEXT pOpenContext;
	PADAPT pAdapt;
	do
	{
		// 取得与此句柄关联的OPEN_CONTEXT结构的指针	// 首先检查此句柄是否打开适配器
		pOpenContext = (POPEN_CONTEXT)pIrpStack->FileObject->FsContext;
		if(pOpenContext == NULL)
		{
			status = STATUS_INVALID_HANDLE;
			break;
		}
		pAdapt = pOpenContext->pAdapt;
		if(pAdapt == NULL)
		{
			status = STATUS_INVALID_HANDLE;
			break;
		}
		
		// 检查缓冲区
		if(uOutSize != uInSize || uOutSize < sizeof(PTUSERIO_OID_DATA) ||
					uOutSize < sizeof(PTUSERIO_OID_DATA) - 1 + pOidData->Length)
		{
			status = STATUS_INVALID_PARAMETER;
			break;
		}
		

		
		// 如果Unbind正在进行，则失败
		NdisAcquireSpinLock(&pAdapt->Lock);
		
		if( pAdapt->UnbindingInProcess )
		{
			NdisReleaseSpinLock(&pAdapt->Lock);
			DBGPRINT(( "      Unbind In Process\n" ));
			status = STATUS_INVALID_DEVICE_STATE;
			break;
		}
		
		//
		// All other queries are failed, if the miniport is not at D0,
		//
		if (pAdapt->MPDeviceState > NdisDeviceStateD0)
		{
			NdisReleaseSpinLock(&pAdapt->Lock);
			DBGPRINT(( "      Invalid Miniport Device State\n" ));
			status = STATUS_INVALID_DEVICE_STATE;
			break;
		}
		
		//
		// This is in the process of powering down the system, always fail the request
		// 
		if (pAdapt->StandingBy == TRUE)
		{
			NdisReleaseSpinLock(&pAdapt->Lock);
			DBGPRINT(( "      Miniport Powering Down\n" ));
			
			status = STATUS_INVALID_DEVICE_STATE;
			break;
		}
		
		NdisReleaseSpinLock(&pAdapt->Lock);
		
			// 检查完毕，最后，进行这个请求

		DevRefOpenContext(pOpenContext);	
		
		// 初始化NDIS_REQUEST结构
		NdisZeroMemory(&pOpenContext->Request, sizeof(pOpenContext->Request));
		if(uIoControlCode == IOCTL_PTUSERIO_SET_OID)
		{ 
			pOpenContext->Request.RequestType = NdisRequestSetInformation;
			pOpenContext->Request.DATA.SET_INFORMATION.Oid = pOidData->Oid;
			pOpenContext->Request.DATA.SET_INFORMATION.InformationBuffer = pOidData->Data;
			pOpenContext->Request.DATA.SET_INFORMATION.InformationBufferLength = pOidData->Length;
		}
		else
		{
			pOpenContext->Request.RequestType = NdisRequestQueryInformation;
			pOpenContext->Request.DATA.QUERY_INFORMATION.Oid = pOidData->Oid;
			pOpenContext->Request.DATA.QUERY_INFORMATION.InformationBuffer = pOidData->Data;
			pOpenContext->Request.DATA.QUERY_INFORMATION.InformationBufferLength = pOidData->Length;
		}
		
		NdisResetEvent( &pOpenContext->RequestEvent);
		
		// 提交这个请求
		NdisRequest(&status, pAdapt->BindingHandle, &pOpenContext->Request);
		
		
		if(status != NDIS_STATUS_PENDING)
		{
			DevRequestComplete(pAdapt, &pOpenContext->Request, status);
		}
		
		// 等待请求的完成，即等待Ndis调用DevRequestComplete例程
		NdisWaitEvent(&pOpenContext->RequestEvent, 0);
		
		if(pOpenContext->RequestStatus == NDIS_STATUS_SUCCESS)
		{
			//  将大小返回到用户缓冲区
			if(uIoControlCode == IOCTL_PTUSERIO_SET_OID) 
			{
				pOidData->Length = pOpenContext->Request.DATA.SET_INFORMATION.BytesRead;
			} 
			else if(uIoControlCode == IOCTL_PTUSERIO_QUERY_OID) 
			{
				pOidData->Length = pOpenContext->Request.DATA.QUERY_INFORMATION.BytesWritten;
			}	
			
			// 设置返回给I/O管理器的信息
			uTransLen = pIrpStack->Parameters.DeviceIoControl.InputBufferLength;
			status = STATUS_SUCCESS;
		}
		else
		{
			status = STATUS_UNSUCCESSFUL;
		}
		
		DevDerefOpenContext(pOpenContext);
	}
	while(FALSE);

	if(status == STATUS_SUCCESS)
		pIrp->IoStatus.Information = uTransLen;
	else
		pIrp->IoStatus.Information = 0;

	pIrp->IoStatus.Status = status;
	IoCompleteRequest(pIrp, IO_NO_INCREMENT);
	return status;
}

VOID DevRequestComplete(PADAPT pAdapt, PNDIS_REQUEST NdisRequest,NDIS_STATUS Status)
{
	POPEN_CONTEXT pOpenContext = CONTAINING_RECORD(NdisRequest, OPEN_CONTEXT, Request);
	pOpenContext->RequestStatus = Status;
    NdisSetEvent(&pOpenContext->RequestEvent);
}

// 获取绑定列表
NTSTATUS DevGetBindingList(
    IN  PVOID              Buffer,		// 缓冲区
    IN  ULONG              Length,		// 缓冲区大小
    IN  OUT PULONG         DataLength	// 返回实际需要的长度
    )
{
	PADAPT pAdapt ;

		// 遍历列表，计算所需的缓冲区大小
	ULONG nRequiredLength = 0;
	ULONG nAdapters = 0;

	NdisAcquireSpinLock(&GlobalLock);

	pAdapt = pAdaptList;
	while(pAdapt != NULL)
	{
		nRequiredLength += pAdapt->DeviceName.Length + sizeof(UNICODE_NULL); 
		nRequiredLength += pAdapt->LowerDeviceName.Length + sizeof(UNICODE_NULL);
		nAdapters++;

		pAdapt = pAdapt->Next;
	}

	// 我们将要以下面的格式返回数据：
	// nAdapters + 一个或者多个（"DeviceName\0" + "LowerDeviceName\0"） + UNICODE_NULL
	// 所以，下面要包含上nAapters和UNICODE_NULL的大小
	nRequiredLength += sizeof(nAdapters) + sizeof(UNICODE_NULL); 

	*DataLength = nRequiredLength;
	if(nRequiredLength > Length) 
	{
       NdisReleaseSpinLock(&GlobalLock);
       return STATUS_BUFFER_TOO_SMALL;
    }

		// 填充缓冲区
	// 首先是适配器数量
	*(PULONG)Buffer = nAdapters;
	Buffer = (PCHAR)Buffer + sizeof(ULONG);

	// 然后复制适配器和符号连接名称
	pAdapt = pAdaptList;
	while(pAdapt != NULL)
	{
		NdisMoveMemory(Buffer,pAdapt->DeviceName.Buffer, pAdapt->DeviceName.Length + sizeof(WCHAR));

		Buffer = (PCHAR)Buffer + pAdapt->DeviceName.Length + sizeof(WCHAR);

        NdisMoveMemory(Buffer,pAdapt->LowerDeviceName.Buffer, pAdapt->LowerDeviceName.Length + sizeof(WCHAR)); 

		Buffer = (PCHAR)Buffer + pAdapt->LowerDeviceName.Length + sizeof(WCHAR);    

		pAdapt = pAdapt->Next;
    }
	
	// 最后的结束标志
    *(PWCHAR)Buffer = UNICODE_NULL;
    
    NdisReleaseSpinLock(&GlobalLock);

	return STATUS_SUCCESS;
}



////////////////////////////////////////////
// 帮助函数

// 更加适配器名称查找适配器的PADAPT结构
PADAPT LookupAdaptByName(PUCHAR pNameBuffer, ULONG nNameLength)
{
	PADAPT pAdapt;

	NdisAcquireSpinLock(&GlobalLock);

	pAdapt = pAdaptList;
	while(pAdapt != NULL)
	{
		if(pAdapt->LowerDeviceName.Length == nNameLength &&
			NdisEqualMemory(pAdapt->LowerDeviceName.Buffer, pNameBuffer, nNameLength))
			break;


		pAdapt = pAdapt->Next;
	}

	// 防止在引用适配器期间，系统释放缓冲区
	if(pAdapt != NULL)
		PtRefAdapter(pAdapt);

	NdisReleaseSpinLock(&GlobalLock);
	return pAdapt;
}

// 申请和初始化一个POPEN_CONTEXT结构
POPEN_CONTEXT DevAllocateOpenContext(PADAPT pAdapt)
{
	POPEN_CONTEXT pOpenContext = NULL;

	// 为OPEN_CONTEXT结构申请内存空间
	NdisAllocateMemoryWithTag(&pOpenContext, sizeof(OPEN_CONTEXT), TAG);
	if(pOpenContext == NULL)
	{
		return NULL;
	}

	// 初始化这个内存空间
	NdisZeroMemory(pOpenContext, sizeof(OPEN_CONTEXT));

	NdisAllocateSpinLock(&pOpenContext->Lock);

	NdisInitializeEvent(&pOpenContext->RequestEvent);

	pOpenContext->RefCount = 1;
	pOpenContext->pAdapt = pAdapt;

	return pOpenContext;
}

// 增加对适配器（PADAPT结构）的引用
VOID PtRefAdapter(PADAPT pAdapt)	
{
   NdisInterlockedIncrement(&pAdapt->RefCount);
}


// 减小对适配器（PADAPT结构）的引用，如果减为0，则释放它占用的内存
VOID PtDerefAdapter(PADAPT pAdapt) 
{
	if(pAdapt == NULL)
		return;
 
	if(NdisInterlockedDecrement(&pAdapt->RefCount) == 0) // 已经没有代码再引用它了，释放内存
	{
		MPFreeAllPacketPools (pAdapt);

// BEGIN_PTEX_FILTER
      //
      // 反初始化此适配器上的过滤相关数据
      //
      FltOnDeinitAdapter(pAdapt);
// END_PTEX_FILTER

		NdisFreeMemory(pAdapt, 0, 0);
	}
}




// 增加对打开环境的引用
VOID DevRefOpenContext(POPEN_CONTEXT pOpenContext) 
{
	// 首先增加对适配器的引用，然后再增加OPEN_CONTEXT的引用计数
   PtRefAdapter(pOpenContext->pAdapt);
   NdisInterlockedIncrement(&pOpenContext->RefCount);
}

// 减少对打开环境的引用，如果减为0，则释放它占用的内存
VOID DevDerefOpenContext(POPEN_CONTEXT pOpenContext) 
{
   PADAPT pAdapt = NULL;
   if(pOpenContext == NULL)
	   return;

   // 首先保存对应的适配器指针，以便后面对它调用PtDerefAdapter函数
   pAdapt = pOpenContext->pAdapt;

   // 减小引用计数，如果没有代码再引用它了，则清除资源
   if(NdisInterlockedDecrement(&pOpenContext->RefCount) == 0)
   {
      NdisFreeSpinLock(&pOpenContext->Lock);
      NdisFreeMemory(pOpenContext, 0, 0);
   }

   // 减少对适配器的引用
   PtDerefAdapter(pAdapt);
}




/*NTSTATUS DevOpenAdapter(PUCHAR pNameBuffer, ULONG nNameLength, )
{

}*/


