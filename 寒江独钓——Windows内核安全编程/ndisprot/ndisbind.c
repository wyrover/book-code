/*++

Copyright (c) 2000  Microsoft Corporation

Module Name:

    ndisbind.c

Abstract:

    NDIS protocol entry points and utility routines to handle binding
    and unbinding from adapters.

Environment:

    Kernel mode only.

Revision History:

--*/


#include "precomp.h"

#define __FILENUMBER 'DNIB'

VOID
NdisProtBindAdapter(
    OUT PNDIS_STATUS                pStatus,
    IN NDIS_HANDLE                  BindContext,
    IN PNDIS_STRING                 pDeviceName,
    IN PVOID                        SystemSpecific1,
    IN PVOID                        SystemSpecific2
    )
/*++

Routine Description:

    Protocol Bind Handler entry point called when NDIS wants us
    to bind to an adapter. We go ahead and set up a binding.
    An OPEN_CONTEXT structure is allocated to keep state about
    this binding.

Arguments:

    pStatus - place to return bind status
    BindContext - handle to use with NdisCompleteBindAdapter
    DeviceName - adapter to bind to
    SystemSpecific1 - used to access protocol-specific registry
                 key for this binding
    SystemSpecific2 - unused

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT           pOpenContext;
    NDIS_STATUS                     Status, ConfigStatus;
    NDIS_HANDLE                     ConfigHandle;

    UNREFERENCED_PARAMETER(BindContext);
    UNREFERENCED_PARAMETER(SystemSpecific2);
    
    do
    {
        //
        //  Allocate our context for this open.
        //

        // 分配空间给每个打开上下文。所谓打开上下文就是每次绑定，
        // 用户分配的一片空间，用来保存这次绑定相关的信息。这里
        // 用宏NPROT_ALLOC_MEM分配内存是为了调试的方便。实际上本
        // 质是用NdisAllocateMemoryWithTag分配空间。读者如果用
        // ExAllocatePoolWithTag代替也是可行的。只是要注意必须是
        // Nonpaged空间。
        NPROT_ALLOC_MEM(pOpenContext, sizeof(NDISPROT_OPEN_CONTEXT));
        if (pOpenContext == NULL)
        {
            Status = NDIS_STATUS_RESOURCES;
            break;
        }

        // 内存清0。同样用宏。实际上用的NdisZeroMemory。 
        NPROT_ZERO_MEM(pOpenContext, sizeof(NDISPROT_OPEN_CONTEXT));

        // 给这个空间写一个特征数据便于识别判错。
        NPROT_SET_SIGNATURE(pOpenContext, oc);

        // 初始化几个用到的数据成员。锁、读队列、写对队列、包队列
        // 电源打开事件
        NPROT_INIT_LOCK(&pOpenContext->Lock);
        NPROT_INIT_LIST_HEAD(&pOpenContext->PendedReads);
        NPROT_INIT_LIST_HEAD(&pOpenContext->PendedWrites);
        NPROT_INIT_LIST_HEAD(&pOpenContext->RecvPktQueue);
        NPROT_INIT_EVENT(&pOpenContext->PoweredUpEvent);

        //
        //  Start off by assuming that the device below is powered up.
        //

        // 认为开始的时候电源是打开的。
        NPROT_SIGNAL_EVENT(&pOpenContext->PoweredUpEvent);

        //
        //  Determine the platform we are running on.
        //

        // 下面开始检测我们运行在什么平台。首先假定是Win9x.
        // 但是为了去掉多余的部分，实际上我已经去掉了对Win9x
        // 的支持。所以下面这一段已经没有意义了。但是下面的
        // 代码依然有参考价值。实际上是在读取注册表的配置。
        //pOpenContext->bRunningOnWin9x = TRUE;

        //NdisOpenProtocolConfiguration(
        //    &ConfigStatus,
        //    &ConfigHandle,
        //    (PNDIS_STRING)SystemSpecific1);
        //
        //if (ConfigStatus == NDIS_STATUS_SUCCESS)
        //{
        //    PNDIS_CONFIGURATION_PARAMETER   pParameter;
        //    NDIS_STRING                     VersionKey = NDIS_STRING_CONST("Environment");

        //    NdisReadConfiguration(
        //        &ConfigStatus,
        //        &pParameter,
        //        ConfigHandle,
        //        &VersionKey,
        //        NdisParameterInteger);
        //    
        //    if ((ConfigStatus == NDIS_STATUS_SUCCESS) &&
        //        ((pParameter->ParameterType == NdisParameterInteger) ||
        //         (pParameter->ParameterType == NdisParameterHexInteger)))
        //    {
        //        pOpenContext->bRunningOnWin9x =
        //            (pParameter->ParameterData.IntegerData == NdisEnvironmentWindows);
        //    }

        //    NdisCloseConfiguration(ConfigHandle);
        //}

        NPROT_REF_OPEN(pOpenContext); 

        //
        //  Add it to the global list.
        //

        // 因为打开上下文已经被分配好。所以这里将这个打开上下文
        // 保存到全局链表里以便日后检索。注意这个操作要加锁。实
         // 际上这里用的就是读者前面学过的自旋锁。
        NPROT_ACQUIRE_LOCK(&Globals.GlobalLock);
        NPROT_INSERT_TAIL_LIST(&Globals.OpenList,
                             &pOpenContext->Link);
        NPROT_RELEASE_LOCK(&Globals.GlobalLock);

        // 正式的绑定过程。
        Status = ndisprotCreateBinding(
                     pOpenContext,
                     (PUCHAR)pDeviceName->Buffer,
                     pDeviceName->Length);
        
        if (Status != NDIS_STATUS_SUCCESS)
        {
            break;
        }
    }
    while (FALSE);

    *pStatus = Status;

    return;
}


VOID
NdisProtOpenAdapterComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN NDIS_STATUS                  Status,
    IN NDIS_STATUS                  OpenErrorCode
    )
/*++

Routine Description:

    Completion routine called by NDIS if our call to NdisOpenAdapter
    pends. Wake up the thread that called NdisOpenAdapter.

Arguments:

    ProtocolBindingContext - pointer to open context structure
    Status - status of the open
    OpenErrorCode - if unsuccessful, additional information

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT           pOpenContext;

    UNREFERENCED_PARAMETER(OpenErrorCode);
    
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    pOpenContext->BindStatus = Status;

    NPROT_SIGNAL_EVENT(&pOpenContext->BindEvent);
}


VOID
NdisProtUnbindAdapter(
    OUT PNDIS_STATUS                pStatus,
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN NDIS_HANDLE                  UnbindContext
    )
/*++

Routine Description:

    NDIS calls this when it wants us to close the binding to an adapter.

Arguments:

    pStatus - place to return status of Unbind
    ProtocolBindingContext - pointer to open context structure
    UnbindContext - to use in NdisCompleteUnbindAdapter if we return pending

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT           pOpenContext;

    UNREFERENCED_PARAMETER(UnbindContext);
    
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    //
    //  Mark this open as having seen an Unbind.
    //
    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

    NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_UNBIND_FLAGS, NUIOO_UNBIND_RECEIVED);

    //
    //  In case we had threads blocked for the device below to be powered
    //  up, wake them up.
    //
    NPROT_SIGNAL_EVENT(&pOpenContext->PoweredUpEvent);

    NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    ndisprotShutdownBinding(pOpenContext);

    *pStatus = NDIS_STATUS_SUCCESS;
    return;
}


VOID
NdisProtCloseAdapterComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN NDIS_STATUS                  Status
    )
/*++

Routine Description:

    Called by NDIS to complete a pended call to NdisCloseAdapter.
    We wake up the thread waiting for this completion.

Arguments:

    ProtocolBindingContext - pointer to open context structure
    Status - Completion status of NdisCloseAdapter

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT           pOpenContext;

    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    pOpenContext->BindStatus = Status;

    NPROT_SIGNAL_EVENT(&pOpenContext->BindEvent);
}

    
NDIS_STATUS
NdisProtPnPEventHandler(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN PNET_PNP_EVENT               pNetPnPEvent
    )
/*++

Routine Description:

    Called by NDIS to notify us of a PNP event. The most significant
    one for us is power state change.

Arguments:

    ProtocolBindingContext - pointer to open context structure
                this is NULL for global reconfig events.

    pNetPnPEvent - pointer to the PNP event

Return Value:

    Our processing status for the PNP event.

--*/
{
    PNDISPROT_OPEN_CONTEXT           pOpenContext;
    NDIS_STATUS                     Status;

    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;

    switch (pNetPnPEvent->NetEvent)
    {
        case NetEventSetPower:
            NPROT_STRUCT_ASSERT(pOpenContext, oc);
            pOpenContext->PowerState = *(PNET_DEVICE_POWER_STATE)pNetPnPEvent->Buffer;

            if (pOpenContext->PowerState > NetDeviceStateD0)
            {
                //
                //  The device below is transitioning to a low power state.
                //  Block any threads attempting to query the device while
                //  in this state.
                //
                NPROT_INIT_EVENT(&pOpenContext->PoweredUpEvent);

                //
                //  Wait for any I/O in progress to complete.
                //
                ndisprotWaitForPendingIO(pOpenContext, FALSE);

                //
                //  Return any receives that we had queued up.
                //
                ndisprotFlushReceiveQueue(pOpenContext);
                DEBUGP(DL_INFO, ("PnPEvent: Open %p, SetPower to %d\n",
                    pOpenContext, pOpenContext->PowerState));
            }
            else
            {
                //
                //  The device below is powered up.
                //
                DEBUGP(DL_INFO, ("PnPEvent: Open %p, SetPower ON: %d\n",
                    pOpenContext, pOpenContext->PowerState));
                NPROT_SIGNAL_EVENT(&pOpenContext->PoweredUpEvent);
            }

            Status = NDIS_STATUS_SUCCESS;
            break;

        case NetEventQueryPower:
            Status = NDIS_STATUS_SUCCESS;
            break;

        case NetEventBindsComplete:
            
            NPROT_SIGNAL_EVENT(&Globals.BindsComplete);
            if(!ndisprotRegisterExCallBack()){
                DEBUGP(DL_ERROR, ("DriverEntry: ndisprotRegisterExCallBack failed\n"));
            }                    
            Status = NDIS_STATUS_SUCCESS;
            break;

        case NetEventQueryRemoveDevice:
        case NetEventCancelRemoveDevice:
        case NetEventReconfigure:
        case NetEventBindList:
        case NetEventPnPCapabilities:
            Status = NDIS_STATUS_SUCCESS;
            break;

        default:
            Status = NDIS_STATUS_NOT_SUPPORTED;
            break;
    }

    DEBUGP(DL_INFO, ("PnPEvent: Open %p, Event %d, Status %x\n",
            pOpenContext, pNetPnPEvent->NetEvent, Status));

    return (Status);
}
    
