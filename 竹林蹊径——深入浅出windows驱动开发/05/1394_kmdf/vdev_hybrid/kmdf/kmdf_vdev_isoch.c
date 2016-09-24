/*++

Copyright (c) Microsoft Corporation

Module Name:

    kmdf_vdev_isoch.c

Abstract

--*/

#include "kmdf_vdev.h"


NTSTATUS
kmdf1394_IsochAllocateBandwidth (
                              IN WDFDEVICE Device,
                              IN WDFREQUEST Request,
                              IN OUT PISOCH_ALLOCATE_BANDWIDTH IsochAllocateBandwidth)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER ("kmdf1394_IsochAllocateBandwidth");

    TRACE (TL_TRACE, ("nMaxBytesPerFrameRequested = 0x%x\n", 
                    IsochAllocateBandwidth->nMaxBytesPerFrameRequested));
    TRACE (TL_TRACE, ("fulSpeed = 0x%x\n", IsochAllocateBandwidth->fulSpeed));

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_ALLOCATE_BANDWIDTH;
    pIrb->Flags = 0;
    pIrb->u.IsochAllocateBandwidth.nMaxBytesPerFrameRequested = \
        IsochAllocateBandwidth->nMaxBytesPerFrameRequested;
    pIrb->u.IsochAllocateBandwidth.fulSpeed = IsochAllocateBandwidth->fulSpeed;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS (ntStatus)) 
    {
        IsochAllocateBandwidth->hBandwidth = \
            pIrb->u.IsochAllocateBandwidth.hBandwidth;
        IsochAllocateBandwidth->BytesPerFrameAvailable= \
            pIrb->u.IsochAllocateBandwidth.BytesPerFrameAvailable;
        IsochAllocateBandwidth->SpeedSelected = \
            pIrb->u.IsochAllocateBandwidth.SpeedSelected;

        TRACE (
            TL_TRACE, 
            ("hBandwidth = 0x%x\n", 
            IsochAllocateBandwidth->hBandwidth));

        TRACE (
            TL_TRACE, 
            ("BytesPerFrameAvailable = 0x%x\n", 
            IsochAllocateBandwidth->BytesPerFrameAvailable));

        //
        // lets see if we got the speed we wanted
        //
        if (IsochAllocateBandwidth->fulSpeed != \
            pIrb->u.IsochAllocateBandwidth.SpeedSelected) 
        {
            TRACE (TL_TRACE, ("Different bandwidth speed selected.\n"));
        }

        TRACE (
            TL_TRACE, 
            ("SpeedSelected = 0x%x\n", 
            IsochAllocateBandwidth->SpeedSelected));
    }
    else 
    {
        TRACE (TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_IsochAllocateBandwidth", ntStatus);
    return ntStatus;
} // kmdf1394_IsochAllocateBandwidth

NTSTATUS
kmdf1394_IsochAllocateChannel (
                              IN WDFDEVICE Device,
                              IN WDFREQUEST Request,
                              IN OUT PISOCH_ALLOCATE_CHANNEL IsochAllocateChannel)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PIRB  pIrb = NULL;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);

    ENTER ("kmdf1394_IsochAllocateChannel");

    TRACE (
        TL_TRACE, 
        ("nRequestedChannel = 0x%x\n", 
        IsochAllocateChannel->nRequestedChannel));

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb)
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_ALLOCATE_CHANNEL;
    pIrb->Flags = 0;
    pIrb->u.IsochAllocateChannel.nRequestedChannel = \
        IsochAllocateChannel->nRequestedChannel;
    
    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS (ntStatus))
    {
        IsochAllocateChannel->Channel = pIrb->u.IsochAllocateChannel.Channel;
        IsochAllocateChannel->ChannelsAvailable = \
            pIrb->u.IsochAllocateChannel.ChannelsAvailable;

        TRACE(TL_TRACE, ("Channel = 0x%x\n", IsochAllocateChannel->Channel));
        TRACE(TL_TRACE, ("ChannelsAvailable.LowPart = 0x%x\n", 
                        IsochAllocateChannel->ChannelsAvailable.LowPart));
        TRACE(TL_TRACE, ("ChannelsAvailable.HighPart = 0x%x\n", 
                        IsochAllocateChannel->ChannelsAvailable.HighPart));
    }
    else 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT ("kmdf1394_IsochAllocateChannel", ntStatus);
    return ntStatus;
} // kmdf1394_IsochAllocateChannel

