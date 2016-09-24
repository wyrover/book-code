/*++

Copyright (c) Microsoft Corporation

Module Name:

    kmdf_vdev_api.c

Abstract

--*/

#include "kmdf_vdev.h"

NTSTATUS
kmdf1394_GetLocalHostInformation(
                                 IN WDFDEVICE  Device,
                                 IN WDFREQUEST Request,
                                 IN OUT PGET_LOCAL_HOST_INFORMATION GetLocalHostInfo)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION  deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    GET_LOCAL_HOST_INFO7    LocalHostInfo7  = {0};
    GET_LOCAL_HOST_INFO6    LocalHostInfo6  = {0};
    GET_LOCAL_HOST_INFO5    LocalHostInfo5  = {0};

    ENTER("kmdf1394_GetLocalHostInformation");

    TRACE(TL_TRACE, ("nLevel = 0x%x\n", GetLocalHostInfo->nLevel));
    TRACE(TL_TRACE, ("Information = 0x%x\n", GetLocalHostInfo->Information));

    pIrb = ExAllocatePoolWithTag (NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;   
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_GET_LOCAL_HOST_INFO;
    pIrb->Flags = 0;
    pIrb->u.GetLocalHostInformation.nLevel = GetLocalHostInfo->nLevel;

    switch (GetLocalHostInfo->nLevel)
    {
    case GET_HOST_DMA_CAPABILITIES:
        {

            //
            // need to ensure that no one passed us an GET_LOCAL_HOST_INFO7 
            // structure that just piggy backed on the 
            // GET_LOCAL_HOST_INFORMATION struct, if they did this will cause
            // data misalignment errors in 64 bit because 1394bus will be expecting that structure to
            // be aligned correctly
            //
            RtlCopyMemory (&LocalHostInfo7, GetLocalHostInfo->Information, 
                                    sizeof (GET_LOCAL_HOST_INFO7));

            pIrb->u.GetLocalHostInformation.Information = &LocalHostInfo7;

            TRACE(
                TL_TRACE, 
                ("HostDmaCapabilities = 0x%p\n", 
                LocalHostInfo7.HostDmaCapabilities));

            TRACE(TL_TRACE, ("MaxDmaBufferSize High = 0x%x\n", 
                                    LocalHostInfo7.MaxDmaBufferSize.HighPart));
            TRACE(TL_TRACE, ("MaxDmaBufferSize Low = 0x%x\n", 
                                    LocalHostInfo7.MaxDmaBufferSize.LowPart));

        }
        break;

    case GET_HOST_CSR_CONTENTS:
        {

            LocalHostInfo6.CsrBaseAddress.Off_High = \
                ((PGET_LOCAL_HOST_INFO6)GetLocalHostInfo->Information)->CsrBaseAddress.Off_High;

            LocalHostInfo6.CsrBaseAddress.Off_Low = \
                ((PGET_LOCAL_HOST_INFO6)GetLocalHostInfo->Information)->CsrBaseAddress.Off_Low;

            LocalHostInfo6.CsrDataLength = \
                ((PGET_LOCAL_HOST_INFO6)GetLocalHostInfo->Information)->CsrDataLength;

            //
            // Pointer math needed to correct size differential between 
            // data structures on WIN64 only LocalHostInfo6.CsrDataBuffer 
            // is UCHAR @ 4 bytes, Information->CsrDataBuffer is a PVIOD 
            // @ 8 bytes
            //
            LocalHostInfo6.CsrDataBuffer = \
                ((PCHAR)&(((PGET_LOCAL_HOST_INFO6)GetLocalHostInfo->Information)->CsrDataLength)) \
                + sizeof (ULONG);

            pIrb->u.GetLocalHostInformation.Information = &LocalHostInfo6;

            TRACE(TL_TRACE, ("CsrBaseAddress.Off_High = 0x%x\n", 
                                    LocalHostInfo6.CsrBaseAddress.Off_High));
            TRACE(TL_TRACE, ("CsrBaseAddress.Off_Low = 0x%x\n", 
                                    LocalHostInfo6.CsrBaseAddress.Off_Low));
            TRACE(TL_TRACE, ("CsrDataLength = 0x%x\n", LocalHostInfo6.CsrDataLength));
            TRACE(TL_TRACE, ("CsrDataBuffer = 0x%p\n", LocalHostInfo6.CsrDataBuffer));
        }
        break;

    case GET_HOST_CONFIG_ROM:
        {
            //
            // need to ensure that no one passed us an GET_LOCAL_HOST_INFO5 
            // structure that just piggy backed on the 
            // GET_LOCAL_HOST_INFORMATION struct, if they did this will cause
            // data misalignment errors in 64 bit because 1394bus will be expecting that structure to
            // be aligned correctly
            //
            RtlCopyMemory (&LocalHostInfo5, GetLocalHostInfo->Information, 
                                    sizeof (GET_LOCAL_HOST_INFO5));

            pIrb->u.GetLocalHostInformation.Information = &LocalHostInfo5;

            TRACE(TL_TRACE, ("ConfigRom = 0x%p\n", LocalHostInfo5.ConfigRom));
            TRACE(
                TL_TRACE, 
                ("ConfigRomLength = 0x%x\n", 
                LocalHostInfo5.ConfigRomLength));
        }
    break;

    default:
        {
            pIrb->u.GetLocalHostInformation.Information = \
                GetLocalHostInfo->Information;
        }

        break;
    }// switch

    ntStatus = kmdf1394_SubmitIrpSynch(
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS (ntStatus)) 
    {
        if (STATUS_INVALID_BUFFER_SIZE == ntStatus) 
        {
            //
            // we have to set status=success, otherwise, we don't get
            // the size value passed up to ring 3.
            //
            ntStatus = STATUS_SUCCESS;
            GetLocalHostInfo->Status = 122;

            TRACE(
                TL_WARNING, 
                ("Invalid Buffer Size. nLevel = 0x%x\n", 
                GetLocalHostInfo->nLevel));

            //
            // see if it's a config rom request or a csr request
            //
            if (GET_HOST_CONFIG_ROM == GetLocalHostInfo->nLevel) 
            {
                //
                // it is, lets print out the size
                //
                TRACE(
                    TL_TRACE, 
                    ("ConfigRomLength = 0x%x\n", 
                    LocalHostInfo5.ConfigRomLength));

                //
                // need to set this in the information struct we were passed down
                //
                RtlCopyMemory(
                    GetLocalHostInfo->Information, 
                    &LocalHostInfo5,
                    sizeof (GET_LOCAL_HOST_INFO5));
            }
            else if (GET_HOST_CSR_CONTENTS == GetLocalHostInfo->nLevel) 
            {
                //
                // csr, print out size
                //
                TRACE(
                    TL_TRACE, 
                    ("CsrDataLength needed = 0x%x\n", 
                    LocalHostInfo6.CsrDataLength));

                //
                // need to set this in the information struct we were passed down
                //
                ((PGET_LOCAL_HOST_INFO6)GetLocalHostInfo->Information)->CsrDataLength = \
                    LocalHostInfo6.CsrDataLength;
            }
            else if (GET_HOST_DMA_CAPABILITIES == GetLocalHostInfo->nLevel) 
            {
                //
                // copy the results of the struct passed down
                //
                RtlCopyMemory(
                    GetLocalHostInfo->Information, 
                    &LocalHostInfo7, 
                    sizeof (GET_LOCAL_HOST_INFO7));
            }
        } // if (STATUS_INVALID_BUFFER_SIZE == ntStatus)
        else 
        {
            TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
        }
    } // if (!NT_SUCCESS (ntStatus))
    else 
    {
        GetLocalHostInfo->Status = 0;

        if (GET_HOST_UNIQUE_ID == GetLocalHostInfo->nLevel) 
        {
            PGET_LOCAL_HOST_INFO1   LocalHostInfo1;

            LocalHostInfo1 = (PGET_LOCAL_HOST_INFO1)GetLocalHostInfo->Information;

            TRACE(TL_TRACE, ("UniqueId.LowPart = 0x%x\n",
                             LocalHostInfo1->UniqueId.LowPart));
            TRACE(TL_TRACE, ("UniqueId.HighPart = 0x%x\n",
                             LocalHostInfo1->UniqueId.HighPart));
        }
        else if (GET_HOST_CAPABILITIES == GetLocalHostInfo->nLevel) 
        {
            PGET_LOCAL_HOST_INFO2   LocalHostInfo2;

            LocalHostInfo2 = (PGET_LOCAL_HOST_INFO2)GetLocalHostInfo->Information;

            TRACE(TL_TRACE, ("HostCapabilities = 0x%x\n",
                             LocalHostInfo2->HostCapabilities));
            TRACE(TL_TRACE, ("MaxAsyncReadRequest = 0x%x\n",
                             LocalHostInfo2->MaxAsyncReadRequest));
            TRACE(TL_TRACE, ("MaxAsyncWriteRequest = 0x%x\n",
                             LocalHostInfo2->MaxAsyncWriteRequest));
        }
        else if (GET_POWER_SUPPLIED == GetLocalHostInfo->nLevel) 
        {
            PGET_LOCAL_HOST_INFO3   LocalHostInfo3;

            LocalHostInfo3 = (PGET_LOCAL_HOST_INFO3)GetLocalHostInfo->Information;

            TRACE(TL_TRACE, ("deciWattsSupplied = 0x%x\n",
                             LocalHostInfo3->deciWattsSupplied));
            TRACE(TL_TRACE, ("Voltage = 0x%x\n", LocalHostInfo3->Voltage));
        }
        else if (GET_PHYS_ADDR_ROUTINE == GetLocalHostInfo->nLevel)
        {
            PGET_LOCAL_HOST_INFO4   LocalHostInfo4;

            LocalHostInfo4 = (PGET_LOCAL_HOST_INFO4)GetLocalHostInfo->Information;

            TRACE(TL_TRACE, ("Context = 0x%x\n", LocalHostInfo4->Context));
        }
        else if (GET_HOST_CONFIG_ROM == GetLocalHostInfo->nLevel) 
        {
            PGET_LOCAL_HOST_INFO5   pLocalHostInfo5;

            pLocalHostInfo5 = \
                (PGET_LOCAL_HOST_INFO5)GetLocalHostInfo->Information;

            TRACE(
                TL_TRACE, 
                ("ConfigRomLength = 0x%x\n", 
                pLocalHostInfo5->ConfigRomLength));
        }
        else if (GET_HOST_DMA_CAPABILITIES == GetLocalHostInfo->nLevel) 
        {
            //
            // need to copy the results back into the original address space passed in
            //
            TRACE(TL_TRACE, ("HostDmaCapabilities = 0x%x\n",
                             LocalHostInfo7.HostDmaCapabilities));
            TRACE(TL_TRACE, ("MaxDmaBufferSize Low = 0x%x\n",
                             LocalHostInfo7.MaxDmaBufferSize.LowPart));
            TRACE(TL_TRACE, ("MaxDmaBufferSize High = 0x%x\n",
                             LocalHostInfo7.MaxDmaBufferSize.HighPart));

            RtlCopyMemory(GetLocalHostInfo->Information, &LocalHostInfo7, 
                                    sizeof (GET_LOCAL_HOST_INFO7));
        }
        else 
        {
            TRACE(
                TL_WARNING, 
                ("Returned success on invalid nLevel = 0x%x\n", 
                GetLocalHostInfo->nLevel));
        }
    }
    
    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_GetLocalHostInformation", ntStatus);
    return ntStatus;
} // kmdf1394_GetLocalHostInformation

