/*++

Copyright (c) Microsoft Corporation.  All rights reserved.

    THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
    KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
    IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A PARTICULAR
    PURPOSE.

Module Name:

   INIT.C

Abstract:

    This module contains initialization helper routines called during
    MiniportInitialize.

--*/

#include "ndiswdm.h"

#pragma NDIS_PAGEABLE_FUNCTION(NICAllocAdapter)
#pragma NDIS_PAGEABLE_FUNCTION(NICFreeAdapter)
#pragma NDIS_PAGEABLE_FUNCTION(NICAllocSendResources)
#pragma NDIS_PAGEABLE_FUNCTION(NICFreeSendResources)
#pragma NDIS_PAGEABLE_FUNCTION(NICAllocRecvResources)
#pragma NDIS_PAGEABLE_FUNCTION(NICFreeRecvResources)
#pragma NDIS_PAGEABLE_FUNCTION(NICReadRegParameters)
#pragma NDIS_PAGEABLE_FUNCTION(NICInitializeAdapter)
#pragma NDIS_PAGEABLE_FUNCTION(NICInitWorkItemCallback)
#pragma NDIS_PAGEABLE_FUNCTION(NICInitAdapterWorker)

#ifdef INTERFACE_WITH_NDISPROT

#pragma NDIS_PAGEABLE_FUNCTION(NICOpenNdisProtocolInterface)

#endif


ULONG    g_ulAddress = 0;


NDIS_STATUS
NICAllocAdapter(
    PMP_ADAPTER *pAdapter)
/*++

Routine Description:

    Allocates memory of our Adapter Context structure.

Arguments:

    Adapter    Pointer to our adapter

    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:

    NDIS_STATUS_SUCCESS
    NDIS_STATUS_FAILURE
    NDIS_STATUS_RESOURCES

--*/
{
    PMP_ADAPTER Adapter = NULL;
    NDIS_STATUS Status;

    PAGED_CODE();

    DEBUGP(MP_TRACE, ("--> NICAllocAdapter\n"));

    *pAdapter = NULL;

    do
    {
        //
        // Allocate memory for adapter context
        //
        Status = NdisAllocateMemoryWithTag(
            &Adapter,
            sizeof(MP_ADAPTER),
            NIC_TAG);
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, ("Failed to allocate memory for adapter context\n"));
            break;
        }
        //
        // Zero the memory block
        //
        NdisZeroMemory(Adapter, sizeof(MP_ADAPTER));
        //
        // Lock to synchronize access to misc state variables used to track the
        // overall state of the Adapter.
        NdisAllocateSpinLock(&Adapter->Lock);

        //
        //
        // List to link other instances of adapter.
        //
        NdisInitializeListHead(&Adapter->List);
        //
        // Event used to synchronize execution of NICInitAdapterWorker routine.
        // We want to let only one thread execute this routine at any time.
        //
        KeInitializeEvent((PKEVENT)&Adapter->InitEvent, SynchronizationEvent, TRUE);
        //
        // This event is used to make sure all the asynchronously running routines
        // such as DPCs, workitems, threads have exited before returning from halt
        // handler.
        //
        NdisInitializeEvent(&Adapter->HaltEvent);

    } WHILE (FALSE);


    *pAdapter = Adapter;

    //
    // In the failure case, the caller of this routine will end up
    // calling NICFreeAdapter to free all the successfully allocated
    // resources.
    //
    DEBUGP(MP_TRACE, ("<-- NICAllocAdapter\n"));

    return(Status);

}

VOID
NICFreeAdapter(
    PMP_ADAPTER Adapter)
/*++

Routine Description:

    Frees memory of our Adapter Context structure.

Arguments:

    Adapter    Pointer to our adapter

    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:

    VOID

--*/
{
    DEBUGP(MP_TRACE, ("--> NICFreeAdapter\n"));

    PAGED_CODE();

    ASSERT(Adapter);

    ASSERT(!Adapter->RefCount);

    NdisFreeSpinLock(&Adapter->Lock);

    if (Adapter->WdfDevice){
        WdfObjectDelete(Adapter->WdfDevice);
    }

    //
    // Finally free the memory for adapter context.
    //
    NdisFreeMemory(Adapter, sizeof(MP_ADAPTER), 0);

    DEBUGP(MP_TRACE, ("<-- NICFreeAdapter\n"));
}

NDIS_STATUS
NICAllocSendResources(
    PMP_ADAPTER Adapter)
/*++
Routine Description:

    Allocate resources required to Send packets to the device

Arguments:

    Adapter    Pointer to our adapter
    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:

    NDIS_STATUS_SUCCESS
    NDIS_STATUS_FAILURE
    NDIS_STATUS_RESOURCES

--*/
{
    NDIS_STATUS Status;
    NTSTATUS    ntStatus;
    UINT index;
    PNDIS_BUFFER Buffer = NULL;
    PUCHAR pTCBMem = NULL;

    DEBUGP(MP_TRACE, ("--> NICAllocSendResources\n"));

    PAGED_CODE();

    do
    {
        //
        // Following are the lists to hold packets at different
        // stages of processing.
        // SendWaitList - Original send packets waiting to be processed
        // SendFreeList - Packets available for send operation
        // SendBusyList - Packets sent to the lower WDM stack
        // SendLock is used to synchronize access to these lists.
        //
        NdisInitializeListHead(&Adapter->SendWaitList);
        NdisInitializeListHead(&Adapter->SendFreeList);
        NdisInitializeListHead(&Adapter->SendBusyList);
        NdisAllocateSpinLock(&Adapter->SendLock);

        //
        // Let us set the flag to indicate resources are allocated.
        // NICFreeSendResources will check this flag to determine whether there
        // is any resource that needs to be freed.
        //

        MP_SET_FLAG(Adapter, fMP_SEND_SIDE_RESOURCE_ALLOCATED);

        //
        // Allocate a huge block of memory for all TCB's
        //
        Status = NdisAllocateMemoryWithTag(
            &pTCBMem,
            sizeof(TCB) * NIC_MAX_BUSY_SENDS,
            NIC_TAG);

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, ("Failed to allocate memory for TCB's\n"));
            break;
        }
        NdisZeroMemory(pTCBMem, sizeof(TCB) * NIC_MAX_BUSY_SENDS);
        Adapter->TCBMem = pTCBMem;

        //
        // Allocate a buffer pool for send buffers.
        //

        NdisAllocateBufferPool(
            &Status,
            &Adapter->SendBufferPoolHandle,
            NIC_MAX_BUSY_SENDS);
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, ("NdisAllocateBufferPool for send buffer failed\n"));
            break;
        }

        //
        // Divide the TCBMem blob into TCBs and create a buffer
        // descriptor for the Data portion of the TCBs. The reason for doing
        // this instead of using the OriginalSend Packet buffers is because
        // the target driver we are talking to is not capable of handling
        // chained buffers (MDLs).
        //
        for(index=0; index < NIC_MAX_BUSY_SENDS; index++)
        {
            PTCB pTCB = (PTCB) pTCBMem;
            //
            // Create a buffer descriptor for the Data portion of the TCBs.
            // Buffer descriptors are nothing but MDLs on NT systems.
            //
            NdisAllocateBuffer(
                &Status,
                &Buffer,
                Adapter->SendBufferPoolHandle,
                (PVOID)&pTCB->Data[0],
                NIC_BUFFER_SIZE);
            if (Status != NDIS_STATUS_SUCCESS)
            {
                DEBUGP(MP_ERROR, ("NdisAllocateBuffer failed\n"));
                break;
            }

            //
            // Initialize the TCB structure.
            //
            pTCB->Buffer = Buffer;
            pTCB->pData = (PUCHAR) &pTCB->Data[0];
            pTCB->Adapter = Adapter;

            //
            // Preallocate all the Requests required.  Do not charge quota
            // to the current process for this Request. If the miniport is
            // loaded as a function driver of the WDM stack, say in case
            // of USB, the IoTarget will be the NextDeviceObject.
            //

            ASSERT(Adapter->IoTarget);

            if (!Adapter->IoTarget) {
                ntStatus = STATUS_UNSUCCESSFUL;
                NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
                break;
            }

            ntStatus = WdfRequestCreate(
                WDF_NO_OBJECT_ATTRIBUTES,
                Adapter->IoTarget,
                &pTCB->Request);

            if (!NT_SUCCESS(ntStatus)) {
                NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
                break;
            }

            //
            // Insert TCB blocks into FreeList.
            //
            NdisInterlockedInsertTailList(
                &Adapter->SendFreeList,
                &pTCB->List,
                &Adapter->SendLock);

            pTCBMem = pTCBMem + sizeof(TCB);

        }

    } WHILE (FALSE);

    DEBUGP(MP_TRACE, ("<-- NICAllocSendResources %x\n", Status));

    return Status;
}