NTSTATUS
kmdf1394_IsochAllocateResources (
                                IN WDFDEVICE Device,
                                IN WDFREQUEST Request,
                                IN OUT PISOCH_ALLOCATE_RESOURCES IsochAllocateResources)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PIRB pIrb = NULL;
    PDEVICE_EXTENSION   deviceExtension = GetDeviceContext(Device);
    
    ENTER ("kmdf1394_IsochAllocateResources");

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;  
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_ALLOCATE_RESOURCES;
    pIrb->Flags = 0;
    pIrb->u.IsochAllocateResources.fulSpeed = IsochAllocateResources->fulSpeed;
    pIrb->u.IsochAllocateResources.fulFlags = IsochAllocateResources->fulFlags;
    pIrb->u.IsochAllocateResources.nChannel = IsochAllocateResources->nChannel;
    pIrb->u.IsochAllocateResources.nMaxBytesPerFrame = \
        IsochAllocateResources->nMaxBytesPerFrame;
    pIrb->u.IsochAllocateResources.nNumberOfBuffers = \
        IsochAllocateResources->nNumberOfBuffers;
    pIrb->u.IsochAllocateResources.nMaxBufferSize = \
        IsochAllocateResources->nMaxBufferSize;
    pIrb->u.IsochAllocateResources.nQuadletsToStrip = \
        IsochAllocateResources->nQuadletsToStrip;
    
    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS (ntStatus)) 
    {
        PISOCH_RESOURCE_DATA  IsochResourceData;

        IsochAllocateResources->hResource = \
            pIrb->u.IsochAllocateResources.hResource;

        TRACE(TL_TRACE, ("hResource = 0x%x\n", IsochAllocateResources->hResource));

        //
        // need to add to our list...
        //
        IsochResourceData = ExAllocatePoolWithTag (
            NonPagedPool,
            sizeof(ISOCH_RESOURCE_DATA),
            POOLTAG_KMDF_VDEV);
        if (IsochResourceData) 
        {
            IsochResourceData->hResource = \
                pIrb->u.IsochAllocateResources.hResource;

            WdfSpinLockAcquire (deviceExtension->IsochResourceSpinLock);

            InsertHeadList (&deviceExtension->IsochResourceData,
                           &IsochResourceData->IsochResourceList);

            WdfSpinLockRelease (deviceExtension->IsochResourceSpinLock);
        }
        else
        {
            TRACE (TL_WARNING, ("Failed to allocate IsochResourceData!\n"));
        }
    }
    else 
    {

        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_IsochAllocateResources", ntStatus);
    return(ntStatus);
} // kmdf1394_IsochAllocateResources

NTSTATUS
kmdf1394_IsochAttachBuffers (
                            IN WDFDEVICE Device,
                            IN WDFREQUEST Request,
                            IN PISOCH_ATTACH_BUFFERS IsochAttachBuffers)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;

    ENTER("kmdf1394_IsochAttachBuffers");
    UNREFERENCED_PARAMETER (Device);
    UNREFERENCED_PARAMETER (Request);
    UNREFERENCED_PARAMETER (IsochAttachBuffers);
    
    //
    // TODO: Put the appropriate code to attach buffers back in
    //

    EXIT("kmdf1394_IsochAttachBuffers", ntStatus);
    return ntStatus;
} // kmdf1394_IsochAttachBuffers

NTSTATUS
kmdf1394_IsochDetachBuffers(
                            IN WDFDEVICE Device,
                            IN WDFREQUEST Request,
                            IN PISOCH_DETACH_BUFFERS IsochDetachBuffers)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;

    ENTER ("kmdf1394_IsochDetachBuffers");

    UNREFERENCED_PARAMETER (Device);
    UNREFERENCED_PARAMETER (Request);
    UNREFERENCED_PARAMETER (IsochDetachBuffers);

    //
    // TODO: Put the appropriate code to detach buffers back in
    //

    EXIT ("kmdf1394_IsochDetachBuffers", ntStatus);
    return ntStatus;
} // kmdf1394_IsochDetachBuffers

