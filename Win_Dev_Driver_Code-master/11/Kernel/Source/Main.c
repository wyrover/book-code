/*++

Copyright (c) 张佩 01/05/2010

Module Name:
    Virtual ASIO.c

Abstract:
    虚拟ASIO声卡驱动，配合ASIO音频软件，实现音频数据Loopback。

Revision History:
    1.0.001

--*/

#include <ntddk.h>
#include "public.h" //common to app and driver
#include "Main.h" // private to driver
#include <stdio.h>
#include <ntstrsafe.h>
#include <stdarg.h>

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, DispatchCreateClose)
#pragma alloc_text (PAGE, DrvUnload)
#endif


NTSTATUS DriverEntry(
    __in PDRIVER_OBJECT  DriverObject,
    __in PUNICODE_STRING RegistryPath
    )
{
    PDEVICE_OBJECT      deviceObject;
    PDEVICE_EXTENSION   pContext;
    UNICODE_STRING      ntDeviceName;
    UNICODE_STRING      symbolicLinkName;
    NTSTATUS            status;

    UNREFERENCED_PARAMETER(RegistryPath);

    DebugPrint(("==>DriverEntry\n"));

    // 创建设备对象
    RtlInitUnicodeString(&ntDeviceName, NTDEVICE_NAME_STRING);

    status = IoCreateDevice(DriverObject,               // DriverObject
                            sizeof(DEVICE_EXTENSION),   // DeviceExtensionSize
                            &ntDeviceName,              // DeviceName
                            FILE_DEVICE_UNKNOWN,        // DeviceType
                            FILE_DEVICE_SECURE_OPEN,    // DeviceCharacteristics
                            FALSE,                      // Not Exclusive
                            &deviceObject);             // DeviceObject
        
    if (!NT_SUCCESS(status)) {
        DebugPrint(("\tIoCreateDevice returned 0x%x\n", status));
        return(status);
    }

    DriverObject->MajorFunction[IRP_MJ_CREATE]          = DispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_CLOSE]           = DispatchCreateClose;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]  = DispatchDispatchIoControl;
    DriverObject->DriverUnload                          = DrvUnload;

    RtlInitUnicodeString(&symbolicLinkName, SYMBOLIC_NAME_STRING);
    status = IoCreateSymbolicLink(&symbolicLinkName, &ntDeviceName);

    if (!NT_SUCCESS(status)) 
	{
        IoDeleteDevice(deviceObject);
        DebugPrint(("\tIoCreateSymbolicLink returned 0x%x\n", status));
        return(status);
    }

    pContext = deviceObject->DeviceExtension;
    RtlZeroMemory(pContext, sizeof(DEVICE_EXTENSION));

    pContext->Self = deviceObject;
	pContext->bStoppingDPC = TRUE;
	pContext->ulSampleRate = 44100; // 初始采样率。我们没有提供给用户设置采样率的接口。所以它总是固定的。

    // 初始化计时器
    KeInitializeDpc (
        &pContext->TimerDpc,
        TimerDpc,
        deviceObject
        );
	
    KeInitializeEvent (
        &pContext->StopDPCEvent,
        SynchronizationEvent,
        FALSE
        );

    KeInitializeTimer (&pContext->Timer);

    // 用户缓冲区方式
    deviceObject->Flags |= DO_BUFFERED_IO;
    return status;
}

// 驱动卸载的时候将调用DrvUnload函数
VOID DrvUnload(
    __in PDRIVER_OBJECT DriverObject
    )
{
    PDEVICE_OBJECT       deviceObject = DriverObject->DeviceObject;
    PDEVICE_EXTENSION    pContext = deviceObject->DeviceExtension;
    UNICODE_STRING      symbolicLinkName;

    DebugPrint(("==>Unload\n"));

    PAGED_CODE();

    StopDPC( deviceObject );
    RtlInitUnicodeString(&symbolicLinkName, SYMBOLIC_NAME_STRING);
    IoDeleteSymbolicLink(&symbolicLinkName);
    IoDeleteDevice(deviceObject);

    return;
}

// IRP_MJ_CREATE/IRP_MJ_CLOSE处理函数
NTSTATUS DispatchCreateClose(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
    )
{
    PIO_STACK_LOCATION irpStack;
    NTSTATUS            status;
    PFILE_CONTEXT       fileContext;

    UNREFERENCED_PARAMETER(DeviceObject);

    PAGED_CODE();

    irpStack = IoGetCurrentIrpStackLocation(Irp);

    ASSERT(irpStack->FileObject != NULL);    

    switch (irpStack->MajorFunction)
    {
	case IRP_MJ_CREATE:
		{
			DebugPrint(("IRP_MJ_CREATE\n"));

			fileContext = (PFILE_CONTEXT)ExAllocatePoolWithQuotaTag(
				NonPagedPool, 
				sizeof(FILE_CONTEXT),
				TAG);

			if (NULL == fileContext) 
			{
				status =  STATUS_INSUFFICIENT_RESOURCES;
				break;
			}

			IoInitializeRemoveLock(&fileContext->FileRundownLock, TAG, 0, 0);

			ASSERT(irpStack->FileObject->FsContext == NULL);
			irpStack->FileObject->FsContext = (PVOID) fileContext;

			status = STATUS_SUCCESS;
			break;
		}

	case IRP_MJ_CLOSE:
		{
			DebugPrint(("IRP_MJ_CLOSE\n"));

			fileContext = irpStack->FileObject->FsContext;
			IoAcquireRemoveLock(&fileContext->FileRundownLock, 0);
			IoReleaseRemoveLockAndWait(&fileContext->FileRundownLock, 0);

			ExFreePoolWithTag(fileContext, TAG);

			status = STATUS_SUCCESS;
			break;
		}

	default:
		ASSERT(FALSE);  // should never hit this
		status = STATUS_NOT_IMPLEMENTED;
		break;
    }

    Irp->IoStatus.Status = status;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
    return status;
}

