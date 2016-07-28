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

void TimerDpc (KDPC* pDPC, void* par1, void* par2, void* par3)
{
    DEVICE_OBJECT *pDevice = (DEVICE_OBJECT*)par1;
    PDEVICE_EXTENSION pContext = (PDEVICE_EXTENSION)pDevice->DeviceExtension;

    ASSERT(pContext);
    pContext->ulTicks++;

    if (!pContext->bStoppingDPC) {

        // 重新发布计时器
        LARGE_INTEGER NextTime;
        LONGLONG llInterval;

        KeSetEvent(
			pContext->hEvent[pContext->nBuffer], 
			IO_SOUND_INCREMENT, 
			FALSE);

        pContext->nBuffer = (pContext->nBuffer == 0 ? 1:0);

        KeQuerySystemTime(&NextTime);
        llInterval = NextTime.QuadPart - pContext->startTime.QuadPart;
        
		if(llInterval - pContext->timerInterval * pContext->ulTicks > 1/3 * pContext->timerInterval ||
            pContext->timerInterval * pContext->ulTicks - llInterval > 1/3 * pContext->timerInterval)
        {
            //KdPrint(("%d, %x", pContext->ulTicks, (ULONG)llInterval));
        }
        
        NextTime.QuadPart = pContext->startTime.QuadPart +
            (pContext->timerInterval * (pContext->ulTicks + 1));
        KeSetTimer (&pContext->Timer, NextTime, &pContext->TimerDpc);

    } else {

        // 结束
        KeSetEvent (&pContext->StopDPCEvent, IO_NO_INCREMENT, FALSE);
    }
}

void StartDPC( IN DEVICE_OBJECT* Device, IN LONGLONG TimerInterval )
{
    LARGE_INTEGER NextTime;
    PDEVICE_EXTENSION pContext = (PDEVICE_EXTENSION)Device->DeviceExtension;

    PAGED_CODE();
    ASSERT(pContext);
    pContext->ulTicks = 0;
    pContext->timerInterval = TimerInterval;

    pContext->bStoppingDPC = FALSE;
    
    KeQuerySystemTime (&pContext->startTime);
    KdPrint(("StartDPC: %s %x-%x", __TIME__, 
            pContext->startTime.HighPart, 
            pContext->startTime.LowPart));

    // 开始计时
    NextTime.QuadPart = pContext->startTime.QuadPart + TimerInterval;
    KeSetTimer (&pContext->Timer, NextTime, &pContext->TimerDpc);
}

void StopDPC( IN PDEVICE_OBJECT Device )
{
    PDEVICE_EXTENSION pContext = (PDEVICE_EXTENSION)Device->DeviceExtension;

    PAGED_CODE();
    ASSERT(pContext);
    
	if(FALSE == pContext->bStoppingDPC)
	{
		pContext->bStoppingDPC = TRUE;
		
	    KeWaitForSingleObject (
	        &pContext->StopDPCEvent,
	        Executive,
	        KernelMode,
	        FALSE,
	        NULL);
    }
}

// 启动内核ASIO数据流
NTSTATUS AsioStart(DEVICE_OBJECT* Device)
{
	LONGLONG interval;
    PDEVICE_EXTENSION   deviceExtension;
    NTSTATUS    status = STATUS_INSUFFICIENT_RESOURCES;    

    KDBG(DPFLTR_INFO_LEVEL, "AsioSetEvent");
    deviceExtension = Device->DeviceExtension;
    
    // 求得固定长度采样数据的处理时间
    interval = deviceExtension->ulBufSize;
    interval *= 10000000;// How much 100ns in 1 second
    interval /= deviceExtension->ulSampleRate;
    
    StartDPC(Device, interval);
        
    return STATUS_SUCCESS;
}

// 停止内核数据流
NTSTATUS AsioStop(DEVICE_OBJECT* Device)
{
    PDEVICE_EXTENSION   deviceExtension;
    NTSTATUS    status = STATUS_SUCCESS;

    KDBG(DPFLTR_INFO_LEVEL, "AsioStop");

    deviceExtension = Device->DeviceExtension;
    deviceExtension->bStart = FALSE;
    StopDPC(Device);

    return status;
}

// 用户驱动通知数据准备完毕
NTSTATUS AsioBufReady(DEVICE_OBJECT* Device, ULONG ulBufIndex)
{
    int i;
    PDEVICE_EXTENSION   deviceExtension;
    deviceExtension = Device->DeviceExtension;

    for(i = 0; i < ASIO_MAX_CHANNELS; i++)
    {
        if(deviceExtension->ppInputBufs[i][ulBufIndex] && 
            deviceExtension->ppOutputBufs[i][ulBufIndex])
        {
            RtlCopyMemory(deviceExtension->ppInputBufs[i][ulBufIndex], 
                deviceExtension->ppOutputBufs[i][ulBufIndex],
                deviceExtension->ulBufSize*sizeof(ULONG));
        }
    }

    return STATUS_SUCCESS;
}