NTSTATUS
kmdf1394_IsochFreeBandwidth (
                             IN WDFDEVICE Device,
                             IN WDFREQUEST Request,
                             IN HANDLE hBandwidth)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext (Device);
    PIRB pIrb = NULL;

    ENTER ("kmdf1394_IsochFreeBandwidth");

    TRACE (TL_TRACE, ("hBandwidth = 0x%x\n", hBandwidth));


    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_FREE_BANDWIDTH;
    pIrb->Flags = 0;
    pIrb->u.IsochFreeBandwidth.hBandwidth = hBandwidth;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS (ntStatus))
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_IsochFreeBandwidth", ntStatus);
    return ntStatus;
} // kmdf1394_IsochFreeBandwidth

NTSTATUS
kmdf1394_IsochFreeChannel (
                          IN WDFDEVICE Device,
                          IN WDFREQUEST Request,
                          IN ULONG nChannel)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext (Device);
    PIRB pIrb = NULL;

    ENTER ("kmdf1394_IsochFreeChannel");

    TRACE (TL_TRACE, ("nChannel = 0x%x\n", nChannel));

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_FREE_CHANNEL;
    pIrb->Flags = 0;
    pIrb->u.IsochFreeChannel.nChannel = nChannel;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS (ntStatus)) 
    {
        TRACE (TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT ("kmdf1394_IsochFreeChannel", ntStatus);
    return ntStatus;
} // kmdf1394_IsochFreeChannel

NTSTATUS
kmdf1394_IsochFreeResources (
                         IN WDFDEVICE Device,
                         IN WDFREQUEST Request,
                         IN HANDLE hResource)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext (Device);
    PIRB pIrb = NULL;
    PISOCH_RESOURCE_DATA  IsochResourceData   = NULL;
    PLIST_ENTRY listHead, thisEntry;

    ENTER ("kmdf1394_IsochFreeResources");

    TRACE (TL_TRACE, ("hResource = 0x%x\n", hResource));

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    //
    // remove this one from our list...
    //
    WdfSpinLockAcquire (deviceExtension->IsochResourceSpinLock);

    listHead = &deviceExtension->IsochResourceData;

    for (thisEntry = listHead->Flink;
        thisEntry != listHead;
        IsochResourceData = NULL, thisEntry = thisEntry->Flink)
    {
        IsochResourceData = CONTAINING_RECORD (
            thisEntry,
            ISOCH_RESOURCE_DATA,
            IsochResourceList);

        if (IsochResourceData->hResource == hResource) 
        {
            TRACE(TL_TRACE, ("Removing hResource = 0x%x\n", hResource));

            RemoveEntryList(&IsochResourceData->IsochResourceList);
            ExFreePoolWithTag(IsochResourceData, POOLTAG_KMDF_VDEV);
            break;
        }
    }

    WdfSpinLockRelease(deviceExtension->IsochResourceSpinLock);

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_FREE_RESOURCES;
    pIrb->Flags = 0;
    pIrb->u.IsochFreeResources.hResource = hResource;

    ntStatus = kmdf1394_SubmitIrpSynch (deviceExtension->StackIoTarget, Request, pIrb);
    if (!NT_SUCCESS (ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_IsochFreeResources", ntStatus);
    return ntStatus;
} // kmdf1394_IsochFreeResources

NTSTATUS
kmdf1394_IsochListen (
                  IN WDFDEVICE Device,
                  IN WDFREQUEST Request,
                  IN PISOCH_LISTEN IsochListen)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER ("kmdf1394_IsochListen");

   
    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_LISTEN;
    pIrb->Flags = 0;
    pIrb->u.IsochListen.hResource = IsochListen->hResource;
    pIrb->u.IsochListen.fulFlags = IsochListen->fulFlags;
    pIrb->u.IsochListen.StartTime = IsochListen->StartTime;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS (ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_IsochListen", ntStatus);
    return ntStatus;
} // kmdf1394_IsochListen

NTSTATUS
kmdf1394_IsochQueryCurrentCycleTime (
                                 IN WDFDEVICE Device,
                                 IN WDFREQUEST Request,
                                 OUT PCYCLE_TIME pCurrentCycleTime)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PIRB pIrb = NULL;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    
    ENTER ("kmdf1394_IsochQueryCurrentCycleTime");

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_QUERY_CYCLE_TIME;
    pIrb->Flags = 0;

    ntStatus = kmdf1394_SubmitIrpSynch(
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS (ntStatus)) 
    {
        *pCurrentCycleTime = pIrb->u.IsochQueryCurrentCycleTime.CycleTime;

        TRACE(
            TL_TRACE, 
            ("CurrentCycleTime.CL_CycleOffset = 0x%x\n",
            pCurrentCycleTime->CL_CycleOffset));
        
        TRACE(
            TL_TRACE, 
            ("CurrentCycleTime.CL_CycleCount = 0x%x\n",
            pCurrentCycleTime->CL_CycleCount));
        
        TRACE(
            TL_TRACE, 
            ("CurrentCycleTime.CL_SecondCount = 0x%x\n",
            pCurrentCycleTime->CL_SecondCount));
    }
    else 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }
 
    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_IsochQueryCurrentCycleTime", ntStatus);
    return(ntStatus);
} // kmdf1394_IsochQueryCurrentCycleTime