// IRP_MJ_DEVICE_CONTROL处理函数
NTSTATUS DispatchDispatchIoControl(
    __in PDEVICE_OBJECT DeviceObject,
    __in PIRP Irp
    )
{
    PDEVICE_EXTENSION   pContext;
    PIO_STACK_LOCATION  irpStack;
    NTSTATUS    status;
    PFILE_CONTEXT fileContext;
    PVOID lpInBuf;
    PVOID lpOutBuf;
    ULONG ulInLen, ulOutLen;

    pContext = (PDEVICE_EXTENSION)DeviceObject->DeviceExtension;
    irpStack = IoGetCurrentIrpStackLocation(Irp);
    
    ASSERT(irpStack->FileObject != NULL);

    fileContext = irpStack->FileObject->FsContext;    

    status = IoAcquireRemoveLock(
		&fileContext->FileRundownLock,
		Irp);

    if (!NT_SUCCESS(status)) 
	{
        Irp->IoStatus.Status = status;
        IoCompleteRequest(Irp, IO_NO_INCREMENT);
        return status;
    }
    
    lpInBuf = irpStack->Parameters.DeviceIoControl.Type3InputBuffer;
    lpOutBuf = Irp->UserBuffer;

    ulInLen = irpStack->Parameters.DeviceIoControl.InputBufferLength;
    ulOutLen = irpStack->Parameters.DeviceIoControl.OutputBufferLength;

    status = STATUS_INVALID_PARAMETER;

    switch (irpStack->Parameters.DeviceIoControl.IoControlCode)
	{
	case IOCTL_ASIO_START:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_ASIO_START");
			status = AsioStart(DeviceObject);
			break;
		}

	case IOCTL_ASIO_STOP:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_ASIO_STOP");
			status = AsioStop(DeviceObject);
			break;
		}

	case IOCTL_ASIO_BUFFERS:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_ASIO_BUFFERS");
			if(ulOutLen == sizeof(BOOLEAN))
			{
				BOOLEAN bSet = *(BOOLEAN*)lpOutBuf;
				if(bSet)
				{
					if(ulInLen == sizeof(BUFPOINTERSTRUCT))
						status = AsioSetBuffers(DeviceObject, lpInBuf);
				}
				else
				{
					AsioReleaseBuffers(DeviceObject);
					status = STATUS_SUCCESS;
				}
			}
			break;
		}

	case IOCTL_ASIO_BUFFER_READY:
		{
			if(ulInLen == sizeof(ULONG))
				status = AsioBufReady(DeviceObject, *(ULONG*)lpInBuf);
			break;
		}

	case IOCTL_ASIO_BUFFER_SIZE:
		{
			status = STATUS_SUCCESS;

			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_ASIO_BUFFER_SIZE %d %d (%p %p) %d %d",
				ulInLen, ulOutLen, lpInBuf, lpOutBuf, sizeof(BOOLEAN), sizeof(ULONG));

			if(ulInLen == sizeof(BOOLEAN) && ulOutLen == sizeof(ULONG) &&
				lpInBuf && lpOutBuf)
			{
				BOOLEAN bSet = *(BOOLEAN*)lpInBuf;
				if(bSet){
					pContext->ulLatency = *(ULONG*)lpOutBuf;
					KDBG(DPFLTR_INFO_LEVEL, "%d", pContext->ulLatency);
				}
				else{
					*(ULONG*)lpOutBuf = pContext->ulLatency;
					KDBG(DPFLTR_INFO_LEVEL, "%d", pContext->ulLatency);
				}
			}

			break;
		}

	case IOCTL_ASIO_EVENT:
		{
			KDBG(DPFLTR_INFO_LEVEL, "IOCTL_ASIO_EVENT");

			if(ulOutLen == sizeof(BOOLEAN))
			{
				BOOLEAN bSet = *(BOOLEAN*)lpOutBuf;
				if(bSet)
				{
					if(ulInLen == 2 * sizeof(HANDLE))
						status = AsioSetEvent(DeviceObject, (HANDLE*)lpInBuf);
				}                    
				else
				{
					AsioReleaseEvent(DeviceObject);
					status = STATUS_SUCCESS;
				}
			}

			break;
		}

	default:
		status = STATUS_NOT_IMPLEMENTED;
		break;

    }

	Irp->IoStatus.Status = status;
	Irp->IoStatus.Information = 0;
	IoCompleteRequest(Irp, IO_NO_INCREMENT);

    IoReleaseRemoveLock(&fileContext->FileRundownLock, Irp);        
    return status;
}

#if (DBG)
__inline KDBG(int nLevel, char* msg, ...)
{
    static char csmsg[1024];

    va_list argp;
    va_start(argp, msg);
    RtlStringCchVPrintfA(csmsg, 1024, msg, argp);
    va_end(argp);

    KdPrintEx((DPFLTR_DEFAULT_ID, nLevel, "V-ASIO:"));// 格式化头
    KdPrintEx((DPFLTR_DEFAULT_ID, nLevel, csmsg));     // Log body
    KdPrintEx((DPFLTR_DEFAULT_ID, nLevel, "\n"));     // 换行
}
#else
__inline KDBG(int nLevel, char* msg, ...)
{
}
#endif