NTSTATUS
kmdf1394_Get1394AddressFromDeviceObject(
                                        IN WDFDEVICE Device,
                                        IN WDFREQUEST Request,
                                        IN OUT PGET_1394_ADDRESS Get1394Address)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER("kmdf1394_Get1394AddressFromDeviceObject");

    TRACE(TL_TRACE, ("fulFlags = 0x%x\n", Get1394Address->fulFlags));

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_GET_ADDR_FROM_DEVICE_OBJECT;
    pIrb->Flags = 0;
    pIrb->u.Get1394AddressFromDeviceObject.fulFlags = Get1394Address->fulFlags;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS(ntStatus)) 
    {
        Get1394Address->NodeAddress = \
            pIrb->u.Get1394AddressFromDeviceObject.NodeAddress;
    }
    else 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_Get1394AddressFromDeviceObject", ntStatus);
    return ntStatus;
} // kmdf1394_Get1394AddressFromDeviceObject

NTSTATUS
kmdf1394_Control (
                 IN WDFDEVICE Device,
                 IN WDFREQUEST Request)
{
    NTSTATUS    ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION   deviceExtension = GetDeviceContext(Device);

    UNREFERENCED_PARAMETER(Request);
    UNREFERENCED_PARAMETER(deviceExtension);

    ENTER("kmdf1394_Control");

    //
    // TODO: We don't have a solid implementation for this 
    // request as yet.  It's a work list item for the 1394 team
    //
    ntStatus = STATUS_NOT_IMPLEMENTED;

    EXIT("kmdf1394_Control", ntStatus);
    return ntStatus;
} // kmdf1394_Control