NTSTATUS
kmdf1394_IsochQueryResources (
                             IN WDFDEVICE Device,
                             IN WDFREQUEST Request,
                             IN OUT PISOCH_QUERY_RESOURCES IsochQueryResources)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext (Device);
    PIRB pIrb = NULL;

    ENTER ("kmdf1394_IsochQueryResources");

    TRACE (TL_TRACE, ("fulSpeed = 0x%x\n", IsochQueryResources->fulSpeed));

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_QUERY_RESOURCES;
    pIrb->Flags = 0;
    pIrb->u.IsochQueryResources.fulSpeed = IsochQueryResources->fulSpeed;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS(ntStatus)) 
    {

        IsochQueryResources->BytesPerFrameAvailable = \
            pIrb->u.IsochQueryResources.BytesPerFrameAvailable;
        IsochQueryResources->ChannelsAvailable = \
            pIrb->u.IsochQueryResources.ChannelsAvailable;

        TRACE (
            TL_TRACE, 
            ("BytesPerFrameAvailable = 0x%x\n",
            IsochQueryResources->BytesPerFrameAvailable));

        TRACE (
            TL_TRACE, 
            ("ChannelsAvailable.LowPart = 0x%x\n",
            IsochQueryResources->ChannelsAvailable.LowPart));

        TRACE (
            TL_TRACE, 
            ("ChannelsAvailable.HighPart = 0x%x\n",
            IsochQueryResources->ChannelsAvailable.HighPart));
    }
    else 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_IsochQueryResources", ntStatus);
    return(ntStatus);
} //kmdf1394_IsochQueryResources

NTSTATUS
kmdf1394_IsochSetChannelBandwidth (
                                  IN WDFDEVICE Device,
                                  IN WDFREQUEST Request,
                                  IN PISOCH_SET_CHANNEL_BANDWIDTH IsochSetChannelBandwidth)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PIRB pIrb = NULL;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    
    ENTER ("kmdf1394_IsochSetChannelBandwidth");

    TRACE (
        TL_TRACE, 
        ("hBandwidth = 0x%x\n", 
        IsochSetChannelBandwidth->hBandwidth));

    TRACE (
        TL_TRACE, 
        ("nMaxBytesPerFrame = 0x%x\n", 
        IsochSetChannelBandwidth->nMaxBytesPerFrame));

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;  
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_SET_CHANNEL_BANDWIDTH;
    pIrb->Flags = 0;
    pIrb->u.IsochSetChannelBandwidth.hBandwidth = \
        IsochSetChannelBandwidth->hBandwidth;
    pIrb->u.IsochSetChannelBandwidth.nMaxBytesPerFrame = \
        IsochSetChannelBandwidth->nMaxBytesPerFrame;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS(ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_IsochSetChannelBandwidth",  ntStatus);
    return(ntStatus);
} // kmdf1394_IsochSetChannelBandwidth