VOID
NdisProtProtocolUnloadHandler(
    VOID
    )
/*++

Routine Description:

    NDIS calls this on a usermode request to uninstall us.

Arguments:

    None

Return Value:

    None

--*/
{
    ndisprotDoProtocolUnload();
}

NDIS_STATUS
ndisprotCreateBinding(
    IN PNDISPROT_OPEN_CONTEXT                   pOpenContext,
    __in_bcount(BindingInfoLength) IN PUCHAR    pBindingInfo,
    IN ULONG                                    BindingInfoLength
    )
{
    NDIS_STATUS Status;
    NDIS_STATUS  OpenErrorCode;
    NDIS_MEDIUM  MediumArray[1] = {NdisMedium802_3};
    UINT  SelectedMediumIndex;
    PNDISPROT_OPEN_CONTEXT  pTmpOpenContext;
    BOOLEAN  fDoNotDisturb = FALSE;
    BOOLEAN  fOpenComplete = FALSE;
    ULONG  BytesProcessed;
    ULONG GenericUlong = 0;

    // 输出一句调试信息。
    DEBUGP(DL_LOUD, ("CreateBinding: open %p/%x, device [%ws]\n",
                pOpenContext, pOpenContext->Flags, pBindingInfo));
    Status = NDIS_STATUS_SUCCESS;

    do
    {
        // 检查看看是否已经绑定了这个网卡。如果已经绑定的话，就没有必
        // 要再次绑定了，直接返回成功即可。请注意，ndisprotLookupDevice
        // 会给这个打开上下文增加一个引用。
        pTmpOpenContext = ndisprotLookupDevice(pBindingInfo, BindingInfoLength);

        // 如果没有找到的话，就返回NULL了。
        if (pTmpOpenContext != NULL)
        {
            DEBUGP(DL_WARN,
                ("CreateBinding: Binding to device %ws already exists on open %p\n",
                    pTmpOpenContext->DeviceName.Buffer, pTmpOpenContext));

            // 减少这个打开上下文的一个引用。
            NPROT_DEREF_OPEN(pTmpOpenContext); 
            Status = NDIS_STATUS_FAILURE;
            break;
        }

        // 获得锁。为什么这里要用锁？这是因为只有对空闲的打开上下文
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        // 通过标记来检查...如果绑定标志不是空闲状态，或者解除绑定信
        // 息收到了解除绑定的要求，那就直接返回失败。
        if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_IDLE) ||
            NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_UNBIND_FLAGS, NUIOO_UNBIND_RECEIVED))
        {
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);

            Status = NDIS_STATUS_NOT_ACCEPTED;
            // 设置了标记，表示
            fDoNotDisturb = TRUE;
            break;
        }

        // 设置标记，表示我们已经开始绑定了。别人勿操作它。
        NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_OPENING);

        // 释放锁。
        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        // 分配名字。到这里开始绑定了。先分配设备名字符串。
        NPROT_ALLOC_MEM(pOpenContext->DeviceName.Buffer, BindingInfoLength + sizeof(WCHAR));
        if (pOpenContext->DeviceName.Buffer == NULL)
        {
            DEBUGP(DL_WARN, ("CreateBinding: failed to alloc device name buf (%d bytes)\n",
                BindingInfoLength + sizeof(WCHAR)));
            Status = NDIS_STATUS_RESOURCES;
            break;
        }

        // 从pBindingInfo中把字符串拷贝出来。
        NPROT_COPY_MEM(pOpenContext->DeviceName.Buffer, pBindingInfo, BindingInfoLength);
