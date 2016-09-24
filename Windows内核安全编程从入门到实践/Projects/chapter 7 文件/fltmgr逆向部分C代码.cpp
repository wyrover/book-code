\\
\\XP sp3
\\

//
//代表服务端的数据结构
//
typedef struct _FLT_SERVER_PORT_OBJECT_ {
    PFLT_CONNECT_NOTIFY  ConnectNotify;       //微过滤驱动连接处理函数地址
    PFLT_DISCONNECT_NOTIFY  DisconnectNotify; //断开连接处理函数地址
    PFLT_MESSAGE_NOTIFY  MessageNotify;       //处理应用层发送的消息的函数地址
    PFLT_FILTER  Filter;                      //调用FltRegisterFilter得到的对象
    PVOID  Cookie;
    ULONG  Flags;
    ULONG  NumberOfConnections;               //当前连接到服务端的连接数
    ULONG  MaxConnections;

} FLT_SERVER_PORT_OBJECT, *PFLT_SERVER_PORT_OBJECT;


//
//创建一个用于通信的服务端口：
//   实际上就是生成一个类型为FilterConnectionPort的对象，
//并将连接等处理函数地址存入该对象
//
NTSTATUS
FltCreateCommunicationPort(
    IN PFLT_FILTER  Filter,
    OUT PFLT_PORT  *ServerPort,
    IN POBJECT_ATTRIBUTES  ObjectAttributes,
    IN PVOID  ServerPortCookie OPTIONAL,
    IN PFLT_CONNECT_NOTIFY  ConnectNotifyCallback,
    IN PFLT_DISCONNECT_NOTIFY  DisconnectNotifyCallback,
    IN PFLT_MESSAGE_NOTIFY  MessageNotifyCallback,
    IN LONG  MaxConnections
)
{
    NTSTATUS status;
    PFLT_SERVER_PORT_OBJECT pObject;

    //
    //进行一些参数合法性检查
    //
    if (MaxConnections <= 0) {
        return STATUS_INVALID_PARAMETER;
    }

    if (!(ObjectAttributes->Attributes & OBJ_KERNEL_HANDLE)) {
        return STATUS_INVALID_PARAMETER;
    }

    status = FltObjectReference(Filter);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //创建一个类型为FilterConnectionPortType的对象，大小为0x20,
    //其对应的数据结构为FLT_SERVER_PORT_OBJECT
    status = ObCreateObject(0, FilterConnectionPortType, ObjectAttributes, 0, 0, 0x20, 0, 0, &pObject);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    RtlZeroMemory(pObject, 0x20);
    FltpObjectPointerReference(Filter);
    //填入一些函数地址，以供之后调用
    pObject->ConnectNotify = ConnectNotifyCallback;
    pObject->DisconnectNotify = DisconnectNotifyCallback;
    pObject->MessageNotify = MessageNotifyCallback;
    pObject->Filter = Filter;
    pObject->Cookie = ServerPortCookie;
    pObject->MaxConnections = MaxConnections;
    //添加到句柄表中
    status = ObInsertObject(pObject, 0, 0x1f0001, 0, 0, ServerPort);
    return status;
}




FilterConnectCommunicationPort:

1:
kd > kb
ChildEBP RetAddr  Args to Child
b28a695c f84af70c 800007e0 00000000 00000000 scanner!ScannerPortConnect + 0x5 [d:\projects\scanner\scanner.c @ 299]
b28a6a0c f849ea56 8235d970 82429078 82429088 fltMgr!FltpOpenCommunicationPort + 0x29c
b28a6a2c f84ab53d 82187648 82429078 82181918 fltMgr!FltpMsgDispatch + 0xa6
b28a6a5c 804f018f 82187648 82429078 82429078 fltMgr!FltpCreate + 0x53
b28a6a6c 805841fa 82187630 821622dc b28a6c04 nt!IopfCallDriver + 0x31
b28a6b4c 805c0444 82187648 00000000 82162238 nt!IopParseDevice + 0xa12
b28a6bc4 805bc9d0 00000000 b28a6c04 00000040 nt!ObpLookupObjectName + 0x53c
b28a6c18 80577033 00000000 00000000 8a6c8401 nt!ObOpenObjectByName + 0xea
b28a6c94 805779aa 0012fe30 00100003 0012fde8 nt!IopCreateFile + 0x407
b28a6cf0 8057a0b4 0012fe30 00100003 0012fde8 nt!IoCreateFile + 0x8e
b28a6d30 8054261c 0012fe30 00100003 0012fde8 nt!NtCreateFile + 0x30
b28a6d30 7c92e4f4 0012fe30 00100003 0012fde8 nt!KiFastCallEntry + 0xfc
0012fda4 7c92d09c 4aa9149b 0012fe30 00100003 ntdll!KiFastSystemCallRet
0012fda8 4aa9149b 0012fe30 00100003 0012fde8 ntdll!ZwCreateFile + 0xc
0012fe34 004012aa 0040b3a4 00000000 00000000 FLTLIB!FilterConnectCommunicationPort + 0x13f
0012ff78 0040180e 00000001 003b2ea0 003b2ef8 scanuser!main + 0xaa [d:\projects\scanner\scanuser.c @ 318]
003b0518 00000000 003b0520 003b0520 003b0528 scanuser!__tmainCRTStartup + 0xfb [f:\dd\vctools\crt_bld\self_x86\crt\src\crt0.c @ 266]


HRESULT
WINAPI
FilterConnectCommunicationPort(
    IN LPCWSTR  lpPortName,
    IN DWORD  dwOptions,
    IN LPVOID  lpContext OPTIONAL,
    IN DWORD  dwSizeOfContext,
    IN LPSECURITY_ATTRIBUTES  lpSecurityAttributes OPTIONAL,
    OUT HANDLE  *hPort
)
{
    ...
    FltpOpenCommunicationPort(fileobj, pIrp);
    ...
}

typedef struct _FLT_MUTEX_LIST_HEAD_ {
    FAST_MUTEX mLock;
    LIST_ENTRY mList;                         //待完成消息链表
    ULONG mCount;

} FLT_MUTEX_LIST_HEAD;

typedef struct _FLT_MESSAGE_WAITER_QUEUE_ {
    IO_CSQ Csq;
    FLT_MUTEX_LIST_HEAD WaiterQ;               //包含待完成消息链表
    ULONG MinimumWaiterLength;
    KSEMAPHORE Semaphore;                      //信号灯，与消息数量有关
    KEVENT Event;                              //应用层每投递一次将会设置该事件，这样如FltSendMessage等函数中的对应的KeWaitForMultipleObjects将返回

} FLT_MESSAGE_WAITER_QUEUE, *PFLT_MESSAGE_WAITER_QUEUE;

//
//代表通信的另一端——客户端对象数据结构
//
typedef struct _FLT_PORT_OBJECT_ {
    LIST_ENTRY Links;
    PFLT_SERVER_PORT_OBJECT ServerPort;         //指向了服务端口
    PVOID Cookie;
    FAST_MUTEX Lock;
    FLT_MESSAGE_WAITER_QUEUE MsgQ;              //包含待完成消息链表等，应用层调用FilterGetMessage将会把消息投递到这里
    ULONGULONG MessageId;                       //消息ID,以1为单位递增
    KEVENT  DisconnectEvent;
    UCHAR Disconnected;

} FLT_PORT_OBJECT, *PFLT_PORT_OBJECT;


typedef _PORT_CCB_ {
    PFLT_PORT_OBJECT Port;                      //保存了通信客户端的地址
    FLT_MUTEX_LIST_HEAD ReplyWaiterList;        //保存了需要完成reply请求的链表，每一个单元的数据类型为ReplyWaitNode

} PORT_CCB, *PPORT_CCB;