NTSTATUS
kmdf1394_IsochModifyStreamProperties (
                                     IN WDFDEVICE Device,
                                     IN WDFREQUEST Request,
                                     IN PISOCH_MODIFY_STREAM_PROPERTIES IsochModifyStreamProperties)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER ("kmdf1394_IsochModifyStreamProperties\n");

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb)
    {

        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_MODIFY_STREAM_PROPERTIES;
    pIrb->Flags = 0;
    pIrb->u.IsochModifyStreamProperties.hResource = \
        IsochModifyStreamProperties->hResource;
    pIrb->u.IsochModifyStreamProperties.ChannelMask = \
        IsochModifyStreamProperties->ChannelMask;
    pIrb->u.IsochModifyStreamProperties.fulSpeed = \
        IsochModifyStreamProperties->fulSpeed;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS (ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_IsochModifyStreamProperties", ntStatus);
    return ntStatus;
}

NTSTATUS
kmdf1394_IsochStop (
                   IN WDFDEVICE      Device,
                   IN WDFREQUEST  Request,
                   IN PISOCH_STOP  IsochStop)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PIRB pIrb = NULL;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);

    ENTER ("kmdf1394_IsochStop");

    TRACE (TL_TRACE, ("hResource = 0x%x\n", IsochStop->hResource));
    TRACE (TL_TRACE, ("fulFlags = 0x%x\n", IsochStop->fulFlags));

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_STOP;
    pIrb->Flags = 0;
    pIrb->u.IsochStop.hResource = IsochStop->hResource;
    pIrb->u.IsochStop.fulFlags = IsochStop->fulFlags;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS (ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT ("kmdf1394_IsochStop", ntStatus);
    return ntStatus;
} //kmdf1394_IsochStop


NTSTATUS
kmdf1394_IsochTalk (
                    IN WDFDEVICE        Device,
                    IN WDFREQUEST    Request,
                    IN PISOCH_TALK     IsochTalk)