NTSTATUS
kmdf1394_GetMaxSpeedBetweenDevices (
                                    IN WDFDEVICE Device,
                                    IN WDFREQUEST Request,
                                    IN OUT PGET_MAX_SPEED_BETWEEN_DEVICES MaxSpeedBtwnDevices)
{
    NTSTATUS    ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION  deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;
    ULONG i;

    ENTER("kmdf1394_GetMaxSpeedBetweenDevices");

    for (i=0; i < MaxSpeedBtwnDevices->ulNumberOfDestinations; i++) 
    {
        TRACE(
            TL_TRACE, 
            ("hDestinationDeviceObjects[%d] = 0x%x\n", 
            i, 
            MaxSpeedBtwnDevices->hDestinationDeviceObjects[i]));
    }

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } // if

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_GET_SPEED_BETWEEN_DEVICES;
    pIrb->Flags = 0;
    pIrb->u.GetMaxSpeedBetweenDevices.fulFlags = MaxSpeedBtwnDevices->fulFlags;
    pIrb->u.GetMaxSpeedBetweenDevices.ulNumberOfDestinations = \
        MaxSpeedBtwnDevices->ulNumberOfDestinations;

    for (i=0; i < MaxSpeedBtwnDevices->ulNumberOfDestinations; i++) 
    {
        pIrb->u.GetMaxSpeedBetweenDevices.hDestinationDeviceObjects[i] = \
            MaxSpeedBtwnDevices->hDestinationDeviceObjects[i];
    }

    ntStatus = kmdf1394_SubmitIrpSynch(
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS(ntStatus)) 
    {
        MaxSpeedBtwnDevices->fulSpeed = pIrb->u.GetMaxSpeedBetweenDevices.fulSpeed;

        TRACE(TL_TRACE, ("fulSpeed = 0x%x\n", MaxSpeedBtwnDevices->fulSpeed));
    }
    else 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_GetMaxSpeedBetweenDevices", ntStatus);
    return ntStatus;
} // kmdf1394_GetMaxSpeedBetweenDevices