typedef struct _FLT_TYPE_ {
    WORD Signature;
    WORD Size;

} FLT_TYPE, *PFLT_TYPE;

typedef union _FLT_CCB_DATA_ {
    MANAGER_CCB Manager;
    FILTER_CCB Filter;
    INSTANCE_CCB Instance;
    VOLUME_CCB Volume;
    PORT_CCB Port;
} FLT_CCB_DATA, *PFLT_CCB_DATA;

//
//该数据结构可以保存很多种CCB的内容，如PORT_CCB；
//该数据结构将保存在对应fileobject的FsContext2中
//
typedef struct _FLT_CCB {
    FLT_TYPE Type;
    FLT_CCB_DATA Data;

} FLT_CCB, *PFLT_CCB;

//
//应用层调用FilterConnectCommunicationPort将会调用该函数，
//该函数主要生成一个类型为FilterCommunicationPortType的对象代表客户端通信端口，
//另外，还会分配一个FLT_CCB结构保存在文件对象中，以后通过这个文件对象将能获得
//reply消息链表以及应用程序投递的消息链表
//
NTSTATUS
FltpOpenCommunicationPort(PFILE_OBJECT fileobj, PIRP pIrp)
{
    NTSTATUS status;
    PFLT_PORT_OBJECT pClientPortObject;
    PFLT_SERVER_PORT_OBJECT pServerPortObject;
    PVOID pBuffer;
    OBJECT_ATTRIBUTES oa;
    HANDLE hClientPortHandle;
    PFLT_CCB pFltccb;

    //
    //参数检查
    //
    if (fileobj->FileName.Length <= 0) {
        return STATUS_ACCESS_DENIED;
    }

    pBuffer = pIrp->AssociatedIrp.SystemBuffer;

    if (pBuffer == NULL) {
        return STATUS_ACCESS_DENIED;
    }

    if (*(WORD*)((ULONG)pBuffer + 6) < 0x10) {
        return STATUS_ACCESS_DENIED;
    }

    if (*(BYTE*)((ULONG)pBuffer + 5) != 7) {
        return STATUS_ACCESS_DENIED;
    }

    if (RtlCompareMemory((ULONG)pBuffer + 8, "FLTPORT", 7) != 7) {
        return STATUS_ACCESS_DENIED;
    }

    if (*(WORD*)((ULONG)pBuffer + 0x18) + 0x10 > *(WORD*)((ULONG)pBuffer + 6)) {
        return STATUS_ACCESS_DENIED;
    }

    //获取通信服务端对象
    status = ObReferenceObjectByName(
                 *(UNICODE_STRING*)(ULONG)pBuffer + 0x10, //"ScannerPort"
                 0,
                 0,
                 0x1f0001,
                 FilterConnectionPortType,
                 ExGetPreviousMode(),
                 0,
                 &pServerPortObject
             );

    if (!NT_SUCCESS(status)) {
        return STATUS_ACCESS_DENIED;
    }

    //增加连接数量
    pServerPortObject->NumberOfConnections += 1;

    if (pServerPortObject->NumberOfConnections > pServerPortObject->MaxConnections) {
        return STATUS_CONNECTION_COUNT_LIMIT;
    }

    oa.Length = 0x18;
    oa.RootDirectory = NULL;
    oa.ObjectName = NULL;
    oa.Attributes = 0x200;
    oa.SecurityDescriptor = 0;
    oa.SecurityQualityOfService = 0;
    //生成一个类型为FilterCommunicationPortType的对象，代表通信的客户端
    status = ObCreateObject(
                 0,
                 FilterCommunicationPortType,
                 &oa,
                 0,
                 0,
                 0xc8,
                 0,
                 0,
                 &pClientPortObject
             );

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //
    //初始化这个客户端对象
    //
    RtlZeroMemory(pClientPortObject, 0xc8);
    pClientPortObject->Lock.Count = 1;
    pClientPortObject->Lock.Owner = NULL;
    pClientPortObject->Lock.Contention = 0;
    KeInitializeEvent(&pClientPortObject->Lock.Event, SynchronizationEvent, false);
    //保存了通信服务端的对象
    pClientPortObject->ServerPort = pServerPortObject;
    pClientPortObject->MsgQ.WaiterQ.mCount = 0;
    pClientPortObject->MsgQ.WaiterQ.mLock.Count = 1;
    pClientPortObject->MsgQ.WaiterQ.mLock.Owner = NULL;
    pClientPortObject->MsgQ.WaiterQ.mLock.Contention = NULL;
    KeInitializeEvent(&pClientPortObject->MsgQ.WaiterQ.mLock.Event, SynchronizationEvent, false);
    //初始化投递消息链表，应用程序每调用一次FilterGetMessage将会在该链表中增加一个节点（IRP）,
    //当微过滤驱动调用一次FltSendMessage将会消耗一个该节点（即完成IRP）
    pClientPortObject->MsgQ.WaiterQ.mList.Flink = &pClientPortObject->MsgQ.WaiterQ.mList;
    pClientPortObject->MsgQ.WaiterQ.mList.Blink = &pClientPortObject->MsgQ.WaiterQ.mList;
    pClientPortObject->MsgQ.MinimumWaiterLength |= 0xffffffff;
    //初始化信号灯，信号灯的数量即当前投递到mList中IRP的数量
    KeInitializeSemaphore(&pClientPortObject->MsgQ.Semaphore, 0, 0x7fffffff);
    //初始化事件，每投递一次请求，都会设置该事件有信号
    KeInitializeEvent(&pClientPortObject->MsgQ.Event, NotificationEvent, false);
    KeInitializeEvent(&pClientPortObject->DisconnectEvent, NotificationEvent, false);
    //初始化IoCsq
    FltpIoCsqInitializeEx(
        &pClientPortObject->MsgQ,
        FltpAddMessageWaiter,
        FltpRemoveMessageWaiter,
        FltpGetNextMessageWaiter,
        FltpAcquireMessageWaiterLock,
        FltpReleaseMessageWaiterLock,
        FltpCancelMessageWaiter
    );
//FltpIoCsqInitializeEx的内容即：
//  pClientPortObject->MsgQ.Csq.Type = 3;
//  pClientPortObject->MsgQ.Csq.CsqInsertIrp = FltpAddMessageWaiter;
//  pClientPortObject->MsgQ.Csq.CsqRemoveIrp = FltpRemoveMessageWaiter;
//  pClientPortObject->MsgQ.Csq.CsqPeekNextIrp = FltpGetNextMessageWaiter;
//  pClientPortObject->MsgQ.Csq.CsqAcquireLock = FltpAcquireMessageWaiterLock;
//  pClientPortObject->MsgQ.Csq.CsqReleaseLock = FltpReleaseMessageWaiterLock;
//  pClientPortObject->MsgQ.Csq.CsqCompleteCanceledIrp = FltpCancelMessageWaiter;
//  pClientPortObject->MsgQ.Csq.ReservePointer = NULL;
    //将客户端对象添加到句柄表中
    status = ObInsertObject(pClientPortObject, 0, 0x1f0001, 1, 0, &hClientPortHandle);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //分配一个FLT_CCB结构
    //1.该结构会保存客户端对象
    //2.该结构会保存Reply链表
    pFltccb = FltpAllocateFltCcb(0x0f10d, 0);

    if (NULL = pFltccb) {
        return STATUS_INSUFFICIENT_RESOURCES;
    }

    //保存客户端对象
    pFltccb->Data->Port.Port = pClientPortObject;
    pFltccb->Data->Port.ReplyWaiterList.mCount = 0;
    pFltccb->Data->Port.ReplyWaiterList.mLock.Count = 1;
    pFltccb->Data->Port.ReplyWaiterList.mLock.Owner = NULL;
    pFltccb->Data->Port.ReplyWaiterList.mLock.Contention = 0;
    KeInitializeEvent(&pFltccb->Data->Port.ReplyWaiterList.mLock.Event, SynchronizationEvent, false);
    //初始化Reply链表，这个链表用来保存微过滤驱动调用FltSendMessage希望获得回复的请求
    //微过滤驱动每调用一次FltSendMessage且ReplyBuffer 参数不为空，将会在该链表中添加一个请求；
    //当应用程序调用FilterReplyMessage将会完成该链表中的一个请求
    pFltccb->Data->Port.ReplyWaiterList.mList.Flink = &pFltccb->Data->Port.ReplyWaiterList.mList;
    pFltccb->Data->Port.ReplyWaiterList.mList.Blink = &pFltccb->Data->Port.ReplyWaiterList.mList;
    status = FltObjectReference(pServerPortObject->Filter);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    //调用微过滤驱动向过滤管理器注册的连接处理函数
    status = pServerPortObject->ConnectNotify(
                 hClientPortHandle,
                 pServerPortObject->Cookie,
                 (ULONG)pBuffer + 0x20,
                 *(WORD*)((ULONG)pBuffer + 0x18),
                 pClientPortObject->Cookie
             );

    if (!NT_SUCCESS(status)) {
        FltObjectDereference(pServerPortObject->Filter);
        return status;
    }

    //保存这个FLT_CCB结构到文件对象结构中
    fileobj->FsContext2 = pFltccb;
    pClientPortObject->Links.Flink = 0;
    ExAcquireFastMutex(&pServerPortObject->Filter->PortList.mLock);

    if (!pServerPortObject->Filter->PortList.mInvalid) {
        pServerPortObject->Filter->PortList.mCount += 2;
        InsertHeadList(&pServerPortObject->Filter->PortList.mList, &pClientPortObject->Links);
    }

    ExReleaseFastMutex(&pServerPortObject->Filter->PortList.mLock);
    FltObjectDereference(pServerPortObject->Filter);
    return status;
}