VOID
NICFreeSendResources(
    PMP_ADAPTER Adapter)
/*++
Routine Description:

  Free resources allocated for send operation

Arguments:

    Adapter     Pointer to our adapter
    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:

--*/
{
    PTCB   pTCB;

    DEBUGP(MP_TRACE, ("--> NICFreeSendResources\n"));

    PAGED_CODE();

    //
    // Did we allocate any resources for send?
    //
    if (!MP_TEST_FLAG(Adapter, fMP_SEND_SIDE_RESOURCE_ALLOCATED)){
        return;
    }

    while (!IsListEmpty(&Adapter->SendFreeList))
    {
        pTCB = (PTCB) NdisInterlockedRemoveHeadList(
                     &Adapter->SendFreeList,
                     &Adapter->SendLock);
        if (!pTCB) {
            break;
        }

        if (pTCB->Buffer) {
            NdisFreeBuffer(pTCB->Buffer);
        }
        if (pTCB->Request){
            WdfObjectDelete(pTCB->Request);
        }
    }

    if (Adapter->SendBufferPoolHandle)
    {
        NdisFreeBufferPool(Adapter->SendBufferPoolHandle);
        Adapter->SendBufferPoolHandle = NULL;
    }

    if (Adapter->TCBMem) {
        NdisFreeMemory(Adapter->TCBMem, sizeof(TCB) * NIC_MAX_BUSY_SENDS, 0);
        Adapter->TCBMem = NULL;
    }

    ASSERT(IsListEmpty(&Adapter->SendFreeList));
    ASSERT(IsListEmpty(&Adapter->SendWaitList));

    NdisFreeSpinLock(&Adapter->SendLock);

    MP_CLEAR_FLAG(Adapter, fMP_SEND_SIDE_RESOURCE_ALLOCATED);

}

NDIS_STATUS
NICAllocRecvResources(
    PMP_ADAPTER Adapter)