#pragma prefast(suppress: 12009, "DeviceName length will not cause overflow")                   
        *(PWCHAR)((PUCHAR)pOpenContext->DeviceName.Buffer + BindingInfoLength) = L'\0';        
        NdisInitUnicodeString(&pOpenContext->DeviceName, pOpenContext->DeviceName.Buffer);


        // 分配包池。用来做发送缓冲区，容纳将要发送出去的包。
        NdisAllocatePacketPoolEx(
            &Status,
            &pOpenContext->SendPacketPool,
            MIN_SEND_PACKET_POOL_SIZE,
            MAX_SEND_PACKET_POOL_SIZE - MIN_SEND_PACKET_POOL_SIZE,
            sizeof(NPROT_SEND_PACKET_RSVD));
       
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: failed to alloc"
                    " send packet pool: %x\n", Status));
            break;
        }

        // 分配包池，用来容纳
        NdisAllocatePacketPoolEx(
            &Status,
            &pOpenContext->RecvPacketPool,
            MIN_RECV_PACKET_POOL_SIZE,
            MAX_RECV_PACKET_POOL_SIZE - MIN_RECV_PACKET_POOL_SIZE,
            sizeof(NPROT_RECV_PACKET_RSVD));
       
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: failed to alloc"
                    " recv packet pool: %x\n", Status));
            break;
        }

        // 包池。用来做接收缓冲区。
        NdisAllocateBufferPool(
            &Status,
            &pOpenContext->RecvBufferPool,
            MAX_RECV_PACKET_POOL_SIZE);
        
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: failed to alloc"
                    " recv buffer pool: %x\n", Status));
            break;
        }

        // 电源状态是打开着的。
        pOpenContext->PowerState = NetDeviceStateD0;

        // 初始化一个打开事件。（打开就是绑定！）
        NPROT_INIT_EVENT(&pOpenContext->BindEvent);

        NdisOpenAdapter(
            &Status,
            &OpenErrorCode,
            &pOpenContext->BindingHandle,
            &SelectedMediumIndex,
            &MediumArray[0],
            sizeof(MediumArray) / sizeof(NDIS_MEDIUM),
            Globals.NdisProtocolHandle,
            (NDIS_HANDLE)pOpenContext,
            &pOpenContext->DeviceName,
            0,
            NULL);
    
        // 等待请求完成。
        if (Status == NDIS_STATUS_PENDING)
        {
            NPROT_WAIT_EVENT(&pOpenContext->BindEvent, 0);
            Status = pOpenContext->BindStatus;
        }

        // 如果不成功
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: NdisOpenAdapter (%ws) failed: %x\n",
                pOpenContext->DeviceName.Buffer, Status));
            break;
        }

        // 记住我们已经成功的绑定了。但是我们还没有更新打开
        // 状态。这是为了避免别的线程开始关闭这个绑定。
        fOpenComplete = TRUE;

        // 发请求，获得一个可阅读的名字。不过这并不是非
        // 成功不可的。所以不检查返回值。
        (VOID)NdisQueryAdapterInstanceName(
                &pOpenContext->DeviceDescr,
                pOpenContext->BindingHandle
                );

        // 获得下面网卡的Mac地址
        Status = ndisprotDoRequest(
                    pOpenContext,
                    NdisRequestQueryInformation,
                    OID_802_3_CURRENT_ADDRESS,
                    &pOpenContext->CurrentAddress[0],
                    NPROT_MAC_ADDR_LEN,
                    &BytesProcessed
                    );
        
        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: qry current address failed: %x\n",
                    Status));
            break;
        }
        
        // 获得网卡选项
        Status = ndisprotDoRequest(
                    pOpenContext,
                    NdisRequestQueryInformation,
                    OID_GEN_MAC_OPTIONS,
                    &pOpenContext->MacOptions,
                    sizeof(pOpenContext->MacOptions),
                    &BytesProcessed
                    );

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: qry MAC options failed: %x\n",
                    Status));
            break;
        }

        // 获得最大帧长
        Status = ndisprotDoRequest(
                    pOpenContext,
                    NdisRequestQueryInformation,
                    OID_GEN_MAXIMUM_FRAME_SIZE,
                    &pOpenContext->MaxFrameSize,
                    sizeof(pOpenContext->MaxFrameSize),
                    &BytesProcessed
                    );

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: qry max frame failed: %x\n",
                    Status));
            break;
        }

        // 获得下层连接状态。
        Status = ndisprotDoRequest(
                    pOpenContext,
                    NdisRequestQueryInformation,
                    OID_GEN_MEDIA_CONNECT_STATUS,
                    &GenericUlong,
                    sizeof(GenericUlong),
                    &BytesProcessed
                    );

        if (Status != NDIS_STATUS_SUCCESS)
        {
            DEBUGP(DL_WARN, ("CreateBinding: qry media connect status failed: %x\n",
                    Status));
            break;
        }

        if (GenericUlong == NdisMediaStateConnected)
        {
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_MEDIA_FLAGS, NUIOO_MEDIA_CONNECTED);
        }
        else
        {
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_MEDIA_FLAGS, NUIOO_MEDIA_DISCONNECTED);
        }


        // 设置标记
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE);

        // 检测是否这时候出现了一个解除绑定请求
        if (NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_UNBIND_FLAGS, NUIOO_UNBIND_RECEIVED))
        {
            // 出现了则这次绑定失败
            Status = NDIS_STATUS_FAILURE;
        }

        // 标记测试完之后就可以解锁了。
        NPROT_RELEASE_LOCK(&pOpenContext->Lock);
       
    }
    while (FALSE);

    // 如果没有成功，而且
    if ((Status != NDIS_STATUS_SUCCESS) && !fDoNotDisturb)
    {
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        // 如果是已经成功的绑定了
        if (fOpenComplete)
        {
            // 如果已经绑定结束了，设置已经绑定标记。
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE);
        }
        else if (NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_OPENING))
        {
            // 如果是正在绑定过程中，设置绑定失败了。
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_FAILED);
        }
        // 释放锁。
        NPROT_RELEASE_LOCK(&pOpenContext->Lock);
        // 调用停止绑定函数。这里会释放所有资源。
        ndisprotShutdownBinding(pOpenContext);
    }

    DEBUGP(DL_INFO, ("CreateBinding: OpenContext %p, Status %x\n",
            pOpenContext, Status));

    return (Status);
}