NTSTATUS
kmdf1394_SetDeviceXmitProperties(
                              IN WDFDEVICE Device,
                              IN WDFREQUEST Request,
                              IN PDEVICE_XMIT_PROPERTIES DeviceXmitProperties)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER("kmdf1394_SetDeviceXmitProperties");

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_SET_DEVICE_XMIT_PROPERTIES;
    pIrb->Flags = 0;
    pIrb->u.SetDeviceXmitProperties.fulSpeed = DeviceXmitProperties->fulSpeed;
    pIrb->u.SetDeviceXmitProperties.fulPriority = DeviceXmitProperties->fulPriority;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS(ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }
    
    ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_SetDeviceXmitProperties", ntStatus);
    return (ntStatus);
} // kmdf1394_SetDeviceXmitProperties

NTSTATUS
kmdf1394_GetConfigurationInformation (
                                     IN WDFDEVICE Device,
                                     IN WDFREQUEST Request)
{
    UNREFERENCED_PARAMETER(Device);
    UNREFERENCED_PARAMETER(Request);

    return STATUS_NOT_IMPLEMENTED;

} // kmdf1394_GetConfigurationInformation

NTSTATUS
kmdf1394_BusReset (
               IN WDFDEVICE Device,
               IN WDFREQUEST Request,
               IN PULONG fulFlags)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER("kmdf1394_BusReset");

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_BUS_RESET;
    pIrb->Flags = 0;
    pIrb->u.BusReset.fulFlags = *fulFlags;

    ntStatus = kmdf1394_SubmitIrpSynch(
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS(ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_BusReset", ntStatus);
    return ntStatus;
} // kmdf1394_BusReset

NTSTATUS
kmdf1394_GetGenerationCount (
                             IN WDFDEVICE     Device,
                             IN WDFREQUEST Request,
                             IN OUT PULONG   GenerationCount)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER("kmdf1394_GetGenerationCount");

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_GET_GENERATION_COUNT;
    pIrb->Flags = 0;

    ntStatus = kmdf1394_SubmitIrpSynch(
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (NT_SUCCESS(ntStatus)) 
    {
        *GenerationCount = pIrb->u.GetGenerationCount.GenerationCount;
        TRACE(TL_TRACE, ("GenerationCount = 0x%x\n", *GenerationCount));
    }
    else 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
    
    EXIT("kmdf1394_GetGenerationCount", ntStatus);
    return ntStatus;
} // kmdf1394_GetGenerationCount

NTSTATUS
kmdf1394_SendPhyConfigurationPacket (
                                 IN WDFDEVICE Device,
                                 IN WDFREQUEST Request,
                                 IN PPHY_CONFIGURATION_PACKET  PhyConfigPacket)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;

    ENTER("kmdf1394_SendPhyConfigurationPacket");

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    } 

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_SEND_PHY_CONFIG_PACKET;
    pIrb->Flags = 0;
    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Force_Root = \
        PhyConfigPacket->PCP_Force_Root;

    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Gap_Count = \
        PhyConfigPacket->PCP_Gap_Count;

    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Inverse = \
        PhyConfigPacket->PCP_Inverse;

    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Packet_ID = \
        PhyConfigPacket->PCP_Packet_ID;

    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Phys_ID = \
        PhyConfigPacket->PCP_Phys_ID;

    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Reserved1 = \
        PhyConfigPacket->PCP_Reserved1;

    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Reserved2 = \
        PhyConfigPacket->PCP_Reserved2;

    pIrb->u.SendPhyConfigurationPacket.PhyConfigurationPacket.PCP_Set_Gap_Count = \
        PhyConfigPacket->PCP_Set_Gap_Count;

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS(ntStatus)) 
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }
    
    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_SendPhyConfigurationPacket", ntStatus);
    return ntStatus;
} //kmdft1394_SendPhyConfigurationPacket