/*++
Routine Description:

    Allocate resources required to receive packets from the WDM driver


Arguments:

    Adapter - Pointer to our adapter

    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:

    NDIS_STATUS_SUCCESS
    NDIS_STATUS_FAILURE
    NDIS_STATUS_RESOURCES

--*/
{
    PUCHAR pRCBMem = NULL;
    PNDIS_PACKET Packet = NULL;
    PNDIS_BUFFER Buffer = NULL;
    UINT index;
    NDIS_STATUS Status;
    NTSTATUS    ntStatus;
    WDF_OBJECT_ATTRIBUTES   attributes;
    WDF_WORKITEM_CONFIG     workitemConfig;

    DEBUGP(MP_TRACE, ("--> NICAllocRecvResources\n"));

    PAGED_CODE();

    do{

        //
        // Workitem for posting read requests.
        //
        WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
        attributes.ParentObject = Adapter->WdfDevice;

        WDF_WORKITEM_CONFIG_INIT(&workitemConfig, NICPostReadsWorkItemCallBack);

        ntStatus = WdfWorkItemCreate(&workitemConfig,
                                    &attributes,
                                    &Adapter->ReadWorkItem);

        if (!NT_SUCCESS(ntStatus)) {
            DEBUGP(MP_ERROR, ("WdfWorkItemCreate failed 0x%x\n", ntStatus));
            NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
            break;
        }

        //
        // Following are the lists to hold packets at different
        // stages of processing.
        // RecvFreeList - Packets available for received operation
        // RecvBusyList - Packets posted  to the lower WDM stack
        // RecvLock is used to synchronize access to these lists.
        //
        NdisInitializeListHead(&Adapter->RecvFreeList);
        NdisInitializeListHead(&Adapter->RecvBusyList);
        NdisAllocateSpinLock(&Adapter->RecvLock);

        //
        // Let us set the flag to indicate that resources are allocated.
        // NICFreeRecvResources will check this flag to determine whether there
        // is any resource that needs to be freed.
        //

        MP_SET_FLAG(Adapter, fMP_RECV_SIDE_RESOURCE_ALLOCATED);

        //
        // Allocate a huge block of memory for all RCB's
        //
        Status = NdisAllocateMemoryWithTag(
            &pRCBMem,
            sizeof(RCB) * NIC_MAX_BUSY_RECVS,
            NIC_TAG);

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, ("Failed to allocate memory for RCB's\n"));
            break;
        }

        NdisZeroMemory(pRCBMem, sizeof(RCB) * NIC_MAX_BUSY_RECVS);
        Adapter->RCBMem = pRCBMem;

        //
        // Allocate a buffer pool for recv buffers.
        //

        NdisAllocateBufferPool(
            &Status,
            &Adapter->RecvBufferPoolHandle,
            NIC_MAX_BUSY_RECVS);
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, ("NdisAllocateBufferPool for recv buffer failed\n"));
            break;
        }

        //
        // Allocate packet pool for receive indications
        //
        NdisAllocatePacketPool(
            &Status,
            &Adapter->RecvPacketPoolHandle,
            NIC_MAX_BUSY_RECVS,
            PROTOCOL_RESERVED_SIZE_IN_PACKET);

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, ("NdisAllocatePacketPool failed\n"));
            break;
        }

        //
        // Divide the RCBMem blob into RCBs and create a buffer
        // descriptor for the Data portion of the RCBs.
        //
        for(index=0; index < NIC_MAX_BUSY_RECVS; index++)
        {
            PRCB pRCB = (PRCB) pRCBMem;
            //
            // Create a buffer descriptor for the Data portion of the RCBs.
            // Buffer descriptors are nothing but MDLs on NT systems.
            //
            NdisAllocateBuffer(
                &Status,
                &Buffer,
                Adapter->RecvBufferPoolHandle,
                (PVOID)&pRCB->Data[0],
                NIC_BUFFER_SIZE);
            if (Status != NDIS_STATUS_SUCCESS)
            {
                DEBUGP(MP_ERROR, ("NdisAllocateBuffer for Recv failed\n"));
                break;
            }

            //
            // Initialize the RCB structure.
            //
            pRCB->Buffer = Buffer;
            pRCB->pData = (PUCHAR) &pRCB->Data[0];
            pRCB->Adapter = Adapter;

            //
            // Preallocate all the Requests required.  Do not charge quota
            // to the current process for this Request.
            //

            ASSERT(Adapter->IoTarget);

            if (!Adapter->IoTarget) {
                Status = NDIS_STATUS_FAILURE;
                break;
            }

            ntStatus = WdfRequestCreate(
                WDF_NO_OBJECT_ATTRIBUTES,
                Adapter->IoTarget,
                &pRCB->Request);

            if (!NT_SUCCESS(ntStatus)) {
                NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
                break;
            }

            //
            // Initialize receive packets.
            // Allocate a packet descriptor for receive packets
            // from a preallocated pool.
            //
            NdisAllocatePacket(
                &Status,
                &Packet,
                Adapter->RecvPacketPoolHandle);
            if (Status != NDIS_STATUS_SUCCESS)
            {
                DEBUGP(MP_ERROR, ("NdisAllocatePacket failed\n"));
                break;
            }

            pRCB->Packet = Packet;

            NDIS_SET_PACKET_HEADER_SIZE(Packet, ETH_HEADER_SIZE);

            NdisInterlockedInsertTailList(
                &Adapter->RecvFreeList,
                &pRCB->List,
                &Adapter->RecvLock);

            pRCBMem = pRCBMem + sizeof(RCB);

        }

    } WHILE (FALSE);

    DEBUGP(MP_TRACE, ("--> NICAllocRecvResources %x\n", Status));

    return Status;

}


VOID
NICFreeRecvResources(
    PMP_ADAPTER Adapter)
/*++
Routine Description:

   Free resources allocated for receive operation

Arguments:

    Adapter - Pointer to our adapter

    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:


--*/
{
    PRCB   pRCB;

    DEBUGP(MP_TRACE, ("--> NICFreeRecvResources\n"));

    PAGED_CODE();

    //
    // Free all the resources we allocated for receive.
    //
    if (!MP_TEST_FLAG(Adapter, fMP_RECV_SIDE_RESOURCE_ALLOCATED)){
        return;
    }

    while (!IsListEmpty(&Adapter->RecvFreeList))
    {
        pRCB = (PRCB) NdisInterlockedRemoveHeadList(
                     &Adapter->RecvFreeList,
                     &Adapter->RecvLock);
        if (!pRCB){
            break;
        }

        if (pRCB->Buffer){
            NdisFreeBuffer(pRCB->Buffer);
        }
        if (pRCB->Packet) {
            NdisFreePacket(pRCB->Packet);
        }
        if (pRCB->Request){
            WdfObjectDelete(pRCB->Request);
        }

    }

    if (Adapter->RecvPacketPoolHandle)
    {
        NdisFreePacketPool(Adapter->RecvPacketPoolHandle);
        Adapter->RecvPacketPoolHandle = NULL;
    }

    if (Adapter->RecvBufferPoolHandle)
    {
        NdisFreeBufferPool(Adapter->RecvBufferPoolHandle);
        Adapter->RecvBufferPoolHandle = NULL;
    }

    if (Adapter->RCBMem)
    {
        NdisFreeMemory(Adapter->RCBMem, sizeof(RCB) * NIC_MAX_BUSY_RECVS, 0);
    }

    ASSERT(IsListEmpty(&Adapter->RecvFreeList));
    ASSERT(IsListEmpty(&Adapter->RecvBusyList));

    NdisFreeSpinLock(&Adapter->RecvLock);

    MP_CLEAR_FLAG(Adapter, fMP_RECV_SIDE_RESOURCE_ALLOCATED);

    return;
}



VOID
NICAttachAdapter(
    PMP_ADAPTER Adapter)
/*++
Routine Description:

   Attach this adapter to the global list of adapters controlled by this
   driver.

Arguments:

    Adapter - Pointer to our adapter

    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:


--*/
{
    DEBUGP(MP_TRACE, ("--> NICAttachAdapter\n"));

    NdisInterlockedInsertTailList(
        &GlobalData.AdapterList,
        &Adapter->List,
        &GlobalData.Lock);

    DEBUGP(MP_TRACE, ("<-- NICAttachAdapter\n"));
}

VOID NICDetachAdapter(
    PMP_ADAPTER Adapter)
/*++
Routine Description:

   Detach this adapter from the global list of adapters controlled by this
   driver.

Arguments:

    Adapter - Pointer to our adapter

    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:


--*/
{
    DEBUGP(MP_TRACE, ("--> NICDetachAdapter\n"));

    NdisAcquireSpinLock(&GlobalData.Lock);
    RemoveEntryList(&Adapter->List);
    NdisReleaseSpinLock(&GlobalData.Lock);
    DEBUGP(MP_TRACE, ("<-- NICDetachAdapter\n"));
}

NDIS_STATUS
NICReadRegParameters(
    PMP_ADAPTER Adapter,
    NDIS_HANDLE WrapperConfigurationContext)