VOID
ndisprotShutdownBinding(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext
    )
{
    NDIS_STATUS Status;
    BOOLEAN DoCloseBinding = FALSE;

    do
    {
        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        // 检查标记，如果是正在打开的话，立刻退出。
        // 放弃解除绑定操作。
        if (NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_OPENING))
        {
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            break;
        }

        // 如果是绑定已经完成的情况，则设置为开始解除绑定。
        // 其他的情况则根本不用做下面的操作。因为绑定既然
        // 没有完成，也当然不用解除绑定。
        if (NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
        {
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_CLOSING);
            DoCloseBinding = TRUE;
        }

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        if (DoCloseBinding)
        {
            // 从这里开始解除绑定。
            ULONG    PacketFilter = 0;
            ULONG    BytesRead = 0;
            
            // 把绑定了的网卡的包过滤器设置为0，换句话说，就是从现在
            // 开始停止收包。这是为了便于清理资源。
            Status = ndisprotDoRequest(
                        pOpenContext,
                        NdisRequestSetInformation,
                        OID_GEN_CURRENT_PACKET_FILTER,
                        &PacketFilter,
                        sizeof(PacketFilter),
                        &BytesRead);

            if (Status != NDIS_STATUS_SUCCESS)
            {
                DEBUGP(DL_WARN, ("ShutDownBinding: set packet filter failed: %x\n", Status));
            }
            
            // 把这个网卡的广播列表设置为NULL.
            Status = ndisprotDoRequest(
                        pOpenContext,
                        NdisRequestSetInformation,
                        OID_802_3_MULTICAST_LIST,
                        NULL,
                        0,
                        &BytesRead);

            if (Status != NDIS_STATUS_SUCCESS)
            {
                DEBUGP(DL_WARN, ("ShutDownBinding: set multicast list failed: %x\n", Status));
            }

            // 取消所有的提交状态IRP。
            ndisServiceIndicateStatusIrp(pOpenContext,
                                0,
                                NULL,
                                0,
                                TRUE);

            // 等待所有未决IRP完成。
            ndisprotWaitForPendingIO(pOpenContext, TRUE);

            // 清理掉接收队列中所有的包。
            ndisprotFlushReceiveQueue(pOpenContext);

            // 初始化绑定事件，这个时间将用来等待解除绑定完成。
            NPROT_INIT_EVENT(&pOpenContext->BindEvent);

            DEBUGP(DL_INFO, ("ShutdownBinding: Closing OpenContext %p,"
                    " BindingHandle %p\n",
                    pOpenContext, pOpenContext->BindingHandle));
            
            // 正式调用解除绑定。
            NdisCloseAdapter(&Status, pOpenContext->BindingHandle);
            if (Status == NDIS_STATUS_PENDING)
            {
                NPROT_WAIT_EVENT(&pOpenContext->BindEvent, 0);
                Status = pOpenContext->BindStatus;
            }

            NPROT_ASSERT(Status == NDIS_STATUS_SUCCESS);

            pOpenContext->BindingHandle = NULL;
        }

        if (DoCloseBinding)
        {
            // 设置上已经解除绑定的标记。
            NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_IDLE);
            NPROT_SET_FLAGS(pOpenContext->Flags, NUIOO_UNBIND_FLAGS, 0);
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        }

        // 下面的操作主要是释放资源。
        NPROT_ACQUIRE_LOCK(&Globals.GlobalLock);
        NPROT_REMOVE_ENTRY_LIST(&pOpenContext->Link);
        NPROT_RELEASE_LOCK(&Globals.GlobalLock);
        ndisprotFreeBindResources(pOpenContext);
        NPROT_DEREF_OPEN(pOpenContext);  // Shutdown binding
    }
    while (FALSE);
}


VOID
ndisprotFreeBindResources(
    IN PNDISPROT_OPEN_CONTEXT       pOpenContext
    )
/*++

Routine Description:

    Free any resources set up for an NDIS binding.

Arguments:

    pOpenContext - pointer to open context block

Return Value:

    None

--*/
{
    if (pOpenContext->SendPacketPool != NULL)
    {
        NdisFreePacketPool(pOpenContext->SendPacketPool);
        pOpenContext->SendPacketPool = NULL;
    }

    if (pOpenContext->RecvPacketPool != NULL)
    {
        NdisFreePacketPool(pOpenContext->RecvPacketPool);
        pOpenContext->RecvPacketPool = NULL;
    }

    if (pOpenContext->RecvBufferPool != NULL)
    {
        NdisFreeBufferPool(pOpenContext->RecvBufferPool);
        pOpenContext->RecvBufferPool = NULL;
    }

    if (pOpenContext->SendBufferPool != NULL)
    {
        NdisFreeBufferPool(pOpenContext->SendBufferPool);
        pOpenContext->SendBufferPool = NULL;
    }

    if (pOpenContext->DeviceName.Buffer != NULL)
    {
        NPROT_FREE_MEM(pOpenContext->DeviceName.Buffer);
        pOpenContext->DeviceName.Buffer = NULL;
        pOpenContext->DeviceName.Length =
        pOpenContext->DeviceName.MaximumLength = 0;
    }

    if (pOpenContext->DeviceDescr.Buffer != NULL)
    {
        //
        // this would have been allocated by NdisQueryAdpaterInstanceName.
        //
        NdisFreeMemory(pOpenContext->DeviceDescr.Buffer, 0, 0);
        pOpenContext->DeviceDescr.Buffer = NULL;
    }
}


VOID
ndisprotWaitForPendingIO(
    IN PNDISPROT_OPEN_CONTEXT            pOpenContext,
    IN BOOLEAN                          DoCancelReads
    )
/*++

Routine Description:

    Utility function to wait for all outstanding I/O to complete
    on an open context. It is assumed that the open context
    won't go away while we are in this routine.

Arguments:

    pOpenContext - pointer to open context structure
    DoCancelReads - do we wait for pending reads to go away (and cancel them)?

Return Value:

    None

--*/
{
    NDIS_STATUS     Status;
    ULONG           LoopCount;
    ULONG           PendingCount;

#ifdef NDIS51
    //
    //  Wait for any pending sends or requests on the binding to complete.
    //
    for (LoopCount = 0; LoopCount < 60; LoopCount++)
    {
        Status = NdisQueryPendingIOCount(
                    pOpenContext->BindingHandle,
                    &PendingCount);

        if ((Status != NDIS_STATUS_SUCCESS) ||
            (PendingCount == 0))
        {
            break;
        }

        DEBUGP(DL_INFO, ("WaitForPendingIO: Open %p, %d pending I/O at NDIS\n",
                pOpenContext, PendingCount));

        NPROT_SLEEP(2);
    }

    NPROT_ASSERT(LoopCount < 60);

#endif // NDIS51

    //
    //  Make sure any threads trying to send have finished.
    //
    for (LoopCount = 0; LoopCount < 60; LoopCount++)
    {
        if (pOpenContext->PendedSendCount == 0)
        {
            break;
        }

        DEBUGP(DL_WARN, ("WaitForPendingIO: Open %p, %d pended sends\n",
                pOpenContext, pOpenContext->PendedSendCount));

        NPROT_SLEEP(1);
    }

    NPROT_ASSERT(LoopCount < 60);

    if (DoCancelReads)
    {
        //
        //  Wait for any pended reads to complete/cancel.
        //
        while (pOpenContext->PendedReadCount != 0)
        {
            DEBUGP(DL_INFO, ("WaitForPendingIO: Open %p, %d pended reads\n",
                pOpenContext, pOpenContext->PendedReadCount));

            //
            //  Cancel any pending reads.
            //
            ndisprotCancelPendingReads(pOpenContext);

            NPROT_SLEEP(1);
        }
    }

}


VOID
ndisprotDoProtocolUnload(
    VOID
    )
/*++

Routine Description:

    Utility routine to handle unload from the NDIS protocol side.

Arguments:

    None

Return Value:

    None

--*/
{
    NDIS_HANDLE     ProtocolHandle;
    NDIS_STATUS     Status;

    DEBUGP(DL_INFO, ("ProtocolUnload: ProtocolHandle %lx\n", 
        Globals.NdisProtocolHandle));

    if (Globals.NdisProtocolHandle != NULL)
    {
        ProtocolHandle = Globals.NdisProtocolHandle;
        Globals.NdisProtocolHandle = NULL;

        NdisDeregisterProtocol(
            &Status,
            ProtocolHandle
            );

    }
}