// 设置同步事件
NTSTATUS AsioSetEvent(DEVICE_OBJECT* Device, HANDLE* hEvent)
{
    PDEVICE_EXTENSION   deviceExtension;
    NTSTATUS    status = STATUS_UNSUCCESSFUL;

    KDBG(DPFLTR_INFO_LEVEL, "AsioSetEvent");

    deviceExtension = Device->DeviceExtension;

    if(hEvent == NULL || hEvent[0] == NULL || hEvent[1] == NULL){
        KDBG(DPFLTR_ERROR_LEVEL, "hEvent parameters are invalid.");
        return status;
    }

    if(deviceExtension->hEvent[0] || deviceExtension->hEvent[1]){
        KDBG(DPFLTR_ERROR_LEVEL, "hEvent parameters already exist.");
        return status;
    }

    __try{
        if(!NT_SUCCESS(ObReferenceObjectByHandle(hEvent[0], 
            EVENT_MODIFY_STATE, 
            *ExEventObjectType,
            KernelMode, 
            (PVOID*) &deviceExtension->hEvent[0], 
            NULL)))
        {
            KDBG(DPFLTR_ERROR_LEVEL, "ObReferenceObjectByHandle 1 failed");
            __leave;
        }


        if(!NT_SUCCESS(ObReferenceObjectByHandle(hEvent[1], 
            EVENT_MODIFY_STATE, 
            *ExEventObjectType,
            KernelMode, 
            (PVOID*) &deviceExtension->hEvent[1], 
            NULL)))
        {
            KDBG(DPFLTR_ERROR_LEVEL, "ObReferenceObjectByHandle 2 failed");
            __leave;
        }

        status = STATUS_SUCCESS;
    }
    __finally{

        if(!NT_SUCCESS(status))
        {
            if(deviceExtension->hEvent[0])ObDereferenceObject(deviceExtension->hEvent[0]);
            if(deviceExtension->hEvent[1])ObDereferenceObject(deviceExtension->hEvent[1]);
            deviceExtension->hEvent[0] = NULL;
            deviceExtension->hEvent[1] = NULL;

        }
    }

    return status;
}

// 释放同步事件
void AsioReleaseEvent(DEVICE_OBJECT* Device)
{
    PDEVICE_EXTENSION   deviceExtension;

    KDBG(DPFLTR_INFO_LEVEL, "AsioReleaseEvent");

    deviceExtension = Device->DeviceExtension;

    if(deviceExtension->hEvent[0])ObDereferenceObject(deviceExtension->hEvent[0]);
    if(deviceExtension->hEvent[1])ObDereferenceObject(deviceExtension->hEvent[1]);
    deviceExtension->hEvent[0] = NULL;
    deviceExtension->hEvent[1] = NULL;
}