NTSTATUS
kmdf1394_SetLocalHostProperties (
                                 IN WDFDEVICE Device,
                                 IN WDFREQUEST Request,
                                 IN OUT PSET_LOCAL_HOST_INFORMATION SetLocalHostInfo)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PIRB pIrb = NULL;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PLIST_ENTRY listHead, thisEntry;

    PCROM_DATA  CromData = NULL;
    PSET_LOCAL_HOST_PROPS3  R0_SetLocalHostProps3 = NULL;

    ENTER("kmdf1394_SetLocalHostProperties");

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_SET_LOCAL_HOST_PROPERTIES;
    pIrb->Flags = 0;
    pIrb->u.SetLocalHostProperties.nLevel = SetLocalHostInfo->nLevel;

    TRACE(TL_TRACE, ("nLevel = 0x%x\n", SetLocalHostInfo->nLevel));
    TRACE(TL_TRACE, ("Information = 0x%x\n", SetLocalHostInfo->Information));

    if (SET_LOCAL_HOST_PROPERTIES_GAP_COUNT == SetLocalHostInfo->nLevel) 
    {
        PSET_LOCAL_HOST_PROPS2  SetLocalHostProps2;

        SetLocalHostProps2 = (PSET_LOCAL_HOST_PROPS2)SetLocalHostInfo->Information;

        TRACE(
            TL_TRACE, 
            ("GapCountLowerBound = 0x%x\n", 
            SetLocalHostProps2->GapCountLowerBound));

        pIrb->u.SetLocalHostProperties.Information = SetLocalHostInfo->Information;
    }
    else if (SET_LOCAL_HOST_PROPERTIES_MODIFY_CROM == SetLocalHostInfo->nLevel)
    {

        PSET_LOCAL_HOST_PROPS3  SetLocalHostProps3;

        SetLocalHostProps3 = (PSET_LOCAL_HOST_PROPS3)SetLocalHostInfo->Information;

        TRACE(TL_TRACE, ("fulFlags = 0x%x\n", SetLocalHostProps3->fulFlags));
        TRACE(TL_TRACE, ("hCromData = 0x%x\n", SetLocalHostProps3->hCromData));
        TRACE(TL_TRACE, ("nLength = 0x%x\n", SetLocalHostProps3->nLength));

        //
        // since we need to create a mdl, we'll create another setlocalhostprops3
        // and pass that down to the bus driver
        //
        R0_SetLocalHostProps3 = ExAllocatePoolWithTag (
            NonPagedPool,
            sizeof(SET_LOCAL_HOST_PROPS3),
            POOLTAG_KMDF_VDEV);

        if (NULL == R0_SetLocalHostProps3) 
        {
            TRACE(TL_ERROR, ("Failed to allocate R0_SetLocalHostProps3!\n"));

            ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
            return STATUS_INSUFFICIENT_RESOURCES;
        }

        TRACE(TL_TRACE, ("R0_SetLocalHostProps3 = 0x%x\n", R0_SetLocalHostProps3));

        //
        // copy over the contents...
        //
        RtlCopyMemory( 
            R0_SetLocalHostProps3, 
            SetLocalHostProps3, 
            sizeof(SET_LOCAL_HOST_PROPS3));

        //
        // branch, depending if we are adding or removing
        //
        if (SLHP_FLAG_ADD_CROM_DATA == R0_SetLocalHostProps3->fulFlags)
        {
            //
            // we are adding an entry. let's get our crom data struct...
            //
            CromData = ExAllocatePoolWithTag (
                NonPagedPool,
                sizeof(CROM_DATA),
                POOLTAG_KMDF_VDEV);
            if (NULL == CromData) 
            {
                TRACE(TL_ERROR, ("Failed to allocate CromData!\n"));

                ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag (R0_SetLocalHostProps3, POOLTAG_KMDF_VDEV);
                return STATUS_INSUFFICIENT_RESOURCES;
            }

            if (0 == R0_SetLocalHostProps3->nLength)
            {
                TRACE (TL_ERROR, ("R0_SetLocalHostProps3->nLength is 0\n"));

                ExFreePoolWithTag (R0_SetLocalHostProps3, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag (CromData, POOLTAG_KMDF_VDEV);
                return STATUS_INVALID_PARAMETER;
            }

            //
            // let's allocate our buffer...
            //
            CromData->Buffer = ExAllocatePoolWithTag (
                NonPagedPool,
                R0_SetLocalHostProps3->nLength,
                POOLTAG_KMDF_VDEV);

            TRACE(TL_TRACE, ("CromData->Buffer = 0x%x\n", CromData->Buffer));

            if (0 == CromData->Buffer) 
            {
                TRACE(TL_ERROR, ("Failed to allocate CromData->Buffer!\n"));

                ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag(R0_SetLocalHostProps3, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag(CromData, POOLTAG_KMDF_VDEV);
                return STATUS_INSUFFICIENT_RESOURCES;

            }

            RtlCopyMemory (
                CromData->Buffer,
                &SetLocalHostProps3->Mdl,
                SetLocalHostProps3->nLength);

            R0_SetLocalHostProps3->Mdl = IoAllocateMdl (
                CromData->Buffer,
                R0_SetLocalHostProps3->nLength,
                FALSE,
                FALSE,
                NULL);
            if (NULL == R0_SetLocalHostProps3->Mdl) 
            {
                TRACE(TL_ERROR, ("Failed to allocate mdl for CromData->Buffer!\n"));

                ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag(R0_SetLocalHostProps3, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag(CromData, POOLTAG_KMDF_VDEV);
                return STATUS_INSUFFICIENT_RESOURCES;               
            }

            MmBuildMdlForNonPagedPool(R0_SetLocalHostProps3->Mdl);

            TRACE(TL_TRACE, ("Mdl = 0x%x\n", R0_SetLocalHostProps3->Mdl));
        }
        else if (SLHP_FLAG_REMOVE_CROM_DATA == SetLocalHostProps3->fulFlags) 
        {
            TRACE(TL_TRACE, ("hCromData = 0x%x\n", R0_SetLocalHostProps3->hCromData));
        }

        pIrb->u.SetLocalHostProperties.Information = (PVOID)R0_SetLocalHostProps3;
    }

    ntStatus = kmdf1394_SubmitIrpSynch (
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS (ntStatus)) 
    {
        if (SET_LOCAL_HOST_PROPERTIES_MODIFY_CROM == SetLocalHostInfo->nLevel) 
        {
            if (SLHP_FLAG_ADD_CROM_DATA == R0_SetLocalHostProps3->fulFlags)
            {
                IoFreeMdl (R0_SetLocalHostProps3->Mdl);
                ExFreePoolWithTag (CromData->Buffer, POOLTAG_KMDF_VDEV);
                ExFreePoolWithTag (CromData, POOLTAG_KMDF_VDEV);
            }

            ExFreePoolWithTag(R0_SetLocalHostProps3, POOLTAG_KMDF_VDEV);
        }

        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }
    else 
    {
        if (SET_LOCAL_HOST_PROPERTIES_MODIFY_CROM== SetLocalHostInfo->nLevel)
        {
            //
            // branch, depending if we are adding or removing
            //
            if (SLHP_FLAG_ADD_CROM_DATA == R0_SetLocalHostProps3->fulFlags) 
            {
                PSET_LOCAL_HOST_PROPS3  SetLocalHostProps3;

                SetLocalHostProps3 = \
                    (PSET_LOCAL_HOST_PROPS3)SetLocalHostInfo->Information;
                SetLocalHostProps3->hCromData = R0_SetLocalHostProps3->hCromData;

                TRACE (
                    TL_TRACE, 
                    ("hCromData = 0x%x\n", 
                    SetLocalHostProps3->hCromData));

                CromData->hCromData = SetLocalHostProps3->hCromData;
                CromData->pMdl = R0_SetLocalHostProps3->Mdl;

                //
                // need to add to our list...
                //
                WdfSpinLockAcquire (deviceExtension->CromSpinLock);
                InsertHeadList (&deviceExtension->CromData, &CromData->CromList);

                WdfSpinLockRelease (deviceExtension->CromSpinLock);
            }
            else if (SLHP_FLAG_REMOVE_CROM_DATA == R0_SetLocalHostProps3->fulFlags) 
            {
                //
                // have to find our struct...
                //
                WdfSpinLockAcquire (deviceExtension->CromSpinLock);

                listHead = &deviceExtension->CromData;

                for(thisEntry = listHead->Flink;
                    thisEntry != listHead;
                    CromData = NULL, thisEntry = thisEntry->Flink)
                {
                    CromData = CONTAINING_RECORD (thisEntry, CROM_DATA, CromList);
                    if (CromData->hCromData == R0_SetLocalHostProps3->hCromData) 
                    {
                        RemoveEntryList (&CromData->CromList);
                        break;
                    }
                }

                WdfSpinLockRelease(deviceExtension->CromSpinLock);

                ExFreePoolWithTag (CromData->Buffer, POOLTAG_KMDF_VDEV);
                IoFreeMdl (CromData->pMdl);
                ExFreePoolWithTag (CromData, POOLTAG_KMDF_VDEV);
            }// elseif ()

            ExFreePoolWithTag(R0_SetLocalHostProps3, POOLTAG_KMDF_VDEV);

        } //  if (SET_LOCAL_HOST_PROPERTIES_MODIFY_CROM== SetLocalHostInfo->nLevel)
    }// else

    ExFreePoolWithTag(pIrb, POOLTAG_KMDF_VDEV);

    EXIT("kmdf1394_SetLocalHostProperties", ntStatus);
    return ntStatus;
} // kmdf1394_SetLocalHostProperties

VOID
kmdf1394_BusResetRoutineWorkItem (
                                 IN WDFWORKITEM  WorkItem)
{
    WDFDEVICE Device= WdfWorkItemGetParentObject (WorkItem);

    kmdf1394_UpdateGenerationCount (Device);

    WdfObjectDelete (WorkItem);
}

VOID
kmdf1394_BusResetRoutine(
                         IN PVOID    Context)
{
    WDFDEVICE device= Context;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(device);
    NTSTATUS ntStatus    = STATUS_SUCCESS;
    WDFREQUEST request;
    WDF_WORKITEM_CONFIG workItemConfig;
    WDFWORKITEM workItem;
    WDF_OBJECT_ATTRIBUTES attributes;

    ENTER("kmdf1394_BusResetRoutine");

    TRACE(TL_TRACE, ("Context = 0x%x\n", Context));

    WDF_WORKITEM_CONFIG_INIT (&workItemConfig, kmdf1394_BusResetRoutineWorkItem);

    WDF_OBJECT_ATTRIBUTES_INIT (&attributes);
    attributes.ParentObject = device;

    ntStatus = WdfWorkItemCreate( &workItemConfig,
        &attributes,
        &workItem);
    if (!NT_SUCCESS (ntStatus)) 
    {
        TRACE(TL_ERROR, ("Failed to create workitem %x\n", ntStatus));
        return;
    }

    //
    // Execute this work item.
    //
    WdfWorkItemEnqueue (workItem);

    //
    // If we have any bus reset notify irps, then nows the
    // time to complete them.
    //
    WHILE (TRUE)
    {
        ntStatus = WdfIoQueueRetrieveNextRequest (
            deviceExtension->BusResetRequestsQueue,
            &request);
        if(NT_SUCCESS (ntStatus))
        {
            TRACE(TL_TRACE, ("Completing BusReset Request = 0x%p\n", request));
            WdfRequestCompleteWithInformation (request, STATUS_SUCCESS, 0);
            // continue;
        }
        else if (STATUS_NO_MORE_ENTRIES == ntStatus)
        {
            TRACE (TL_TRACE, ("Request Queue is empty.\n"));
            break;
        }
        else 
        {
            ASSERTMSG (
                "WdfIoQueueRetrieveNextRequest failed", 
                ntStatus);
            break;
        }
    }

    EXIT("kmdf1394_BusResetRoutine", ntStatus);
} // kmdf1394_BusResetRoutine


NTSTATUS
kmdf1394_BusResetNotification (
                              IN WDFDEVICE Device,
                              IN WDFREQUEST Request,
                              IN ULONG fulFlags)
{
    NTSTATUS ntStatus = STATUS_SUCCESS;
    PDEVICE_EXTENSION deviceExtension = GetDeviceContext(Device);
    PIRB pIrb = NULL;
    WDFREQUEST newRequest = NULL;

    ENTER("kmdf1394_BusResetNotification");

    pIrb = ExAllocatePoolWithTag(NonPagedPool, sizeof(IRB), POOLTAG_KMDF_VDEV);
    if (!pIrb) 
    {
        TRACE(TL_ERROR, ("Failed to allocate pIrb!\n"));
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    if(!Request) 
    {
        ntStatus = WdfRequestCreate (
            WDF_NO_OBJECT_ATTRIBUTES,
            deviceExtension->StackIoTarget,
            &newRequest);

        if (!NT_SUCCESS (ntStatus)) 
        {
            TRACE(TL_ERROR, ("Failed to allocate request!\n"));

            ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
            return ntStatus;
        }

        Request = newRequest;
    }

    RtlZeroMemory (pIrb, sizeof (IRB));
    pIrb->FunctionNumber = REQUEST_BUS_RESET_NOTIFICATION;
    pIrb->Flags = 0;
    pIrb->u.BusResetNotification.fulFlags = fulFlags;
    pIrb->u.BusResetNotification.ResetRoutine = kmdf1394_BusResetRoutine;
    pIrb->u.BusResetNotification.ResetContext = Device;

    ntStatus = kmdf1394_SubmitIrpSynch(
        deviceExtension->StackIoTarget, 
        Request, 
        pIrb);
    if (!NT_SUCCESS(ntStatus))
    {
        TRACE(TL_ERROR, ("SubmitIrpSync failed = 0x%x\n", ntStatus));
    }
    
    ExFreePoolWithTag (pIrb, POOLTAG_KMDF_VDEV);
    
    if (newRequest)
    {
        WdfObjectDelete (newRequest);
    }
   
    EXIT("kmdf1394_BusResetNotification", ntStatus);
    return ntStatus;
} // kmdf1394_BusResetNotification