NDIS_STATUS
ndisprotDoRequest(
    IN PNDISPROT_OPEN_CONTEXT pOpenContext,
    IN NDIS_REQUEST_TYPE    RequestType,
    IN NDIS_OID Oid,
    IN PVOID InformationBuffer,
    IN ULONG InformationBufferLength,
    OUT PULONG pBytesProcessed
    )
{
    NDISPROT_REQUEST ReqContext;
    PNDIS_REQUEST pNdisRequest = &ReqContext.Request;
    NDIS_STATUS Status;

    // 初始化一个事件。这个事件会在请求完成函数中被设置，
    // 以便通知请求完成了。
    NPROT_INIT_EVENT(&ReqContext.ReqEvent);

    // 请求的类型。如果只是查询信息，只要用NdisRequestQueryInformation
    // 就可以了。
    pNdisRequest->RequestType = RequestType;

    // 根据不同的请求类型，填写OID和输入输出缓冲区。
    switch (RequestType)
    {
        case NdisRequestQueryInformation:
            pNdisRequest->DATA.QUERY_INFORMATION.Oid = Oid;
            pNdisRequest->DATA.QUERY_INFORMATION.InformationBuffer =
                                    InformationBuffer;
            pNdisRequest->DATA.QUERY_INFORMATION.InformationBufferLength =
                                    InformationBufferLength;
            break;

        case NdisRequestSetInformation:
            pNdisRequest->DATA.SET_INFORMATION.Oid = Oid;
            pNdisRequest->DATA.SET_INFORMATION.InformationBuffer =
                                    InformationBuffer;
            pNdisRequest->DATA.SET_INFORMATION.InformationBufferLength =
                                    InformationBufferLength;
            break;

        default:
            NPROT_ASSERT(FALSE);
            break;
    }

    // 发送请求
    NdisRequest(&Status,
                pOpenContext->BindingHandle,
                pNdisRequest);
    
    // 如果是未决，则等待事件。这个事件会在完成函数中设置。
    if (Status == NDIS_STATUS_PENDING)
    {
        NPROT_WAIT_EVENT(&ReqContext.ReqEvent, 0);
        Status = ReqContext.Status;
    }

    // 如果成功了的话...
    if (Status == NDIS_STATUS_SUCCESS)
    {
        // 获得结果的长度。这个结果的长度是实际需要的长度。可
        // 能比我们实际提供的长度要长。
        *pBytesProcessed = (RequestType == NdisRequestQueryInformation)?
                            pNdisRequest->DATA.QUERY_INFORMATION.BytesWritten:
                            pNdisRequest->DATA.SET_INFORMATION.BytesRead;

        // 如果结果长度比实际上我们提供的缓冲区要长，那么就简
        // 单的设置为输入参数中缓冲区的最大长度。
        if (*pBytesProcessed > InformationBufferLength)
        {
            *pBytesProcessed = InformationBufferLength;
        }
    }
    return (Status);
}


NDIS_STATUS
ndisprotValidateOpenAndDoRequest(
    IN PNDISPROT_OPEN_CONTEXT        pOpenContext,
    IN NDIS_REQUEST_TYPE            RequestType,
    IN NDIS_OID                     Oid,
    IN PVOID                        InformationBuffer,
    IN ULONG                        InformationBufferLength,
    OUT PULONG                      pBytesProcessed,
    IN BOOLEAN                      bWaitForPowerOn
    )
/*++

Routine Description:

    Utility routine to prevalidate and reference an open context
    before calling ndisprotDoRequest. This routine makes sure
    we have a valid binding.

Arguments:

    pOpenContext - pointer to our open context
    RequestType - NdisRequest[Set|Query]Information
    Oid - the object being set/queried
    InformationBuffer - data for the request
    InformationBufferLength - length of the above
    pBytesProcessed - place to return bytes read/written
    bWaitForPowerOn - Wait for the device to be powered on if it isn't already.

Return Value:

    Status of the set/query request

--*/
{
    NDIS_STATUS             Status;

    do
    {
        if (pOpenContext == NULL)
        {
            DEBUGP(DL_WARN, ("ValidateOpenAndDoRequest: request on unassociated file object!\n"));
            Status = NDIS_STATUS_INVALID_DATA;
            break;
        }
               
        NPROT_STRUCT_ASSERT(pOpenContext, oc);

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        //
        //  Proceed only if we have a binding.
        //
        if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
        {
            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            Status = NDIS_STATUS_INVALID_DATA;
            break;
        }

        NPROT_ASSERT(pOpenContext->BindingHandle != NULL);

        //
        //  Make sure that the binding does not go away until we
        //  are finished with the request.
        //
        NdisInterlockedIncrement((PLONG)&pOpenContext->PendedSendCount);

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        if (bWaitForPowerOn)
        {
            //
            //  Wait for the device below to be powered up.
            //  We don't wait indefinitely here - this is to avoid
            //  a PROCESS_HAS_LOCKED_PAGES bugcheck that could happen
            //  if the calling process terminates, and this IRP doesn't
            //  complete within a reasonable time. An alternative would
            //  be to explicitly handle cancellation of this IRP.
            //
            NPROT_WAIT_EVENT(&pOpenContext->PoweredUpEvent, 4500);
        }

        Status = ndisprotDoRequest(
                    pOpenContext,
                    RequestType,
                    Oid,
                    InformationBuffer,
                    InformationBufferLength,
                    pBytesProcessed);
        
        //
        //  Let go of the binding.
        //
        NdisInterlockedDecrement((PLONG)&pOpenContext->PendedSendCount);
      
    }
    while (FALSE);

    DEBUGP(DL_LOUD, ("ValidateOpenAndDoReq: Open %p/%x, OID %x, Status %x\n",
                pOpenContext, pOpenContext->Flags, Oid, Status));

    return (Status);
}


VOID
NdisProtResetComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN NDIS_STATUS                  Status
    )
/*++

Routine Description:

    NDIS entry point indicating that a protocol initiated reset
    has completed. Since we never call NdisReset(), this should
    never be called.

Arguments:

    ProtocolBindingContext - pointer to open context
    Status - status of reset completion

Return Value:

    None

--*/
{
    UNREFERENCED_PARAMETER(ProtocolBindingContext);
    UNREFERENCED_PARAMETER(Status);
    
    ASSERT(FALSE);
    return;
}


VOID
NdisProtRequestComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext,
    IN PNDIS_REQUEST                pNdisRequest,
    IN NDIS_STATUS                  Status
    )
{
    PNDISPROT_OPEN_CONTEXT       pOpenContext;
    PNDISPROT_REQUEST            pReqContext;

    // 这两句话起验证的作用，确保输入参数ProtocolBindingContext
    // 是合法的。但是对后面的处理没影响。
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    // 从pNdisRequest中得到请求上下文
    pReqContext = CONTAINING_RECORD(pNdisRequest, NDISPROT_REQUEST, Request);

    // 保存结果状态
    pReqContext->Status = Status;

    // 设置事件
    NPROT_SIGNAL_EVENT(&pReqContext->ReqEvent);
}