//
//当过滤驱动调用FltSendMessage函数且参数ReplyBuffer不为空的时候，
//将会向FLT_CCB的Reply链表中插入一个该结构
//
typedef struct _ReplyWaitNode_ {
    LIST_ENTRY entry;
    ULONGULONG msg;     //消息ID号，用来定位消息
    KEVENT kevent;      //FltSendMessage会等待该事件，当应用程序调用FilterReplyMessage将会置该事件有信号
    PVOID ReplyBuf;     //FltSendMessage提供的恢复缓冲区，FilterReplyMessage将会填充该缓冲区
    ULONG ReplyLen;     //ReplyBuf大小
    NTSTATUS status;
    ULONG UnKnow2;

} ReplyWaitNode, *PReplyWaitNode;

//
//微过滤驱动调用该函数向应用程序发送消息，ReplyBuffer不为0，将等待应用程序的回复
//
NTSTATUS
FltSendMessage(
    __in PFLT_FILTER  Filter,
    __in PFLT_PORT  *ClientPort,
    __in PVOID  SenderBuffer,
    __in ULONG  SenderBufferLength,
    __out_opt PVOID  ReplyBuffer OPTIONAL,
    __inout_opt PULONG  ReplyLength
    __in_opt PLARGE_INTEGER  Timeout
)
{
    NTSTATUS status;
    PFLT_PORT_OBJECT pClientObject;
    PVOID pWaitObject[2];
    PIRP pIrp;
    PMDL pMdlUserAddr;
    PVOID pAddrbyMdl;
    ReplyWaitNode WaitNode;
    FLT_MUTEX_LIST_HEAD *pReplyWaiter;
    ...
    //获取通信客户端
    status = ObReferenceObjectByHandle(
                 *ClientPort,
                 0x1f0001,
                 FilterCommunicationPortType,
                 0,
                 &pClientObject,
                 0
             );

    if (!NT_SUCCESS(status)) {
        return status;
    }

    ...
    pWaitObject[0] = &pClientObject->DisconnectEvent;   //通信断开事件
    pWaitObject[1] = &pClientObject->MsgQ.Semaphore;    //等待消息信号灯

    do {
        status = KeWaitForMultipleObjects(
                     2,
                     pWaitObject,
                     WaitAny,
                     Executive,
                     KernelMode,
                     false,
                     Timeout,
                     NULL
                 );

        if (STATUS_TIMEOUT == status) {
            return status;
        } else if (STATUS_WAIT_0 == status) {
            return  STATUS_PORT_DISCONNECTED;
        }

        //从等待消息链表中取出一个合适（缓冲区大小相同）的IRP，
        //如果没有找到大小合适的IRP，将会取出链表中的第一个IRP
        pIrp = FltpIoCsqRemoveNextIrp(&pClientObject->MsgQ, SenderBufferLength + 0x10);

        if (pIrp == NULL) {
            ...
            continue;
        }

        //判断取出的IRP大小是否合适，并分别做出处理
        if (pIrp->Tail.Overlay.CurrentStackLocation->Parameters.Read.Length < SenderBufferLength + 0x10) {
            pIrp->IoStatus.Status = STATUS_BUFFER_TOO_SMALL;
            pIrp->IoStatus.Information = SenderBufferLength + 0x10;
            ExAcquireFastMutex(&pClientObject->MsgQ.WaiterQ.mLock);

            if (pClientObject->MsgQ.MinimumWaiterLength > SenderBufferLength + 0x10) {
                pClientObject->MsgQ.MinimumWaiterLength = SenderBufferLength + 0x10;
                KeClearEvent(&pClientObject->MsgQ.Event);
            }

            ExReleaseFastMutex(&pClientObject->MsgQ.WaiterQ.mLock);
            //完成这个IRP，提醒应用程序所提供的缓冲区太小，并期待应用程序重新投递一个大小合适的请求
            IofCompleteRequest(pIrp, 0);
            pWaitObject[1] = &pClientObject->MsgQ.Event;
            //等待应用程序重新投递，应用程序一旦投递将会设置pClientObject->MsgQ.Event这个事件（参见FilterGetMessage）
            status = KeWaitForMultipleObjects(
                         2,
                         pWaitObject,
                         WaitAny,
                         Executive,
                         KernelMode,
                         false,
                         Timeout,
                         NULL
                     );

            if (status == STATUS_TIMEOUT) {
                return status;
            } else if (status == STATUS_WAIT_0) {
                return STATUS_PORT_DISCONNECTED;
            }

            pIrp = NULL;
            ...
            continue;
        }

        //
        //使用MDL映射应用程序请求的缓冲区，将微过滤驱动发送的信息拷贝到映射后的地址中
        //
        if (!pIrp->MdlAddress) {
            //+0x10是为了符合FILTER_MESSAGE_HEADER的要求，sizeof(FILTER_MESSAGE_HEADER)==0x10
            pMdlUserAddr = IoAllocateMdl(pIrp->UserBuffer, SenderBufferLength + 0x10, false, true, NULL);
            pIrp->MdlAddress = pMdlUserAddr;

            if (NULL == pMdlUserAddr) {
                pIrp->IoStatus.Status = STATUS_INSUFFICIENT_RESOURCES;
                pIrp->IoStatus.Information = 0;
                IofCompleteRequest(pIrp, 0);
                pIrp = NULL;
                ...
                continue;
            }

            MmProbeAndLockProcessPages(pMdlUserAddr, IoGetRequestorProcess(pIrp), pIrp->RequestorMode, 1);
        }

        if (pIrp->MdlAddress.MdlFlags & 5) {
            pAddrbyMdl = MmMapLockedPagesSpecifyCache(pMdlUserAddr, KernelMode, MmCached, NULL, 0, 0x10);
        } else {
            pAddrbyMdl = pMdlUserAddr->MappedSystemVa;
        }

        if (NULL == pAddrbyMdl) {
            ExRaiseStatus(STATUS_INSUFFICIENT_RESOURCES);
        }

        RtlZeroMemory(pAddrbyMdl, SenderBufferLength = 0x10);
        RtlCopyMemory((ULONG)pAddrbyMdl + 0x10, SenderBuffer, SenderBufferLength);
        pIrp->IoStatus.Status = STATUS_SUCCESS;
        pIrp->IoStatus.Information = SenderBufferLength + 0x10;
        break;
    } while (true);

    //如果ReplyBuffer为空，那么该函数到此就结束了；
    //否则需要处理恢复请求
    if (NULL == ReplyBuffer) {
        IofCompleteRequest(pIrp, 0);
        return status;
    }

    //分配一个ReplyWaitNode结构，该结构代表了一个回复请求，并将保存到PORT_CCB结构的回复等待链表中
    RtlZeroMemory(&WaitNode, sizeof(ReplyWaitNode));
    WaitNode.UnKnow2 |= 1;
    WaitNode.msg = ++pClientObject->MessageId;
    WaitNode.ReplyBuf = ReplyBuffer;
    WaitNode.ReplyLen = *(ULONG*)ReplyLength;
    KeInitializeEvent(&WaitNode.kevent, NotificationEvent, false);
    ((PFILTER_MESSAGE_HEADER)pAddrbyMdl)->ReplyLength = WaitNode.ReplyLen + 0x10;
    ((PFILTER_MESSAGE_HEADER)pAddrbyMdl)->MessageId = WaitNode.msg;
    pReplyWaiter = &(((PORT_CCB*)((FLT_CCB*)pIrp->Tail.Overlay.CurrentStackLocation->FileObject->FsContext2).Data.Port).ReplyWaiterList);
    ExAcquireFastMutex(&pReplyWaiter->mLock);

    if (pReplyWaiter->mCount & 1 == 0) {
        InsertTailList(&pReplyWaiter->mList, &WaitNode.entry);
    }

    ExReleaseFastMutex(&pReplyWaiter->mLock);
    //完成IRP，等待应用程序回复
    IofCompleteRequest(pIrp, 0);
    //等待事件，FilterReplyMessage将会置该事件有信号
    pWaitObject[1] = &WaitNode.kevent;
    status = KeWaitForMultipleObjects(
                 2,
                 pWaitObject,
                 WaitAny,
                 Executive,
                 KernelMode,
                 false,
                 Timeout,
                 NULL
             );

    if (status == STATUS_TIMEOUT) {
        ...
    } else if (STATUS_WAIT_0 == status) {
        return STATUS_PORT_DISCONNECTED;
    }

    status = WaitNode.status;

    if (ReplyLength) {
        *(ULONG*)ReplyLength = WaitNode.ReplyLen;
    }

    return status;
}