/*++
Routine Description:

    Read device configuration parameters from the registry

Arguments:

    Adapter                         Pointer to our adapter
    WrapperConfigurationContext     For use by NdisOpenConfiguration

    Should be called at IRQL = PASSIVE_LEVEL.

Return Value:

    NDIS_STATUS_SUCCESS
    NDIS_STATUS_FAILURE
    NDIS_STATUS_RESOURCES

--*/
{
    NDIS_STATUS     Status = NDIS_STATUS_SUCCESS;
    NDIS_HANDLE     ConfigurationHandle;
    PUCHAR          NetworkAddress;
    UINT            Length;
    PUCHAR          pAddr;
    PNDIS_CONFIGURATION_PARAMETER pParameterValue;
    NDIS_STRING strMiniportName = NDIS_STRING_CONST("MiniportName");
    NDIS_STRING strFilterName = NDIS_STRING_CONST("Promiscuous");

    DEBUGP(MP_TRACE, ("--> NICReadRegParameters\n"));

    PAGED_CODE();

    //
    // Open the registry for this adapter to read advanced
    // configuration parameters stored by the INF file.
    //
    NdisOpenConfiguration(
        &Status,
        &ConfigurationHandle,
        WrapperConfigurationContext);
    if (Status != NDIS_STATUS_SUCCESS)
    {
        DEBUGP(MP_ERROR, ("NdisOpenConfiguration failed\n"));
        return NDIS_STATUS_FAILURE;
    }

    do
    {
        //
        // Read the Miniport Name.
        // This feature is available only XP and above.
        //

        NdisReadConfiguration(&Status,
                              &pParameterValue,
                              ConfigurationHandle,
                              &strMiniportName,
                              NdisParameterString);

        if (Status != NDIS_STATUS_SUCCESS) {

            DEBUGP(MP_ERROR, ("NdisReadConfiguration for miniport name failed\n"));

        } else {
            Length = min(NIC_ADAPTER_NAME_SIZE-1,
              pParameterValue->ParameterData.StringData.Length/sizeof(WCHAR));

            RtlStringCchCopyW(Adapter->AdapterName, Length+1,
                    (PWCHAR)pParameterValue->ParameterData.StringData.Buffer);
        }

        //
        // Read the Promiscuous filter value.
        //

        NdisReadConfiguration(&Status,
                              &pParameterValue,
                              ConfigurationHandle,
                              &strFilterName,
                              NdisParameterInteger);

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(MP_ERROR, ("NdisReadConfiguration for promiscuous key failed\n"));
            break;
        }

        Adapter->Promiscuous = (BOOLEAN)pParameterValue->ParameterData.IntegerData;

    } WHILE (FALSE);

    //
    // Just for testing purposes, let us make up a dummy mac address.
    // In order to avoid conflicts with MAC addresses, it is usually a good
    // idea to check the IEEE OUI list (e.g. at
    // http://standards.ieee.org/regauth/oui/oui.txt). According to that
    // list 00-50-F2 is owned by Microsoft.
    //
    // An important rule to "generating" MAC addresses is to have the
    // "locally administered bit" set in the address, which is bit 0x02 for
    // LSB-type networks like Ethernet. Also make sure to never set the
    // multicast bit in any MAC address: bit 0x01 in LSB networks.
    //

    pAddr = (PUCHAR) &g_ulAddress;

    ++g_ulAddress;

    Adapter->PermanentAddress[0] = 0x02;
    Adapter->PermanentAddress[1] = 0x50;
    Adapter->PermanentAddress[2] = 0xF2;
    Adapter->PermanentAddress[3] = 0x00;
    Adapter->PermanentAddress[4] = 0x01;
    Adapter->PermanentAddress[5] = 0x80|pAddr[0];


    ETH_COPY_NETWORK_ADDRESS(
                        Adapter->CurrentAddress,
                        Adapter->PermanentAddress);

    //
    // Read NetworkAddress registry value and use it as the current address
    // if there is a software configurable NetworkAddress specified in
    // the registry.
    //
    NdisReadNetworkAddress(
        &Status,
        &NetworkAddress,
        &Length,
        ConfigurationHandle);

    if ((Status == NDIS_STATUS_SUCCESS) && (Length == ETH_LENGTH_OF_ADDRESS))
    {
          if ((ETH_IS_MULTICAST(NetworkAddress)
                    || ETH_IS_BROADCAST(NetworkAddress))
                    || !ETH_IS_LOCALLY_ADMINISTERED (NetworkAddress))
            {
                DEBUGP(MP_ERROR,
                    ("Overriding NetworkAddress is invalid - %02x-%02x-%02x-%02x-%02x-%02x\n",
                    NetworkAddress[0], NetworkAddress[1], NetworkAddress[2],
                    NetworkAddress[3], NetworkAddress[4], NetworkAddress[5]));
            }
            else
            {
                ETH_COPY_NETWORK_ADDRESS(Adapter->CurrentAddress, NetworkAddress);
            }
    }

    DEBUGP(MP_WARNING, ("Permanent Address = %02x-%02x-%02x-%02x-%02x-%02x\n",
        Adapter->PermanentAddress[0],
        Adapter->PermanentAddress[1],
        Adapter->PermanentAddress[2],
        Adapter->PermanentAddress[3],
        Adapter->PermanentAddress[4],
        Adapter->PermanentAddress[5]));

    DEBUGP(MP_WARNING, ("Current Address = %02x-%02x-%02x-%02x-%02x-%02x\n",
        Adapter->CurrentAddress[0],
        Adapter->CurrentAddress[1],
        Adapter->CurrentAddress[2],
        Adapter->CurrentAddress[3],
        Adapter->CurrentAddress[4],
        Adapter->CurrentAddress[5]));

    Adapter->ulLinkSpeed = NIC_LINK_SPEED;

    //
    // Close the configuration registry
    //
    NdisCloseConfiguration(ConfigurationHandle);

    DEBUGP(MP_TRACE, ("<-- NICReadRegParameters\n"));

    return NDIS_STATUS_SUCCESS;
}

NDIS_STATUS
NICInitializeAdapter(
    IN  PMP_ADAPTER  Adapter,
    IN  NDIS_HANDLE  WrapperConfigurationContext
    )