VOID
NdisProtStatus(
    IN NDIS_HANDLE                          ProtocolBindingContext,
    IN NDIS_STATUS                          GeneralStatus,
    __in_bcount(StatusBufferSize) IN PVOID  StatusBuffer,
    IN UINT                                 StatusBufferSize
    )
/*++

Routine Description:

    Protocol entry point called by NDIS to indicate a change
    in status at the miniport.

    We make note of reset and media connect status indications.

Arguments:

    ProtocolBindingContext - pointer to open context
    GeneralStatus - status code
    StatusBuffer - status-specific additional information
    StatusBufferSize - size of the above

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT       pOpenContext;

    UNREFERENCED_PARAMETER(StatusBuffer);
    UNREFERENCED_PARAMETER(StatusBufferSize);
    
    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    DEBUGP(DL_INFO, ("Status: Open %p, Status %x\n",
            pOpenContext, GeneralStatus));

    ndisServiceIndicateStatusIrp(pOpenContext,
                                GeneralStatus,
                                StatusBuffer,
                                StatusBufferSize,
                                FALSE);

    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

    do
    {
        if (pOpenContext->PowerState != NetDeviceStateD0)
        {
            //
            //  The device is in a low power state.
            //
            DEBUGP(DL_INFO, ("Status: Open %p in power state %d,"
                " Status %x ignored\n", pOpenContext,
                pOpenContext->PowerState, GeneralStatus));
            //
            //  We continue and make note of status indications
            //
            // break;
            //

            //
            //  NOTE that any actions we take based on these
            //  status indications should take into account
            //  the current device power state.
            //
        }

        switch(GeneralStatus)
        {
            case NDIS_STATUS_RESET_START:
    
                NPROT_ASSERT(!NPROT_TEST_FLAGS(pOpenContext->Flags,
                                             NUIOO_RESET_FLAGS,
                                             NUIOO_RESET_IN_PROGRESS));

                NPROT_SET_FLAGS(pOpenContext->Flags,
                               NUIOO_RESET_FLAGS,
                               NUIOO_RESET_IN_PROGRESS);

                break;

            case NDIS_STATUS_RESET_END:

                NPROT_ASSERT(NPROT_TEST_FLAGS(pOpenContext->Flags,
                                            NUIOO_RESET_FLAGS,
                                            NUIOO_RESET_IN_PROGRESS));
   
                NPROT_SET_FLAGS(pOpenContext->Flags,
                               NUIOO_RESET_FLAGS,
                               NUIOO_NOT_RESETTING);

                break;

            case NDIS_STATUS_MEDIA_CONNECT:

                NPROT_SET_FLAGS(pOpenContext->Flags,
                               NUIOO_MEDIA_FLAGS,
                               NUIOO_MEDIA_CONNECTED);

                break;

            case NDIS_STATUS_MEDIA_DISCONNECT:

                NPROT_SET_FLAGS(pOpenContext->Flags,
                               NUIOO_MEDIA_FLAGS,
                               NUIOO_MEDIA_DISCONNECTED);

                break;

            default:
                break;
        }
    }
    while (FALSE);
       
    NPROT_RELEASE_LOCK(&pOpenContext->Lock);
}

VOID
NdisProtStatusComplete(
    IN NDIS_HANDLE                  ProtocolBindingContext
    )
/*++

Routine Description:

    Protocol entry point called by NDIS. We ignore this.

Arguments:

    ProtocolBindingContext - pointer to open context

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT       pOpenContext;

    pOpenContext = (PNDISPROT_OPEN_CONTEXT)ProtocolBindingContext;
    NPROT_STRUCT_ASSERT(pOpenContext, oc);

    return;
}


NDIS_STATUS
ndisprotQueryBinding(
    IN PUCHAR                       pBuffer,
    IN ULONG                        InputLength,
    IN ULONG                        OutputLength,
    OUT PULONG                      pBytesReturned
    )
/*++

Routine Description:

    Return information about the specified binding.

Arguments:

    pBuffer - pointer to NDISPROT_QUERY_BINDING
    InputLength - input buffer size
    OutputLength - output buffer size
    pBytesReturned - place to return copied byte count.

Return Value:

    NDIS_STATUS_SUCCESS if successful, failure code otherwise.

--*/
{
    PNDISPROT_QUERY_BINDING      pQueryBinding;
    PNDISPROT_OPEN_CONTEXT       pOpenContext;
    PLIST_ENTRY                 pEnt;
    ULONG                       Remaining;
    ULONG                       BindingIndex;
    NDIS_STATUS                 Status;

    do
    {
        if (InputLength < sizeof(NDISPROT_QUERY_BINDING))
        {
            Status = NDIS_STATUS_RESOURCES;
            break;
        }

        if (OutputLength < sizeof(NDISPROT_QUERY_BINDING))
        {
            Status = NDIS_STATUS_BUFFER_OVERFLOW;
            break;
        }

        Remaining = OutputLength - sizeof(NDISPROT_QUERY_BINDING);

        pQueryBinding = (PNDISPROT_QUERY_BINDING)pBuffer;
        BindingIndex = pQueryBinding->BindingIndex;

        Status = NDIS_STATUS_ADAPTER_NOT_FOUND;

        pOpenContext = NULL;

        NPROT_ACQUIRE_LOCK(&Globals.GlobalLock);

        for (pEnt = Globals.OpenList.Flink;
             pEnt != &Globals.OpenList;
             pEnt = pEnt->Flink)
        {
            pOpenContext = CONTAINING_RECORD(pEnt, NDISPROT_OPEN_CONTEXT, Link);
            NPROT_STRUCT_ASSERT(pOpenContext, oc);

            NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

            //
            //  Skip if not bound.
            //
            if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
            {
                NPROT_RELEASE_LOCK(&pOpenContext->Lock);
                continue;
            }

            if (BindingIndex == 0)
            {
                //
                //  Got the binding we are looking for. Copy the device
                //  name and description strings to the output buffer.
                //
                DEBUGP(DL_INFO,
                    ("QueryBinding: found open %p\n", pOpenContext));

                pQueryBinding->DeviceNameLength = pOpenContext->DeviceName.Length + sizeof(WCHAR);
                pQueryBinding->DeviceDescrLength = pOpenContext->DeviceDescr.Length + sizeof(WCHAR);
                if (Remaining < pQueryBinding->DeviceNameLength +
                                pQueryBinding->DeviceDescrLength)
                {
                    NPROT_RELEASE_LOCK(&pOpenContext->Lock);
                    Status = NDIS_STATUS_BUFFER_OVERFLOW;
                    break;
                }

                NPROT_ZERO_MEM((PUCHAR)pBuffer + sizeof(NDISPROT_QUERY_BINDING),
                                pQueryBinding->DeviceNameLength +
                                pQueryBinding->DeviceDescrLength);

                pQueryBinding->DeviceNameOffset = sizeof(NDISPROT_QUERY_BINDING);
                NPROT_COPY_MEM((PUCHAR)pBuffer + pQueryBinding->DeviceNameOffset,
                                pOpenContext->DeviceName.Buffer,
                                pOpenContext->DeviceName.Length);
                
                pQueryBinding->DeviceDescrOffset = pQueryBinding->DeviceNameOffset +
                                                    pQueryBinding->DeviceNameLength;
                NPROT_COPY_MEM((PUCHAR)pBuffer + pQueryBinding->DeviceDescrOffset,
                                pOpenContext->DeviceDescr.Buffer,
                                pOpenContext->DeviceDescr.Length);
                
                NPROT_RELEASE_LOCK(&pOpenContext->Lock);

                *pBytesReturned = pQueryBinding->DeviceDescrOffset + pQueryBinding->DeviceDescrLength;
                Status = NDIS_STATUS_SUCCESS;
                break;
            }

            NPROT_RELEASE_LOCK(&pOpenContext->Lock);

            BindingIndex--;
        }

        NPROT_RELEASE_LOCK(&Globals.GlobalLock);

    }
    while (FALSE);

    return (Status);
}