//
//应用程序调用这个函数投递一个消息请求到通信客户端消息等待链表中
//
HRESULT
WINAPI
FilterGetMessage(
    IN HANDLE  hPort,
    IN OUT PFILTER_MESSAGE_HEADER  lpMessageBuffer,
    IN DWORD  dwMessageBufferSize,
    IN LPOVERLAPPED  lpOverlapped OPTIONAL
)
{
    ...
    //FltpGetMessage会调用FltpIoCsqInsertIrpEx，FltpIoCsqInsertIrpEx调用FltpAddMessageWaiter
    FltpGetMessage();
    ...
}

//
//FltpAddMessageWaiter只是将本次请求的IRP添加到MsgQ.WaiterQ.mList链表中，并设置MsgQ.Event
//
NTSTATUS
FltpAddMessageWaiter（FLT_MESSAGE_WAITER_QUEUE* pMsgQ, PIRP pIrp） {
    if (pMsgQ->WaiterQ.mCount & 1 == 0)
    {
        pMsgQ->WaiterQ.mCount += 2;
        InsertTailList(&pMsgQ->WaiterQ.mList, &pIrp->Tail.Overlay.ListEntry);

        if (pIrp->Tail.Overlay.CurrentStackLocation.Parameters.Read.Length > pMsgQ->MinimumWaiterLength) {
            KeSetEvent(&pMsgQ->Event, 0, 0);
        }

        //增加一个信号灯
        KeReleaseSemaphore(&pMsgQ->Semaphore, 1, 1, 0);
    } else
    {
        return STATUS_PORT_DISCONNECTED;
    }

    return STATUS_PENDING;
};