/*++
Routine Description:

    Initialize the adapter.

Arguments:

    Adapter     Pointer to our adapter
    WrapperConfigurationContext - Pointer to the configuration context
                only valid during MiniportInitialize.
Return Value:

    NDIS_STATUS_SUCCESS
    NDIS_STATUS_ADAPTER_NOT_FOUND

--*/
{
    UCHAR               resBuf[NIC_RESOURCE_BUF_SIZE];
    PNDIS_RESOURCE_LIST resList = (PNDIS_RESOURCE_LIST)resBuf;
    UINT                bufSize = NIC_RESOURCE_BUF_SIZE;
    NDIS_STATUS         Status = NDIS_STATUS_SUCCESS;
    NTSTATUS            ntStatus;
    WDF_OBJECT_ATTRIBUTES   attributes;
    WDF_WORKITEM_CONFIG     workitemConfig;

    DEBUGP(MP_TRACE, ("---> InitializeAdapter\n"));

    PAGED_CODE();

    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, WDM_NDIS_REQUEST);

    attributes.ParentObject = Adapter->WdfDevice;

    WDF_WORKITEM_CONFIG_INIT(&workitemConfig, NICRequestWorkItemCallback);


    ntStatus = WdfWorkItemCreate(&workitemConfig,
                                &attributes,
                                &Adapter->WorkItemForNdisRequest);

    if (!NT_SUCCESS(ntStatus)) {
        DEBUGP(MP_ERROR, ("WdfWorkItemCreate failed 0x%x\n", ntStatus));
        NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
        return Status;
    }

    //
    // Get the resources assigned by the PNP manager. NDIS gets
    // these resources in IRP_MN_START_DEVICE request.
    //
    NdisMQueryAdapterResources(
        &Status,
        WrapperConfigurationContext,
        resList,
        &bufSize);
    //
    // We need to know whether the device has been assigned any resources
    // by the bus driver because this enables has to decide whether to
    // indicate the received packets at PASSIVE_LEVEL or DISPATCH_LEVEL.
    // NDIS makes an assumption that any device that uses hardware resources
    // will indicate packets from a DPC routine.
    //
    if (Status == NDIS_STATUS_SUCCESS) {
        //
        // There is possibility that we are enumerated by the toaster bus
        // driver. The toaster bus driver just for illustration purpose
        // assigns a port resource to every child it pops.
        //
        Adapter->IsHardwareDevice = TRUE;

    } else {
        //
        // No hardware resources. So we will indicate the packets at
        // PASSIVE_LEVEL.
        //
        Adapter->IsHardwareDevice = FALSE;

    }
    //
    // Set our media state of disconnected unitl we are successfully connected
    // to the target device. This flag will be cleared by the
    //
    MP_SET_FLAG(Adapter, fMP_DISCONNECTED);

#ifdef NDIS50_MINIPORT
    //
    // Register a shutdown handler for NDIS50 or earlier miniports
    // For NDIS51 miniports, set AdapterShutdownHandler.
    //
    NdisMRegisterAdapterShutdownHandler(
        Adapter->AdapterHandle,
        (PVOID) Adapter,
        (ADAPTER_SHUTDOWN_HANDLER) MPShutdown);
#endif


    ntStatus = NICInitAdapterWorker(Adapter);
    if (ntStatus == STATUS_OBJECT_NAME_NOT_FOUND){
        //
        // If the target device doesn't exist, we will register for ExCallback.
        // Our callback routine will be called at PASSIVE_LEVEL when the
        // target driver (NDISPROT) gets loaded and notifies the callback.
        //
        DEBUGP (MP_INFO, ( "Target device doesn't exist at this time %x\n", ntStatus ));

        #pragma warning(disable: 4127)

        if (NICRegisterExCallback(Adapter)) {
            ntStatus = STATUS_SUCCESS;
        }
        #pragma warning(default: 4127)

    }else if (ntStatus == STATUS_NO_MORE_ENTRIES){
        ntStatus = STATUS_SUCCESS;
    }

    NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);

    DEBUGP(MP_TRACE, ("<--- InitializeAdapter, Status=%x\n", Status));

    return Status;

}


VOID
NICInitWorkItemCallback(
    WDFWORKITEM WorkItem
    )
/*++
Routine Description:

    Workitme to retry our Initialization work as a result of notifiction
    from the target driver.

Arguments:

    WorkItem - Handle to workitem

Return Value:


--*/
{

    PMP_ADAPTER Adapter ;
    NTSTATUS ntStatus;

    DEBUGP(MP_TRACE, ("--> NICInitWorkItemCallback\n"));
    PAGED_CODE();

    Adapter = GetWdfDeviceInfo(WdfWorkItemGetParentObject(WorkItem))->Adapter;

    ntStatus = NICInitAdapterWorker(Adapter);

    if (NT_SUCCESS(ntStatus)){
        //
        // Let us indicate our media state to protocol drivers.
        //
        NdisMIndicateStatus(Adapter->AdapterHandle,
                                NDIS_STATUS_MEDIA_CONNECT,
                                (PVOID)0, 0);

        NdisMIndicateStatusComplete(Adapter->AdapterHandle);

    } else {

        if (ntStatus == STATUS_OBJECT_NAME_NOT_FOUND){
            //
            // This could be a spurious notification as a result
            // of another instance of our miniport registering
            // the callaback.
            //
            DEBUGP (MP_ERROR, ( "NDISPROT is not ready yet\n"));
         }else {
            //
            // I guess something irrecoverable happened. So let
            // us remove our adapter instance by calling.
            //
            DEBUGP (MP_ERROR, ( "Removing miniport...\n"));
            NdisMRemoveMiniport(Adapter->AdapterHandle);
         }
    }

    DEBUGP(MP_TRACE, ("<-- NICInitWorkItemCallback\n"));
    WdfObjectDelete(WorkItem);
    Adapter->ExecutiveCallbackWorkItem = NULL;
    return;
}

NTSTATUS
NICInitAdapterWorker(
    PMP_ADAPTER Adapter
    )