PNDISPROT_OPEN_CONTEXT
ndisprotLookupDevice(
    __in_bcount(BindingInfoLength) IN PUCHAR    pBindingInfo,
    IN ULONG                                    BindingInfoLength
    )
/*++

Routine Description:

    Search our global list for an open context structure that
    has a binding to the specified device, and return a pointer
    to it.

    NOTE: we reference the open that we return.

Arguments:

    pBindingInfo - pointer to unicode device name string
    BindingInfoLength - length in bytes of the above.

Return Value:

    Pointer to the matching open context if found, else NULL

--*/
{
    PNDISPROT_OPEN_CONTEXT       pOpenContext;
    PLIST_ENTRY                 pEnt;

    pOpenContext = NULL;

    NPROT_ACQUIRE_LOCK(&Globals.GlobalLock);

    for (pEnt = Globals.OpenList.Flink;
         pEnt != &Globals.OpenList;
         pEnt = pEnt->Flink)
    {
        pOpenContext = CONTAINING_RECORD(pEnt, NDISPROT_OPEN_CONTEXT, Link);
        NPROT_STRUCT_ASSERT(pOpenContext, oc);

        //
        //  Check if this has the name we are looking for.
        //
        if ((pOpenContext->DeviceName.Length == BindingInfoLength) &&
            NPROT_MEM_CMP(pOpenContext->DeviceName.Buffer, pBindingInfo, BindingInfoLength))
        {
            NPROT_REF_OPEN(pOpenContext);   // ref added by LookupDevice
            break;
        }

        pOpenContext = NULL;
    }

    NPROT_RELEASE_LOCK(&Globals.GlobalLock);

    return (pOpenContext);
}


NDIS_STATUS
ndisprotQueryOidValue(
    IN  PNDISPROT_OPEN_CONTEXT       pOpenContext,
    OUT PVOID                       pDataBuffer,
    IN  ULONG                       BufferLength,
    OUT PULONG                      pBytesWritten
    )
/*++

Routine Description:

    Query an arbitrary OID value from the miniport.

Arguments:

    pOpenContext - pointer to open context representing our binding to the miniport
    pDataBuffer - place to store the returned value
    BufferLength - length of the above
    pBytesWritten - place to return length returned

Return Value:

    NDIS_STATUS_SUCCESS if we successfully queried the OID.
    NDIS_STATUS_XXX error code otherwise.

--*/
{
    NDIS_STATUS             Status;
    PNDISPROT_QUERY_OID      pQuery;
    NDIS_OID                Oid;

    Oid = 0;

    do
    {
        if (BufferLength < sizeof(NDISPROT_QUERY_OID))
        {
            Status = NDIS_STATUS_BUFFER_TOO_SHORT;
            break;
        }

        pQuery = (PNDISPROT_QUERY_OID)pDataBuffer;
        Oid = pQuery->Oid;

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
        {
            DEBUGP(DL_WARN,
                ("QueryOid: Open %p/%x is in invalid state\n",
                    pOpenContext, pOpenContext->Flags));

            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            Status = NDIS_STATUS_FAILURE;
            break;
        }

        //
        //  Make sure the binding doesn't go away.
        //
        NdisInterlockedIncrement((PLONG)&pOpenContext->PendedSendCount);

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        Status = ndisprotDoRequest(
                    pOpenContext,
                    NdisRequestQueryInformation,
                    Oid,
                    &pQuery->Data[0],
                    BufferLength - FIELD_OFFSET(NDISPROT_QUERY_OID, Data),
                    pBytesWritten);

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        NdisInterlockedDecrement((PLONG)&pOpenContext->PendedSendCount);

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        if (Status == NDIS_STATUS_SUCCESS)
        {
            *pBytesWritten += FIELD_OFFSET(NDISPROT_QUERY_OID, Data);
        }

    }
    while (FALSE);

    DEBUGP(DL_LOUD, ("QueryOid: Open %p/%x, OID %x, Status %x\n",
                pOpenContext, pOpenContext->Flags, Oid, Status));

    return (Status);
    
}

NDIS_STATUS
ndisprotSetOidValue(
    IN  PNDISPROT_OPEN_CONTEXT       pOpenContext,
    OUT PVOID                       pDataBuffer,
    IN  ULONG                       BufferLength
    )
/*++

Routine Description:

    Set an arbitrary OID value to the miniport.

Arguments:

    pOpenContext - pointer to open context representing our binding to the miniport
    pDataBuffer - buffer that contains the value to be set
    BufferLength - length of the above

Return Value:

    NDIS_STATUS_SUCCESS if we successfully set the OID
    NDIS_STATUS_XXX error code otherwise.

--*/
{
    NDIS_STATUS             Status;
    PNDISPROT_SET_OID        pSet;
    NDIS_OID                Oid;
    ULONG                   BytesWritten;

    Oid = 0;

    do
    {
        if (BufferLength < sizeof(NDISPROT_SET_OID))
        {
            Status = NDIS_STATUS_BUFFER_TOO_SHORT;
            break;
        }

        pSet = (PNDISPROT_SET_OID)pDataBuffer;
        Oid = pSet->Oid;

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        if (!NPROT_TEST_FLAGS(pOpenContext->Flags, NUIOO_BIND_FLAGS, NUIOO_BIND_ACTIVE))
        {
            DEBUGP(DL_WARN,
                ("SetOid: Open %p/%x is in invalid state\n",
                    pOpenContext, pOpenContext->Flags));

            NPROT_RELEASE_LOCK(&pOpenContext->Lock);
            Status = NDIS_STATUS_FAILURE;
            break;
        }

        //
        //  Make sure the binding doesn't go away.
        //
        NdisInterlockedIncrement((PLONG)&pOpenContext->PendedSendCount);

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

        Status = ndisprotDoRequest(
                    pOpenContext,
                    NdisRequestSetInformation,
                    Oid,
                    &pSet->Data[0],
                    BufferLength - FIELD_OFFSET(NDISPROT_SET_OID, Data),
                    &BytesWritten);

        NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

        NdisInterlockedDecrement((PLONG)&pOpenContext->PendedSendCount);

        NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    }
    while (FALSE);

    DEBUGP(DL_LOUD, ("SetOid: Open %p/%x, OID %x, Status %x\n",
                pOpenContext, pOpenContext->Flags, Oid, Status));

    return (Status);
}

NTSTATUS
ndisprotQueueStatusIndicationIrp(
    IN  PNDISPROT_OPEN_CONTEXT       pOpenContext,
    IN  PIRP                         pIrp,
    OUT PULONG                       pBytesReturned    
    )