{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION  deviceExtension = GetDeviceContext (Device);
    PIRB pIrb = NULL;

    ENTER ("kmdf1394_IsochTalk");

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_ISOCH_TALK;
    pIrb->Flags = 0;
    pIrb->u.IsochTalk.hResource = IsochTalk->hResource;
    pIrb->u.IsochTalk.fulFlags = IsochTalk->fulFlags;
    pIrb->u.IsochTalk.StartTime = IsochTalk->StartTime;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS(ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT ("kmdf1394_IsochTalk", ntStatus);
    return ntStatus;
} // kmdf1394_IsochTalk

void
kmdf1394_IsochCallback (
                    IN PDEVICE_EXTENSION    DeviceExtension,
                    IN PISOCH_DETACH_DATA   IsochDetachData)
{
    ENTER("kmdf1394_IsochCallback");

    if (!IsochDetachData)
    {
        goto Exit_IsochCallback;
    }

    //
    // make sure somebody else isn't already handling cleaning up for this request
    //
    WdfSpinLockAcquire (DeviceExtension->IsochSpinLock);
    
    if (kmdf1394_IsOnList (
        &IsochDetachData->IsochDetachList, 
        &DeviceExtension->IsochDetachData))
    {
        RemoveEntryList (&IsochDetachData->IsochDetachList);

        WdfSpinLockRelease (DeviceExtension->IsochSpinLock);
        KeCancelTimer (&IsochDetachData->Timer);

        TRACE (
            TL_TRACE, 
            ("IsochCallback: IsochDetachData = 0x%x\n", 
            IsochDetachData));

        TRACE (
            TL_TRACE, 
            ("IsochCallback: IsochDetachData->Request = 0x%x\n", 
            IsochDetachData->Request));

        TRACE (
            TL_TRACE, 
            ("IsochCallback: IsochDetachData->newIrp = 0x%x\n", 
            IsochDetachData->newIrp));

        //
        // need to save the status of the attach
        // we'll clean up in the same spot for success's and timeout's
        // TODO: IsochDetachData->AttachStatus = IsochDetachData->Irp->IoStatus.Status;
        //
        kmdf1394_IsochCleanup (IsochDetachData);
    }
    else
    {
        TRACE (
            TL_TRACE, 
            ("IsochCallback: Entry 0x%x not on List 0x%x\n",
            IsochDetachData->IsochDetachList, 
            DeviceExtension->IsochDetachData));

        WdfSpinLockRelease (DeviceExtension->IsochSpinLock);

    }

Exit_IsochCallback:
    EXIT("kmdf1394_IsochCallback", 0);
} // kmdf1394_IsochCallback

void
kmdf1394_IsochTimeout (
                   IN PKDPC Dpc,
                   IN PISOCH_DETACH_DATA IsochDetachData,
                   IN PVOID SystemArgument1,
                   IN PVOID SystemArgument2)
{
    PDEVICE_EXTENSION   DeviceExtension;

    UNREFERENCED_PARAMETER (Dpc);
    UNREFERENCED_PARAMETER (SystemArgument1);
    UNREFERENCED_PARAMETER (SystemArgument2);

    ENTER ("kmdf1394_IsochTimeout");
    TRACE (TL_WARNING, ("Isoch Timeout!\n"));

    //
    // ISSUE: the device extension we are referencing comes from the IsochDetachData
    // but it is possible this memory has been freed before we enter this function.
    // The only way to check is to validate against our DeviceExtension->IsochDetachList
    // but if the IsochDetachData has been freed then that won't be accessible
    //
    DeviceExtension = IsochDetachData->DeviceExtension;
    if (DeviceExtension)
    {
        //
        // make sure nobody else has already handled this request yet
        //
        WdfSpinLockAcquire (DeviceExtension->IsochSpinLock);
        if (kmdf1394_IsOnList (
            &IsochDetachData->IsochDetachList, 
            &DeviceExtension->IsochDetachData))
        {
            RemoveEntryList (&IsochDetachData->IsochDetachList);

            WdfSpinLockRelease (DeviceExtension->IsochSpinLock);

            if(KeCancelTimer (&IsochDetachData->Timer))
            {
                TRACE (
                    TL_TRACE, 
                    ("IsochTimeout: IsochDetachData = 0x%x\n", 
                    IsochDetachData));

                TRACE (
                    TL_TRACE, 
                    ("IsochTimeout: IsochDetachData->Irp = 0x%x\n", 
                    IsochDetachData->Request));

                TRACE (
                    TL_TRACE, 
                    ("IsochTimeout: IsochDetachData->newIrp = 0x%x\n", 
                    IsochDetachData->newIrp));

                // 
                // need to save the status of the attach
                // we'll clean up in the same spot for success's and timeout's
                //
                IsochDetachData->AttachStatus = STATUS_TIMEOUT;
                kmdf1394_IsochCleanup(IsochDetachData);
            }
        }
        else
        {
            WdfSpinLockRelease(DeviceExtension->IsochSpinLock);
        }
    }

    EXIT ("kmdf1394_IsochTimeout", 0);
} // kmdf1394_IsochTimeout


//
// TODO: These last functions are here as place holders for when the 
// attach / detach buffers routines are implemented.
//

void
kmdf1394_IsochCleanup (
                       IN PISOCH_DETACH_DATA   IsochDetachData)
{

    UNREFERENCED_PARAMETER(IsochDetachData);

    ENTER("kmdf1394_IsochCleanup");

    EXIT("kmdf1394_IsochCleanup", 0);
} //kmdf1394_IsochCleanup


NTSTATUS
kmdf1394_IsochDetachCompletionRoutine (
                                   IN PDEVICE_OBJECT Device,
                                   IN PIRP Irp,
                                   IN PISOCH_DETACH_DATA IsochDetachData)
{
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(Irp);
    UNREFERENCED_PARAMETER(IsochDetachData);

    ENTER("kmdf1394_IsochDetachCompletionRoutine");

    EXIT ("kmdf1394_IsochDetachCompletionRoutine", STATUS_MORE_PROCESSING_REQUIRED);
    return STATUS_MORE_PROCESSING_REQUIRED;
} // kmdf1394_IsochDetachCompletionRoutine