/*++
Routine Description:

    This is the worker routine for doing all the initialization work.
    It opens the target device, allocates send & receive side resources
    and spawns a worker thread to start receiving packets from the
    target device.

Arguments:

    Adapter     Pointer to our adapter

Return Value:

    NT Status code

--*/
{
    NTSTATUS            ntStatus = STATUS_UNSUCCESSFUL;
    NDIS_STATUS         Status;
    ULONG               unUsed;
    WDF_OBJECT_ATTRIBUTES attributes;

    DEBUGP(MP_WARNING, ("--> NICInitAdapterWorker %p\n", Adapter));

    PAGED_CODE();

    //
    // Let us make sure only one thread is executing this routine by
    // waiting on this synchronization event.
    //
    DEBUGP(MP_WARNING, ("Waiting on the InitEvent...\n"));
    NdisWaitEvent(&Adapter->InitEvent, 0);

    do {

        if (!MP_TEST_FLAG(Adapter, fMP_INIT_IN_PROGRESS)) {
            //
            // Adapter has been initialized successfully.
            //
            ntStatus = STATUS_SUCCESS;
            break;
        }

#ifdef INTERFACE_WITH_NDISPROT

        // 打开协议的接口。
        ntStatus = NICOpenNdisProtocolInterface(Adapter);
        if (!NT_SUCCESS(ntStatus)) {
            break;
        }

        Adapter->IsTargetSupportChainedMdls = FALSE;

#else
        //
        // To retrieve a handle to a device's local I/O target, the driver calls WdfDeviceGetIoTarget.
        //
        Adapter->IoTarget = WdfDeviceGetIoTarget(Adapter->WdfDevice);
        if (Adapter->IoTarget == NULL) {
            DEBUGP(MP_ERROR, ("WdfDeviceGetIoTarget failed \n"));
            break;
        }
#endif

        Status = NICAllocSendResources(Adapter);
        if (Status != NDIS_STATUS_SUCCESS)
        {
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            DEBUGP(MP_ERROR, ("Failed to allocate send side resources\n"));
            break;
        }

        Status = NICAllocRecvResources(Adapter);
        if (Status != NDIS_STATUS_SUCCESS)
        {
            ntStatus = STATUS_INSUFFICIENT_RESOURCES;
            DEBUGP(MP_ERROR, ("Failed to send side resources\n"));
            break;
        }

        ntStatus = NICSendRequest(Adapter,
                        NdisRequestQueryInformation,
                        OID_GEN_LINK_SPEED,
                        &Adapter->ulLinkSpeed,
                        sizeof(ULONG),
                        &unUsed,
                        &unUsed
                        );

        if (!NT_SUCCESS(ntStatus)){
            DEBUGP(MP_ERROR, ("-->Unable to get link speed %x\n", ntStatus));
            break;
        }

        //
        // If the device property is set to promiscuous, we will set the
        // target NIC to promiscuous mode and use a locally administered
        // MAC address. Otherwise, we will assume the MAC address of the
        // target NIC (We are assuming that user must have disabled the
        // binding of TCP/IP with the target NIC).
        //
        if (Adapter->Promiscuous) {

            ULONG PacketFilter = NDIS_PACKET_TYPE_PROMISCUOUS;

            DEBUGP(MP_TRACE, ("Set the physical NIC to promiscuous mode\n"));

            ntStatus = NICSendRequest(Adapter,
                            NdisRequestSetInformation,
                            OID_GEN_CURRENT_PACKET_FILTER,
                            &PacketFilter,
                            sizeof(ULONG),
                            &unUsed,
                            &unUsed
                            );

            if (!NT_SUCCESS(ntStatus)){
                DEBUGP(MP_ERROR, ("-->Unable to set the NIC to promiscuous mode %x\n", ntStatus));
                break;
            }
        } else {

            //
            // Then get the physical NICs MAC address and use that as our
            // MAC address. Hoping that user must have disabled TCP/IP
            // bindings with the physcial NIC.
            //

            ntStatus = NICSendRequest(Adapter,
                        NdisRequestQueryInformation,
                        OID_802_3_CURRENT_ADDRESS,
                        Adapter->PhyNicMacAddress,
                        ETH_LENGTH_OF_ADDRESS,
                        &unUsed,
                        &unUsed
                        );

            if (!NT_SUCCESS(ntStatus)){
                DEBUGP(MP_ERROR, ("-->Unable to get mac address %x\n", ntStatus));
                break;
            }

            ETH_COPY_NETWORK_ADDRESS(Adapter->CurrentAddress,
                                            Adapter->PhyNicMacAddress);

            DEBUGP(MP_WARNING, ("Current Address = %02x-%02x-%02x-%02x-%02x-%02x\n",
                                    Adapter->CurrentAddress[0],
                                    Adapter->CurrentAddress[1],
                                    Adapter->CurrentAddress[2],
                                    Adapter->CurrentAddress[3],
                                    Adapter->CurrentAddress[4],
                                    Adapter->CurrentAddress[5]));

        }

        MP_CLEAR_FLAG(Adapter, fMP_INIT_IN_PROGRESS);
        MP_CLEAR_FLAG(Adapter, fMP_DISCONNECTED);
        MP_SET_FLAG(Adapter, fMP_POST_WRITES);
        MP_SET_FLAG(Adapter, fMP_POST_READS);

        //
        // Finally schedule a workitme to start reading packets from
        // the target device. .
        //
        WdfWorkItemEnqueue(Adapter->ReadWorkItem);

        //
        // Post a request to get media change notification.
        //
        WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
        attributes.ParentObject = Adapter->IoTarget;


        ntStatus = WdfRequestCreate(
            WDF_NO_OBJECT_ATTRIBUTES,
            Adapter->IoTarget,
            &Adapter->StatusIndicationRequest);

        if (!NT_SUCCESS(ntStatus)) {
            DEBUGP(MP_ERROR, ("WdfRequestCreate for status-indication failed\n"));
            NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
            break;
        }

        WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
        attributes.ParentObject = Adapter->StatusIndicationRequest;

        ntStatus = WdfMemoryCreatePreallocated(&attributes,
                                    &Adapter->NdisProtIndicateStatus,
                                    sizeof(NDISPROT_INDICATE_STATUS),
                                    &Adapter->WdfStatusIndicationBuffer);
        if (!NT_SUCCESS(ntStatus)) {

            DEBUGP(MP_ERROR, ("WdfMemoryCreatePreallocated failed 0x%x\n",
                                        ntStatus));
            NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
            break;
        }


        ntStatus = NICPostAsynchronousStatusIndicationRequest(Adapter);
        if (!NT_SUCCESS(ntStatus)) {

            DEBUGP(MP_ERROR, ("NICPostAsynchronousStatusIndicationRequest failed 0x%x\n",
                                        ntStatus));
            NT_STATUS_TO_NDIS_STATUS(ntStatus, &Status);
            break;
        }

    } WHILE (FALSE);

    if (!NT_SUCCESS(ntStatus)){
        NICFreeSendResources(Adapter);
        NICFreeRecvResources(Adapter);
    }

    //
    // Signal the event so another thread can proceed.
    //
    NdisSetEvent(&Adapter->InitEvent);

    DEBUGP(MP_TRACE, ("<-- NICInitAdapterWorker %x\n", ntStatus));

    return ntStatus;
}

NTSTATUS
NICMakeSynchronousIoctl(
    IN WDFIOTARGET          IoTarget,
    IN PFILE_OBJECT         FileObject,
    IN ULONG                IoctlControlCode,
    IN OUT PVOID            InputBuffer,
    IN ULONG                InputBufferLength,
    IN OUT PVOID            OutputBuffer,
    IN ULONG                OutputBufferLength,
    OUT PULONG              BytesReadOrWritten
    )
