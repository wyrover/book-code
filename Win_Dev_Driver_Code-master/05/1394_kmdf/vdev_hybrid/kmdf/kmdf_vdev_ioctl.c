/*++

Copyright (c) Microsoft Corporation

Module Name:

    kmdf_vdev_ioctl.c

Abstract

--*/

#include "kmdf_vdev.h"

VOID
kmdf1394_EvtIoDeviceControl (
                             IN WDFQUEUE Queue,
                             IN WDFREQUEST Request,
                             IN size_t OutputBufferLength,
                             IN size_t InputBufferLength,
                             IN ULONG IoControlCode)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   deviceExtension;
    PVOID ioBuffer = NULL;
    WDFDEVICE device;
    size_t bufLength;

    //
    // For this sample we are going to use the UMDF driver
    // as are gatekeeper to protect against all the invalid I/O buffer size 
    // conditions.
    //
    UNREFERENCED_PARAMETER (InputBufferLength);
    UNREFERENCED_PARAMETER (OutputBufferLength);

    ENTER ("kmdf1394_EvtIoDeviceControl");
    TRACE (TL_TRACE, ("Request = 0x%p\n", Request));

    device = WdfIoQueueGetDevice (Queue);
    deviceExtension = GetDeviceContext (device);

    //
    // Since all the IOCTLs handled here are buffered, 
    // WdfRequestRetrieveOutputBuffer & WdfRequestRetrieveInputBuffer 
    // return the same buffer pointer.
    // So make sure you read all the information you need from
    // the buffer before you write to it. 
    //
    ntStatus = WdfRequestRetrieveInputBuffer (Request, 0, &ioBuffer, &bufLength);
    if( !NT_SUCCESS (ntStatus) || NULL == ioBuffer) 
    {
        TRACE(TL_ERROR, ("WdfRequestRetrieveInputBuffer failed 0x%x\n", ntStatus));
        WdfRequestComplete(Request, ntStatus);
        return;
    }


    switch (IoControlCode) 
    {
    case IOCTL_ALLOCATE_ADDRESS_RANGE:
        {
            PALLOCATE_ADDRESS_RANGE AllocateAddrRange;

            TRACE (TL_TRACE, ("IOCTL_ALLOCATE_ADDRESS_RANGE\n"));

            AllocateAddrRange = (PALLOCATE_ADDRESS_RANGE)ioBuffer;

            ntStatus = kmdf1394_AllocateAddressRange (
                device,
                Request,
                AllocateAddrRange);
            if (NT_SUCCESS (ntStatus)) 
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_ALLOCATE_ADDRESS_RANGE

    case IOCTL_FREE_ADDRESS_RANGE:
        {
            TRACE(TL_TRACE, ("IOCTL_FREE_ADDRESS_RANGE\n"));

            ntStatus = kmdf1394_FreeAddressRange ( 
                device,
                Request,
                *(PHANDLE)ioBuffer);
        }
        break; // IOCTL_FREE_ADDRESS_RANGE

    case IOCTL_ASYNC_READ:
        {
            PASYNC_READ     AsyncRead;

            TRACE (TL_TRACE, ("IOCTL_ASYNC_READ\n"));

            AsyncRead = (PASYNC_READ) ioBuffer;

            ntStatus = kmdf1394_AsyncRead (  
                device,
                Request,
                AsyncRead);

            if (NT_SUCCESS (ntStatus)) 
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_ASYNC_READ

    case IOCTL_ASYNC_WRITE:
        {
            PASYNC_WRITE    AsyncWrite;

            TRACE(TL_TRACE, ("IOCTL_ASYNC_WRITE\n"));
            AsyncWrite = (PASYNC_WRITE)ioBuffer;


            ntStatus = kmdf1394_AsyncWrite ( 
                device,
                Request,
                AsyncWrite);
        }
        break; // IOCTL_ASYNC_WRITE

    case IOCTL_ASYNC_LOCK:
        {
            PASYNC_LOCK     AsyncLock;

            TRACE(TL_TRACE, ("IOCTL_ASYNC_LOCK\n"));

            AsyncLock = (PASYNC_LOCK)ioBuffer;

            ntStatus = kmdf1394_AsyncLock ( 
                device,
                Request,
                AsyncLock);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_ASYNC_LOCK

    case IOCTL_ASYNC_STREAM:
        {

            PASYNC_STREAM AsyncStream;

            TRACE(TL_TRACE, ("IOCTL_ASYNC_STREAM\n"));

            AsyncStream = (PASYNC_STREAM) ioBuffer;

            ntStatus = kmdf1394_AsyncStream ( 
                device,
                Request,
                AsyncStream);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_ASYNC_STREAM

    case IOCTL_ISOCH_ALLOCATE_BANDWIDTH:
        {
            PISOCH_ALLOCATE_BANDWIDTH   IsochAllocateBandwidth;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_ALLOCATE_BANDWIDTH\n"));

            IsochAllocateBandwidth = (PISOCH_ALLOCATE_BANDWIDTH) ioBuffer;

            ntStatus = kmdf1394_IsochAllocateBandwidth ( 
                device,
                Request,
                IsochAllocateBandwidth);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_ISOCH_ALLOCATE_BANDWIDTH

    case IOCTL_ISOCH_ALLOCATE_CHANNEL:
        {
            PISOCH_ALLOCATE_CHANNEL     IsochAllocateChannel;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_ALLOCATE_CHANNEL\n"));

            IsochAllocateChannel = (PISOCH_ALLOCATE_CHANNEL)ioBuffer;

            ntStatus = kmdf1394_IsochAllocateChannel ( 
                device,
                Request,
                IsochAllocateChannel);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }               
        }
        break; // IOCTL_ISOCH_ALLOCATE_CHANNEL

    case IOCTL_ISOCH_ALLOCATE_RESOURCES:
        {
            PISOCH_ALLOCATE_RESOURCES   IsochAllocateResources;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_ALLOCATE_RESOURCES\n"));

            IsochAllocateResources = (PISOCH_ALLOCATE_RESOURCES)ioBuffer;

            ntStatus = kmdf1394_IsochAllocateResources (
                device,
                Request,
                IsochAllocateResources);
            if (NT_SUCCESS (ntStatus)) 
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }               
        }
        break; // IOCTL_ISOCH_ALLOCATE_RESOURCES

    case IOCTL_ISOCH_ATTACH_BUFFERS:
        {

            TRACE(TL_TRACE, ("IOCTL_ISOCH_ATTACH_BUFFERS - Not implemented\n"));
            //
            // TODO: There are tons of race conditions in the routines that
            // use the AttachBuffer request. There is no synchronization between
            // TimerDpc to cancel the attach buffer request and completion routines. They
            // all stomp on each others memory, free buffers currently being 
            // used by another routine, Detach routine is using IRP (newIrp) 
            // that is used by attach buffer routine.
            //
            // Removing code related to this because the 1394 team is rewriting
            // all these routines.
            //
            ntStatus = STATUS_NOT_IMPLEMENTED;
        }
        break; // IOCTL_ISOCH_ATTACH_BUFFERS

    case IOCTL_ISOCH_DETACH_BUFFERS:
        {
            TRACE(TL_TRACE, ("IOCTL_ISOCH_DETACH_BUFFERS - Not implemented\n"));
            // 
            // TODO: This call is unsupported as we cannot perform the 
            // attach buffers due to the above conditions.
            //
            ntStatus = STATUS_NOT_IMPLEMENTED;
        }
        break; // IOCTL_ISOCH_DETACH_BUFFERS

    case IOCTL_ISOCH_FREE_BANDWIDTH:
        {
            TRACE(TL_TRACE, ("IOCTL_ISOCH_FREE_BANDWIDTH\n"));

            ntStatus = kmdf1394_IsochFreeBandwidth (
                device,
                Request,
                *(PHANDLE)ioBuffer);

        }
        break; // IOCTL_ISOCH_FREE_BANDWIDTH

    case IOCTL_ISOCH_FREE_CHANNEL:
        {
            TRACE(TL_TRACE, ("IOCTL_ISOCH_FREE_CHANNEL\n"));

            ntStatus = kmdf1394_IsochFreeChannel ( 
                device,
                Request,
                *(PULONG)ioBuffer);

        }
        break; // IOCTL_ISOCH_FREE_CHANNEL

    case IOCTL_ISOCH_FREE_RESOURCES:
        {
            TRACE(TL_TRACE, ("IOCTL_ISOCH_FREE_RESOURCES\n"));

            ntStatus = kmdf1394_IsochFreeResources ( 
                device,
                Request,
                *(PHANDLE)ioBuffer);

        }
        break; // IOCTL_ISOCH_FREE_RESOURCES

    case IOCTL_ISOCH_LISTEN:
        {
            PISOCH_LISTEN       IsochListen;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_LISTEN\n"));

            IsochListen = (PISOCH_LISTEN)ioBuffer;

            ntStatus = kmdf1394_IsochListen ( 
                device,
                Request,
                IsochListen);

        }
        break; // IOCTL_ISOCH_LISTEN

    case IOCTL_ISOCH_QUERY_CURRENT_CYCLE_TIME:
        {
            TRACE(TL_TRACE, ("IOCTL_ISOCH_QUERY_CURRENT_CYCLE_TIME\n"));

            ntStatus = kmdf1394_IsochQueryCurrentCycleTime ( 
                device,
                Request,
                (PCYCLE_TIME)ioBuffer);

            if (NT_SUCCESS (ntStatus)) 
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_ISOCH_QUERY_CURRENT_CYCLE_TIME

    case IOCTL_ISOCH_QUERY_RESOURCES:
        {
            PISOCH_QUERY_RESOURCES      IsochQueryResources;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_QUERY_RESOURCES\n"));

            IsochQueryResources = (PISOCH_QUERY_RESOURCES)ioBuffer;

            ntStatus = kmdf1394_IsochQueryResources ( 
                device,
                Request,
                IsochQueryResources);

            if (NT_SUCCESS (ntStatus)) 
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_ISOCH_QUERY_RESOURCES

    case IOCTL_ISOCH_SET_CHANNEL_BANDWIDTH:
        {
            PISOCH_SET_CHANNEL_BANDWIDTH    IsochSetChannelBandwidth;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_SET_CHANNEL_BANDWIDTH\n"));

            IsochSetChannelBandwidth = (PISOCH_SET_CHANNEL_BANDWIDTH) ioBuffer;

            ntStatus = kmdf1394_IsochSetChannelBandwidth ( 
                device,
                Request,
                IsochSetChannelBandwidth);
        }
        break; // IOCTL_ISOCH_SET_CHANNEL_BANDWIDTH


    case IOCTL_ISOCH_MODIFY_STREAM_PROPERTIES:
        {
            PISOCH_MODIFY_STREAM_PROPERTIES     IsochModifyStreamProperties;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_MODIFY_STREAM_PROPERTIES\n"));

            IsochModifyStreamProperties = \
                (PISOCH_MODIFY_STREAM_PROPERTIES) ioBuffer;

            ntStatus = kmdf1394_IsochModifyStreamProperties ( 
                device,
                Request,
                IsochModifyStreamProperties);
        }
        break; // IOCTL_ISOCH_MODIFY_STREAM_PROPERTIES

    case IOCTL_ISOCH_STOP:
        {
            PISOCH_STOP     IsochStop;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_STOP\n"));

            IsochStop = (PISOCH_STOP) ioBuffer;

            ntStatus = kmdf1394_IsochStop ( 
                device,
                Request,
                IsochStop);
        }
        break; // IOCTL_ISOCH_STOP

    case IOCTL_ISOCH_TALK:
        {
            PISOCH_TALK     isochTalk;

            TRACE(TL_TRACE, ("IOCTL_ISOCH_TALK\n"));

            isochTalk = (PISOCH_TALK)ioBuffer;

            ntStatus = kmdf1394_IsochTalk (
                device,
                Request,
                isochTalk);
        }
        break; // IOCTL_ISOCH_TALK

    case IOCTL_GET_LOCAL_HOST_INFORMATION:
        {
            PGET_LOCAL_HOST_INFORMATION     GetLocalHostInformation;

            TRACE(TL_TRACE, ("IOCTL_GET_LOCAL_HOST_INFORMATION\n"));

            GetLocalHostInformation = (PGET_LOCAL_HOST_INFORMATION) ioBuffer;

            ntStatus = kmdf1394_GetLocalHostInformation( 
                device,
                Request,
                GetLocalHostInformation);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_GET_LOCAL_HOST_INFORMATION

    case IOCTL_GET_1394_ADDRESS_FROM_DEVICE_OBJECT:
        {
            PGET_1394_ADDRESS   Get1394Address;

            TRACE(TL_TRACE, ("IOCTL_GET_1394_ADDRESS_FROM_DEVICE_OBJECT\n"));

            Get1394Address = (PGET_1394_ADDRESS)ioBuffer;

            ntStatus = kmdf1394_Get1394AddressFromDeviceObject (
                device,
                Request,
                Get1394Address);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_GET_1394_ADDRESS_FROM_DEVICE_OBJECT

    case IOCTL_CONTROL:
        {
            TRACE(TL_TRACE, ("IOCTL_CONTROL\n"));

            ntStatus = kmdf1394_Control( 
                device,
                Request);

        }
        break; // IOCTL_CONTROL

    case IOCTL_GET_MAX_SPEED_BETWEEN_DEVICES:
        {
            PGET_MAX_SPEED_BETWEEN_DEVICES  MaxSpeedBetweenDevices;

            MaxSpeedBetweenDevices = (PGET_MAX_SPEED_BETWEEN_DEVICES)ioBuffer;

            ntStatus = kmdf1394_GetMaxSpeedBetweenDevices ( 
                device,
                Request,
                MaxSpeedBetweenDevices);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_GET_MAX_SPEED_BETWEEN_DEVICES

    case IOCTL_SET_DEVICE_XMIT_PROPERTIES:
        {
            PDEVICE_XMIT_PROPERTIES     DeviceXmitProperties;

            TRACE(TL_TRACE, ("IOCTL_SET_DEVICE_XMIT_PROPERTIES\n"));

            DeviceXmitProperties = (PDEVICE_XMIT_PROPERTIES)ioBuffer;

            ntStatus = kmdf1394_SetDeviceXmitProperties ( 
                device,
                Request,
                DeviceXmitProperties);
        }
        break; // IOCTL_SET_DEVICE_XMIT_PROPERTIES

    case IOCTL_BUS_RESET:
        {
            TRACE(TL_TRACE, ("IOCTL_BUS_RESET\n"));

            ntStatus = kmdf1394_BusReset( 
                device,
                Request,
                (PULONG)ioBuffer);
        }
        break; // IOCTL_BUS_RESET

    case IOCTL_GET_GENERATION_COUNT:
        {
            TRACE(TL_TRACE, ("IOCTL_GET_GENERATION_COUNT\n"));

            ntStatus =kmdf1394_GetGenerationCount ( 
                device,
                Request,
                (PULONG) ioBuffer);

            if (NT_SUCCESS (ntStatus)) 
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }
        }
        break; // IOCTL_GET_GENERATION_COUNT

    case IOCTL_SEND_PHY_CONFIGURATION_PACKET:
        {
            PPHY_CONFIGURATION_PACKET PhyConfigPacket;

            TRACE(TL_TRACE, ("IOCTL_SEND_PHY_CONFIGURATION_PACKET\n"));

            PhyConfigPacket = (PPHY_CONFIGURATION_PACKET) ioBuffer;

            ntStatus = kmdf1394_SendPhyConfigurationPacket( 
                device,
                Request,
                PhyConfigPacket);

        }
        break; // IOCTL_SEND_PHY_CONFIGURATION_PACKET

    case IOCTL_BUS_RESET_NOTIFICATION:
        {
            TRACE(TL_TRACE, ("IOCTL_BUS_RESET_NOTIFICATION\n"));

            ntStatus = kmdf1394_BusResetNotification( 
                device,
                Request,
                *((PULONG)ioBuffer));
        }
        break; // IOCTL_BUS_RESET_NOTIFICATION

    case IOCTL_SET_LOCAL_HOST_INFORMATION:
        {
            PSET_LOCAL_HOST_INFORMATION     SetLocalHostInformation;

            TRACE(TL_TRACE, ("IOCTL_SET_LOCAL_HOST_INFORMATION\n"));

            SetLocalHostInformation = (PSET_LOCAL_HOST_INFORMATION)ioBuffer;

            ntStatus = kmdf1394_SetLocalHostProperties( 
                device,
                Request,
                SetLocalHostInformation);

            if (NT_SUCCESS (ntStatus))
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            }

        }
        break; // IOCTL_SET_LOCAL_HOST_INFORMATION

    case IOCTL_SET_ADDRESS_DATA:
        {
            PSET_ADDRESS_DATA   SetAddressData;

            TRACE(TL_TRACE, ("IOCTL_SET_ADDRESS_DATA\n"));
            SetAddressData = (PSET_ADDRESS_DATA)ioBuffer;

            ntStatus = kmdf1394_SetAddressData( 
                device,
                Request,
                SetAddressData);
        }
        break; // IOCTL_SET_ADDRESS_DATA

    case IOCTL_GET_ADDRESS_DATA:
        {
            PGET_ADDRESS_DATA   GetAddressData;

            TRACE(TL_TRACE, ("IOCTL_GET_ADDRESS_DATA\n"));

            GetAddressData = (PGET_ADDRESS_DATA)ioBuffer;

            ntStatus = kmdf1394_GetAddressData( 
                device,
                Request,
                GetAddressData);

            if (NT_SUCCESS (ntStatus)) 
            {
                WdfRequestSetInformation (Request, OutputBufferLength);
            } 
        }
        break; // IOCTL_GET_ADDRESS_DATA

    case IOCTL_BUS_RESET_NOTIFY:

        TRACE(TL_TRACE, ("IOCTL_BUS_RESET_NOTIFY\n"));
        //
        // These requests will be held in the queue in a cancelable state.
        //
        ntStatus = WdfRequestForwardToIoQueue ( 
            Request,
            deviceExtension->BusResetRequestsQueue);

        if (NT_SUCCESS (ntStatus)) 
        {
            //
            // The Request is now owned by the Framework until it delivers
            // the Request from the new queue to the driver. This means that
            // the Request should not be accessed any further in this routine.
            //
            ntStatus = STATUS_PENDING;
        }

        break; // IOCTL_BUS_RESET_NOTIFY

    default:
        TRACE (TL_ERROR, ("Invalid ioControlCode = 0x%x\n", IoControlCode));
        ntStatus = STATUS_INVALID_PARAMETER;
        break; // default

    } // switch

    // only complete if the device is there
    if (ntStatus != STATUS_PENDING)
    {
        WdfRequestComplete (Request, ntStatus);
    }

    EXIT("kmdf1394_EvtIoDeviceControl", ntStatus);
    return ;
} // kmdf1394_EvtIoDeviceControl