// 设置同步数据缓冲区
NTSTATUS AsioSetBuffers(DEVICE_OBJECT* Device, BUFPOINTERSTRUCT *Buffers)
{
    int nChIndex = 0;
    int nBufIndex = 0;
    PVOID pBuf = NULL;
    ULONG ulInputBufLen = 0;
    NTSTATUS    status = STATUS_INVALID_PARAMETER;
    PDEVICE_EXTENSION   deviceExtension;

    KDBG(DPFLTR_INFO_LEVEL, "AsioSetBuffers");

    if(NULL == Buffers || NULL == Buffers->InChannelBuffer || NULL == Buffers->OutChannelBuffer)
        return status;

    deviceExtension = Device->DeviceExtension;
    deviceExtension->ulBufSize = Buffers->BufferLength/sizeof(ULONG);

    for (nChIndex = 0; nChIndex < ASIO_MAX_CHANNELS; nChIndex++)
    {
        for (nBufIndex = 0; nBufIndex < ASIO_MAX_BUFFERS; nBufIndex++)
        {
            if(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex] != NULL) 
            {
                KDBG(DPFLTR_INFO_LEVEL, "ppInputBufs_MDL[%d][%d]", nChIndex, nBufIndex);
                return status;
            }

            if(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex] != NULL)
            {
                KDBG(DPFLTR_INFO_LEVEL, "ppOutputBufs_MDL[%d][%d]", nChIndex, nBufIndex);
                return status;
            }
        }
    }

    __try
    {
        status = STATUS_INSUFFICIENT_RESOURCES;

        for (nChIndex = 0; nChIndex < ASIO_MAX_CHANNELS; nChIndex++)
        {
            for (nBufIndex = 0; nBufIndex < ASIO_MAX_BUFFERS; nBufIndex++)
            {
                pBuf = Buffers->InChannelBuffer[nChIndex][nBufIndex];

                if(NULL != pBuf)
                {
                    deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex] = 
                        IoAllocateMdl(pBuf, Buffers->BufferLength, FALSE, FALSE, 0);

                    if(NULL == deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex])
                        __leave;

                    // 内存处理，需要SEH保护
                    __try
                    {
                        // 锁定并获取虚拟内存
                        MmProbeAndLockPages(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex], 
                            KernelMode, 
                            IoWriteAccess);

                        deviceExtension->ppInputBufs[nChIndex][nBufIndex] = 
                            (PUCHAR)MmGetSystemAddressForMdlSafe(
                            deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex], 
                            NormalPagePriority);
                    }
                    __except(EXCEPTION_EXECUTE_HANDLER)
                    {
                        // 转换失败；置空。
                        IoFreeMdl(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex]);
                        deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex] = NULL;
                        deviceExtension->ppInputBufs[nChIndex][nBufIndex] = 0;
                    }
                }
                else
                {
                    deviceExtension->ppInputBufs[nChIndex][nBufIndex] = NULL;
                }

                pBuf = Buffers->OutChannelBuffer[nChIndex][nBufIndex];

                if(NULL != pBuf)
                {
                    deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex] = 
                        IoAllocateMdl(pBuf, Buffers->BufferLength, FALSE, FALSE, 0);

                    if(NULL == deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex])
                        __leave;

                    // 内存处理，需要SEH保护
                    __try
                    {
                        // 锁定并获取虚拟内存
                        MmProbeAndLockPages(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex],
                            KernelMode, 
                            IoWriteAccess);

                        deviceExtension->ppOutputBufs[nChIndex][nBufIndex] = 
                            (PUCHAR)MmGetSystemAddressForMdlSafe(
                            deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex],
                            NormalPagePriority);
                    }
                    __except(EXCEPTION_EXECUTE_HANDLER)
                    {
                        // 转换失败；置空。
                        IoFreeMdl(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex]);
                        deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex] = NULL;
                        deviceExtension->ppOutputBufs[nChIndex][nBufIndex] = 0;
                    }
                }
                else
                {
                    deviceExtension->ppOutputBufs[nChIndex][nBufIndex] = NULL;
                }
            }
        }
        status = STATUS_SUCCESS;
    }
    __finally
    {
        if(!NT_SUCCESS(status))
        {
            KDBG(DPFLTR_INFO_LEVEL, "status = 0x%X", status);

            for (nChIndex = 0; nChIndex < ASIO_MAX_CHANNELS; nChIndex++)
            {
                for (nBufIndex = 0; nBufIndex < ASIO_MAX_BUFFERS; nBufIndex++)
                {
                    if(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex])
                    {
                        MmUnlockPages(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex]);
                        IoFreeMdl(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex]);
                    }
                    deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex] = NULL;
                    deviceExtension->ppInputBufs[nChIndex][nBufIndex] = 0;

                    if(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex])
                    {
                        MmUnlockPages(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex]);
                        IoFreeMdl(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex]);
                    }
                    deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex] = NULL;
                    deviceExtension->ppOutputBufs[nChIndex][nBufIndex] = 0;
                }
            }
        }
    }

    return status;
}

// 释放同步缓冲区
void AsioReleaseBuffers(DEVICE_OBJECT* Device)
{
    int nChIndex = 0;
    int nBufIndex = 0;
    PDEVICE_EXTENSION   deviceExtension;

    KDBG(DPFLTR_INFO_LEVEL, "AsioReleaseBuffers");

    deviceExtension = Device->DeviceExtension;

    for(nChIndex = 0; nChIndex < ASIO_MAX_CHANNELS; nChIndex++)
    {
        for (nBufIndex = 0; nBufIndex < ASIO_MAX_BUFFERS; nBufIndex++)
        {
            if(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex])
            {
                MmUnlockPages(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex]);
                IoFreeMdl(deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex]);
            }

            deviceExtension->ppInputBufs_MDL[nChIndex][nBufIndex] = NULL;
            deviceExtension->ppInputBufs[nChIndex][nBufIndex] = 0;

            if(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex])
            {
                MmUnlockPages(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex]);
                IoFreeMdl(deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex]);
            }

            deviceExtension->ppOutputBufs_MDL[nChIndex][nBufIndex] = NULL;
            deviceExtension->ppOutputBufs[nChIndex][nBufIndex] = 0;
        }
    }
}