/*++

Routine Description:

    This routine sends a synchronous ioctl request. This has been customized
    to handle only buffered IOCTLs. It also assumes that InputBuffer and
    OutputBuffer pointers are non paged kernelmode bufferr addresses. This routine
    can be called at IRQL <= DISPATCH_LEVEL.

Arguments:

    TopOfDeviceStack    - Pointer to the target deviceobject

    IoctlControlCode    - Value of the IOCTL request

    InputBuffer        - Buffer to be sent to the TopOfDeviceStack

    InputBufferLength  - Size of buffer to be sent to the TopOfDeviceStack

    OutputBuffer       - Buffer for received data from the TopOfDeviceStack

    OutputBufferLength - Size of receive buffer from the TopOfDeviceStack

    BytesReadOrWritten - Number of bytes read or written

Return Value:

    NT status code

--*/
{
    NTSTATUS                status;
    WDF_MEMORY_DESCRIPTOR   inputDesc, outputDesc;
    PWDF_MEMORY_DESCRIPTOR  pInputDesc = NULL, pOutputDesc = NULL;
    ULONG_PTR               bytesReturned;

    // FileObject这个参数未使用过。
    UNREFERENCED_PARAMETER(FileObject);

    // 生成输入缓冲描述符
    if (InputBuffer) {
        WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&inputDesc,
                                    InputBuffer,
                                    InputBufferLength);
        pInputDesc = &inputDesc;
    }

    // 生成输出缓冲描述符
    if (OutputBuffer) {
        WDF_MEMORY_DESCRIPTOR_INIT_BUFFER(&outputDesc,
                                    OutputBuffer,
                                    OutputBufferLength);
        pOutputDesc = &outputDesc;
    }

    // 发送请求。
    status = WdfIoTargetSendIoctlSynchronously(
                        IoTarget,
                        WDF_NO_HANDLE, // Request
                        IoctlControlCode,
                        pInputDesc,
                        pOutputDesc,
                        NULL, // PWDF_REQUEST_SEND_OPTIONS
                        &bytesReturned);
    if (!NT_SUCCESS(status)) {
        DEBUGP(MP_ERROR, ("WdfIoTargetSendIoctlSynchronously failed 0x%x\n", status));
    }

    // 记录返回的长度。
    *BytesReadOrWritten = (ULONG)bytesReturned;

    return status;
}


#ifdef INTERFACE_WITH_NDISPROT

NTSTATUS
NICOpenNdisProtocolInterface(
    PMP_ADAPTER Adapter
    )
/*++
Routine Description:

    This routine opens the NDISPROT device and sends buch of IOCTLs
    to get the names of NICs that it's bound to and pick the first
    available one as our target phycial device.

Arguments:

    Adapter     Pointer to our adapter

Return Value:

    NT Status code

--*/
{
    UCHAR           Buf[512];//should be enough to get name and description of one adapter
    ULONG           BufLength = sizeof(Buf);
    ULONG           i;
    NTSTATUS        status;
    PWCHAR          deviceName = NULL;
    PWCHAR          deviceDesc = NULL;
    UNICODE_STRING  fileName;
    ULONG           unUsed;
    PNDISPROT_QUERY_BINDING pQueryBinding = NULL;
    WDF_IO_TARGET_OPEN_PARAMS  openParams;

    PAGED_CODE();

    DEBUGP(MP_TRACE, ("--> NICOpenNdisProtocolInterface\n"));

    RtlInitUnicodeString ( &fileName, (PCWSTR) PROTOCOL_INTERFACE_NAME );

    status = WdfIoTargetCreate(Adapter->WdfDevice,
                        WDF_NO_OBJECT_ATTRIBUTES,
                        &Adapter->IoTarget);
    if (!NT_SUCCESS(status)) {
        DEBUGP(MP_ERROR, ("WdfIoTargetCreate failed 0x%x\n", status));
        return status;
    }

    WDF_IO_TARGET_OPEN_PARAMS_INIT_CREATE_BY_NAME(&openParams,
                                &fileName,
                                STANDARD_RIGHTS_ALL
                                );

    status = WdfIoTargetOpen(Adapter->IoTarget,
                        &openParams);
    if (!NT_SUCCESS(status)) {
        DEBUGP(MP_ERROR, ("WdfIoTargetOpen failed 0x%x\n", status));
        return status;
    }

    DEBUGP (MP_INFO, ( "Successfully opened the protocol interface\n"));

    //
    // Make sure the target device supports direct I/O operations.
    //
    if (!((WdfIoTargetWdmGetTargetDeviceObject(Adapter->IoTarget))->Flags
                            & DO_DIRECT_IO)) {
        ASSERTMSG("Target device doesn't support direct I/O\n", FALSE);
        return STATUS_INVALID_DEVICE_REQUEST;
    }

    Adapter->FileObject = WdfIoTargetWdmGetTargetFileObject(Adapter->IoTarget);

    //
    // Wait for the NDISPROT to completly bind to all the miniport
    // instances.
    //
    status = NICMakeSynchronousIoctl(
                Adapter->IoTarget,
                Adapter->FileObject,
                IOCTL_NDISPROT_BIND_WAIT,
                NULL,
                0,
                NULL,
                0,
                &unUsed
                );
    if (!NT_SUCCESS(status)) {
        DEBUGP(MP_ERROR, ("IOCTL_NDISIO_BIND_WAIT failed, error %x\n", status));
        return status;
    }

    pQueryBinding = (PNDISPROT_QUERY_BINDING)Buf;

    //
    // Query the binding of NDISPROT one at a time and see if you can open
    // the bindings and use that as our physical adapter.
    //
    i = 0;

    for (pQueryBinding->BindingIndex = i;
         /* NOTHING */;
         pQueryBinding->BindingIndex = ++i)
    {

        status = NICMakeSynchronousIoctl(
                Adapter->IoTarget,
                Adapter->FileObject,
                IOCTL_NDISPROT_QUERY_BINDING,
                pQueryBinding,
                sizeof(NDISPROT_QUERY_BINDING),
                Buf,
                BufLength,
                &unUsed
                );
        if (NT_SUCCESS(status))
        {
            deviceName = (PWCHAR)((PUCHAR)pQueryBinding +
                                    pQueryBinding->DeviceNameOffset);
            deviceDesc = (PWCHAR)((PUCHAR )pQueryBinding +
                                    pQueryBinding->DeviceDescrOffset);

            DEBUGP(MP_WARNING, ("%2d. %ws\n     - %ws\n",
                pQueryBinding->BindingIndex, deviceName, deviceDesc));

            //
            // Make sure we are not opening our device or another instance
            // of NDISWDM miniport if more than one instances is installed.
            // We can just use the AdapterDesc to decide this all the time.
            // There is no need to get devicename - just an illustration.
            //
            if (wcscmp(deviceName, Adapter->AdapterName) &&
                    !wcsstr(deviceDesc, Adapter->AdapterDesc))
            {

                Adapter->FileObject->FsContext = NULL;

                status = NICMakeSynchronousIoctl(
                            Adapter->IoTarget,
                            Adapter->FileObject,
                            IOCTL_NDISPROT_OPEN_DEVICE,
                            (PVOID)deviceName,
                            pQueryBinding->DeviceNameLength-sizeof(WCHAR),
                            NULL,
                            0,
                            &unUsed
                            );
                if (!NT_SUCCESS(status)) {
                    DEBUGP(MP_ERROR, ("Failed to open NDIS Device %ws %x\n", deviceDesc, status));
                } else {
                    DEBUGP(MP_WARNING, ("Successfully opened NDIS Device: %ws\n", deviceDesc));
                    break;
                }
            }
            RtlZeroMemory(Buf, BufLength);
        }
        else
        {
            if (status != STATUS_NO_MORE_ENTRIES)
            {
                DEBUGP(MP_ERROR, ("EnumerateDevices: terminated abnormally, error %x\n", status));
            }
            break;
        }
    }

    DEBUGP(MP_TRACE, ("<-- NICOpenNdisProtocolInterface\n"));

    return status;
}