/*++

Routine Description:

    Queue the IRP in the context structure. This IRP will be completed
    one of the following occurs: 1) Status indication is received by
    the bound miniport, 2) IRP is cancelled by the sender, 3) We have
    been asked to unbind from the miniport either because the device is removed
    or user unchecked our binding in the network connection applet.
    
Arguments:

    pOpenContext - pointer to open context representing our binding to the miniport

Return Value:

    NTSTATUS code
    
--*/
{
    NTSTATUS NtStatus;
    
    DEBUGP(DL_LOUD, ("-->ndisprotQueueStatusIndicationIrp\n"));

    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);
    
    if(pOpenContext->StatusIndicationIrp){
        
        DEBUGP(DL_WARN, ("Control IRP to indicate status is already pending: %p\n", pIrp));
        NtStatus = STATUS_DEVICE_BUSY;
        
    }else {

        //
        // Since we are queueing the IRP, we should set the cancel routine.
        //
        IoSetCancelRoutine(pIrp, ndisCancelIndicateStatusIrp);

        //
        // Let us check to see if the IRP is cancelled at this point.
        //
        if(pIrp->Cancel && IoSetCancelRoutine(pIrp, NULL)){
            //
            // The IRP has been already cancelled but the I/O manager
            // hasn't called the cancel routine yet. So complete the
            // IRP after releasing the lock.
            //
            NtStatus = STATUS_CANCELLED;
            
        }else {
            //
            // Queue the IRP and return status pending.
            // 
            IoMarkIrpPending(pIrp);
            pOpenContext->StatusIndicationIrp = pIrp;
            NtStatus = STATUS_PENDING;
            
            //
            // The IRP shouldn't be accessed after the lock is released
            // It could be grabbed by another thread or the cancel routine
            // is running.
            //
        }
    }
    
    *pBytesReturned = 0;
    
    NPROT_RELEASE_LOCK(&pOpenContext->Lock);

    DEBUGP(DL_LOUD, ("<--ndisprotQueueStatusIndicationIrp\n"));

    return NtStatus;
    
}
VOID 
ndisServiceIndicateStatusIrp(
    IN PNDISPROT_OPEN_CONTEXT                       OpenContext,
    IN NDIS_STATUS                                  GeneralStatus,
    __in_bcount_opt(StatusBufferSize) IN PVOID      StatusBuffer,
    IN UINT                                         StatusBufferSize,
    IN BOOLEAN                                      Cancel
    )
/*++

Routine Description:

   We process the IRP based on the input arguments and complete
   the IRP. If the IRP was cancelled for some reason we will let
   the cancel routine do the IRP completion.
    
Arguments:

    ProtocolBindingContext - pointer to open context
    GeneralStatus - status code
    StatusBuffer - status-specific additional information
    StatusBufferSize - size of the above
    Cancel - Should the IRP be cancelled right away.

Return Value:

    None

--*/
{
    PIRP pIrp = NULL;
    PIO_STACK_LOCATION      pIrpSp = NULL;    
    PNDISPROT_INDICATE_STATUS     pIndicateStatus = NULL;
    NTSTATUS    ntStatus;
    ULONG   bytes = 0;
    ULONG inBufLength, outBufLength;

    DEBUGP(DL_LOUD, ("-->ndisServiceIndicateStatusIrp\n"));
    
    NPROT_ACQUIRE_LOCK(&OpenContext->Lock);

    pIrp = OpenContext->StatusIndicationIrp;

    do {
                
        if(pIrp){
            
            pIrpSp = IoGetCurrentIrpStackLocation(pIrp);            
            pIndicateStatus = pIrp->AssociatedIrp.SystemBuffer;
            inBufLength = pIrpSp->Parameters.DeviceIoControl.InputBufferLength;
            outBufLength = pIrpSp->Parameters.DeviceIoControl.OutputBufferLength;            
            //
            // Clear the cancel routine.
            //
            if(IoSetCancelRoutine(pIrp, NULL)){
                //
                // Cancel routine cannot run now and cannot have already 
                // started to run.
                //

                ntStatus = STATUS_CANCELLED;
                
                if(!Cancel){

                    //
                    // Check to see whether the buffer is large enough.
                    //
                    if(outBufLength >= sizeof(NDISPROT_INDICATE_STATUS) &&
                        (outBufLength - sizeof(NDISPROT_INDICATE_STATUS)) >= StatusBufferSize){
                        
                        pIndicateStatus->IndicatedStatus = GeneralStatus;
                        pIndicateStatus->StatusBufferLength = StatusBufferSize;
                        pIndicateStatus->StatusBufferOffset = sizeof(NDISPROT_INDICATE_STATUS);
                                 
                        NPROT_COPY_MEM((PUCHAR)pIndicateStatus + 
                                        pIndicateStatus->StatusBufferOffset,
                                        StatusBuffer,
                                        StatusBufferSize);
                        
                        
                        ntStatus = STATUS_SUCCESS;
                        
                    } else {
                        ntStatus = STATUS_BUFFER_OVERFLOW;
                    }
                }
                //
                // Since we are completing the IRP below, clear this field.
                //
                OpenContext->StatusIndicationIrp = NULL;                
                //
                // Number of bytes copied or number of bytes required.
                //
                bytes = sizeof(NDISPROT_INDICATE_STATUS) + StatusBufferSize;
                break;
            }else {
                //
                // Cancel rotuine is running. Leave the irp alone.
                //
                pIrp = NULL;
            }
        }
    }while(FALSE);
    
    NPROT_RELEASE_LOCK(&OpenContext->Lock);

    if(pIrp){
        pIrp->IoStatus.Information = bytes;
        pIrp->IoStatus.Status = ntStatus;
        IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    }

    DEBUGP(DL_LOUD, ("<--ndisServiceIndicateStatusIrp\n"));
    
    return;
    
}

VOID
ndisCancelIndicateStatusIrp(
    IN PDEVICE_OBJECT               pDeviceObject,
    IN PIRP                         pIrp
    )
/*++

Routine Description:

    Cancel the pending status indication IRP registered by the app or 
    another driver. 
    
Arguments:

    pDeviceObject - pointer to our device object
    pIrp - IRP to be cancelled

Return Value:

    None

--*/
{
    PNDISPROT_OPEN_CONTEXT       pOpenContext;
    PIO_STACK_LOCATION          pIrpSp = NULL;    

    UNREFERENCED_PARAMETER(pDeviceObject);

    DEBUGP(DL_LOUD, ("-->ndisCancelIndicateStatusIrp\n"));
    
    IoReleaseCancelSpinLock(pIrp->CancelIrql);

    pIrpSp = IoGetCurrentIrpStackLocation(pIrp);

    pOpenContext = (PNDISPROT_OPEN_CONTEXT)pIrpSp->FileObject->FsContext;

    NPROT_STRUCT_ASSERT(pOpenContext, oc);
    NPROT_ACQUIRE_LOCK(&pOpenContext->Lock);

    pOpenContext->StatusIndicationIrp = NULL;

    NPROT_RELEASE_LOCK(&pOpenContext->Lock);
    
    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = STATUS_CANCELLED;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    DEBUGP(DL_LOUD, ("<--ndisCancelIndicateStatusIrp\n"));

    return;

}