//
//应用程序调用这个函数回复微过滤驱动的回复请求
//
HRESULT
WINAPI
FilterReplyMessage(
    __in HANDLE  hPort,
    __in PFILTER_REPLY_HEADER  lpReplyBuffer,
    __in DWORD  dwReplyBufferSize
)
{
    ...
    FltpFilterReply(...);
    ...
}

//
//本函数将完成一次微过滤驱动的等待回复请求
//
NTSTATUS
FltpFilterReply(PFILE_OBJECT FileObj, PVOID ReplyBuf, ULONG ReplyLen, PULONG Retlen, BOOLEAN InKernel)
{
    NTSTATUS status;
    FLT_MUTEX_LIST_HEAD *pReplyWaiter;
    ReplyWaitNode* pWaitNode;
    ULONG uBufLen;
    *Retlen = 0;

    if (ReplyLen < 0x10) {
        return STATUS_INVALID_PARAMETER;
    }

    if (InKernel == FALSE) {
        ProbeForRead(ReplyBuf, ReplyLen, 4);
    }

    status = ((FILTER_REPLY_HEADER*)ReplyBuf).Status;
    //ReplyWaitNode被保存在了文件对象中了，参见FltpOpenCommunicationPort函数
    pReplyWaiter = ((PORT_CCB*)(((FLT_CCB*)FileObj->FsContext2).Data.Port))->ReplyWaiterList;
    ExAcquireFastMutex(&pReplyWaiter->mLock);
    pWaitNode = pReplyWaiter->mList.Flink;

    if (pWaitNode == &pReplyWaiter->mList) {
        ExReleaseFastMutex(&pReplyWaiter->mLock);
        return 0xC01C0020;
    }

    //找到待完成的等待回复请求，并取出
    do {
        if (pWaitNode->msg == ((FILTER_REPLY_HEADER*)ReplyBuf)->MessageId) {
            pWaitNode->entry.Blink->Flink = pWaitNode->entry.Flink;
            pWaitNode->entry.Flink->Blink = pWaitNode->entry.Blink;
            break;
        }

        pWaitNode = pWaitNode->entry.Flink;
    } while (pWaitNode != &pReplyWaiter->mList);

    ExReleaseFastMutex(&pReplyWaiter->mLock);

    if (pWaitNode == &pReplyWaiter->mList) {
        return 0xC01C0020;
    }

    uBufLen = ReplyLen - 0x10;

    if (uBufLen >= pWaitNode->ReplyLen) {
        uBufLen = pWaitNode->ReplyLen;
    }

    //填充缓冲区
    RtlCopyMemory(pWaitNode->ReplyBuf, (ULONG)ReplyBuf + 0x10, uBufLen);

    if (ReplyLen - 0x10 > uBufLen) {
        status = STATUS_BUFFER_OVERFLOW;
    }

    *Retlen = uBufLen;
    pWaitNode->status = status;
    pWaitNode->ReplyLen = uBufLen;
    //设置事件，FltSendMessage函数在等待该事件
    KeSetEvent(&pWaitNode->kevent, 0, 0);
    return (status == STATUS_BUFFER_OVERFLOW) ? STATUS_BUFFER_OVERFLOW : STATUS_SUCCESS;
}