#endif

NTSTATUS
NICPostAsynchronousStatusIndicationRequest(
    IN PMP_ADAPTER Adapter
)
/*++

Routine Description:

    This routine sends an asynchronous ioctl request to NDISPROT.
    This ioctl is used to get the status information indicated
    by the real NIC. This IOCTL can be cancelled anytime after
    the call is made by calling NICCancelStatusIndicationRequest.
    This rotuine is called only once by the driver. If this were
    to be called multiple times, some synchronization code should
    be added to protect the status indication related field in
    the context structure.

Arguments:

    Adapter     Pointer to our adapter

Return Value:

    NT status code

--*/
{
    WDFREQUEST                  request = Adapter->StatusIndicationRequest;
    WDF_REQUEST_REUSE_PARAMS    params;
    NTSTATUS                    status;

    DEBUGP(MP_TRACE, ("--> NICPostAsynchronousStatusIndicationRequest\n"));

    WDF_REQUEST_REUSE_PARAMS_INIT(&params, WDF_REQUEST_REUSE_NO_FLAGS, STATUS_SUCCESS);

    status = WdfRequestReuse(request, &params);
    if (!NT_SUCCESS(status)){
        DEBUGP(MP_ERROR, ("WdfRequestReuse failed 0x%x\n", status));
        ASSERT(FALSE);
        return status;
    }

    status = WdfIoTargetFormatRequestForIoctl(
                                Adapter->IoTarget,
                                request,
                                IOCTL_NDISPROT_INDICATE_STATUS,
                                NULL, // InputBuffer
                                NULL, // offsets
                                Adapter->WdfStatusIndicationBuffer,
                                NULL); // OutputBufferOffset
   if (!NT_SUCCESS(status)) {
        DEBUGP(MP_ERROR, ("WdfIoTargetFormatRequestForIoctl failed 0x%x\n", status));
        return status;
    }

    WdfRequestSetCompletionRoutine(request,
                   NICStatusIndicationRequestCompletionRoutine,
                   Adapter);

    if (WdfRequestSend(request, Adapter->IoTarget, WDF_NO_SEND_OPTIONS) == FALSE) {
        status = WdfRequestGetStatus(request);
    }

    DEBUGP(MP_TRACE, ("<-- NICPostAsynchronousStatusIndicationRequest\n"));

    return status;

}

VOID
NICStatusIndicationRequestCompletionRoutine(
    IN WDFREQUEST                  Request,
    IN WDFIOTARGET                 Target,
    PWDF_REQUEST_COMPLETION_PARAMS CompletionParams,
    IN WDFCONTEXT                  Context
    )
/*++

Routine Description:

    This is completion routine for status indication Request.
    This routine carefully makes sure with an interlocked operation
    whether the Request has been cancelled by another thread and if so
    return control. If not processes the indicate_status buffer and
    forwards the indication to the protocol bound to this NIC.

Arguments:

    Adapter     Pointer to our adapter

Return Value:

    NT status code

--*/
{
    PMP_ADAPTER Adapter = (PMP_ADAPTER)Context;
    PNDISPROT_INDICATE_STATUS pIndicateStatus = &Adapter->NdisProtIndicateStatus;
    PCHAR   buffer = (PCHAR)pIndicateStatus;
    BOOLEAN repost = FALSE;
    NTSTATUS    status;

    UNREFERENCED_PARAMETER(Target);
    UNREFERENCED_PARAMETER(Request);

    DEBUGP(MP_TRACE, ("--> NICStatusIndicationRequestCompletionRoutine\n"));

    status = CompletionParams->IoStatus.Status;

    //
    // We have the ownership of the Request
    //
    if (!NT_SUCCESS(status)) {
        //
        // The IOCTL was failed for some reason. If the reason
        // is due to buffer overflow, we should increase our
        // buffer and retry the operation. Well, in this driver
        // since I'm only interested in getting media indication,
        // I'm going to leave that as TODO item for developers
        // to work on depending on their real world scenario.
        //
        DEBUGP (MP_ERROR, ("IndicationRequest failed %x, %d \n",
                status,
                CompletionParams->IoStatus.Information));
    } else {

        //
        // Find out the status and update our Adapter context to
        // be in sync with the real NIC.
        //
        switch (pIndicateStatus->IndicatedStatus)
        {
            case NDIS_STATUS_RESET_START:
                DEBUGP(MP_LOUD, ("NDIS_STATUS_RESET_START\n"));
                break;

            case NDIS_STATUS_RESET_END:
                DEBUGP(MP_LOUD, ("NDIS_STATUS_RESET_END\n"));

                break;

            case NDIS_STATUS_MEDIA_CONNECT:
                DEBUGP(MP_TRACE, ("NDIS_STATUS_MEDIA_CONNECT\n"));
                MP_CLEAR_FLAG(Adapter, fMP_DISCONNECTED);
                break;

            case NDIS_STATUS_MEDIA_DISCONNECT:
                DEBUGP(MP_TRACE, ("NDIS_STATUS_MEDIA_DISCONNECT\n"));
                MP_SET_FLAG(Adapter, fMP_DISCONNECTED);
                break;

            default:
                DEBUGP(MP_INFO, ("IndicatedStatus %x\n",
                        pIndicateStatus->IndicatedStatus));
                break;
        }

        //
        // Let us indicate our media state to protocol drivers.
        //
        NdisMIndicateStatus(Adapter->AdapterHandle,
                            pIndicateStatus->IndicatedStatus,
                            (PVOID)(buffer + (pIndicateStatus->StatusBufferOffset)),
                            pIndicateStatus->StatusBufferLength);

        NdisMIndicateStatusComplete(Adapter->AdapterHandle);

        repost = TRUE;
    }

    if (repost){
        //
        // TODO:
        // Repost the request to the NDISPROT driver. Since I know that NDISPROT
        // always completes the Request asynchronously, I don't have to worry about
        // recursion. But if you know that the target driver can complete the
        // Request in the dispatch routine, reposting the Request can cause recursion. SO
        // queue a workitem here and do a IoCallDriver from there. Please note
        // that this completion routine can be called at DISPATCH_LEVEL. So
        // before reposting to the target driver, make sure it can handle
        // requests at dispatch level.
        //
        NICPostAsynchronousStatusIndicationRequest(Adapter);
    }

    DEBUGP(MP_TRACE, ("<-- NICStatusIndicationRequestCompletionRoutine\n"));

    return ;
}


