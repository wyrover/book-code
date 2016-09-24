//
//Description:
//  本程序用于实现Win7平台下的进程监控程序，在虚拟机Win7平台上测试通过。
//  使用vs2008+visualDDK+WDK7100编译
//
//Writen By: 宁妖 2011/5/20
//
//Last Write Time: 2011/5/20
//
#include "stdafx.h"

//
//Global Vars
//
HANDLE   g_hMeID = NULL;
PKEVENT  gEvent_A = NULL;
KEVENT   Event_B ;
PKEVENT  gEvent_B = NULL;

PKMUTEX gMutex;
int     iChoose = 0;

PVOID   pShareMM_SYS = NULL;
PVOID   pShareMM_User = NULL;
PMDL    pShareMM_MDL = NULL;
ULONG   ImageFileNameOffset = 0;

GlobalFlag GFlag = {0};

//HOOK
RealServiceAddr RealAddr = {0};

ULONG NtCreateUserProcessID = 0;



NTSTATUS
DriverEntry(IN PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
    NTSTATUS status;
    UNICODE_STRING devName;
    UNICODE_STRING linkName;
    PDEVICE_OBJECT pdevObj;
    ULONG index = 0;
    //版本判断，只适用于6.x系统
    status = InitVerion();

    if (!NT_SUCCESS(status)) {
        return STATUS_FAILED_DRIVER_ENTRY;
    }

    //初始化一些同步事件、互斥体
    gMutex = (PKMUTEX)ExAllocatePool(NonPagedPool, sizeof(KMUTEX));

    if (NULL == gMutex) {
        KdPrintEx((xx, "[DriverEntry] ExAllocatePool Failure!\n"));
        return STATUS_FAILED_DRIVER_ENTRY;
    }

    KeInitializeMutex(gMutex, 0);
    gEvent_B = &Event_B;
    KeInitializeEvent(gEvent_B, NotificationEvent, FALSE);
    RtlInitUnicodeString(&devName, L"\\Device\\PProtect");
    DriverObject->DriverUnload = PProtect_Unload;
    DriverObject->MajorFunction[IRP_MJ_CREATE] = PProtect_Create;
    DriverObject->MajorFunction[IRP_MJ_CLOSE] = PProtect_Close;
    DriverObject->MajorFunction[IRP_MJ_CLEANUP] = PProtect_Cleanup;
    DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL] = PProtect_DeivceIoControl;
    status = IoCreateDevice(
                 DriverObject,
                 0,
                 &devName,
                 FILE_DEVICE_UNKNOWN,
                 0,
                 FALSE,
                 &pdevObj
             );

    if (!NT_SUCCESS(status)) {
        return STATUS_FAILED_DRIVER_ENTRY;
    }

    //I/O读写方式为缓冲区方式
    DriverObject->Flags |= DO_BUFFERED_IO;
    RtlInitUnicodeString(&linkName, L"\\??\\PProtect");
    status = IoCreateSymbolicLink(&linkName, &devName);

    if (!NT_SUCCESS(status)) {
        IoDeleteDevice(pdevObj);
        return STATUS_FAILED_DRIVER_ENTRY;
    }

    DriverObject->Flags &= ~DO_DEVICE_INITIALIZING;
    //获取一些用到的偏移值及服务号
    status = Init_HCode();

    if (!NT_SUCCESS(status)) {
        return STATUS_FAILED_DRIVER_ENTRY;
    }

    RealAddr.NtCreateUserProcess = Hook_SSDT(NtCreateUserProcessID, myNtCreateUserProcess);
    return status;
}

VOID PProtect_Unload(IN PDRIVER_OBJECT DriverObject)
{
    PDEVICE_OBJECT pDevObj;
    UNICODE_STRING sysLinkName;
    KdPrintEx((xx, "[PProtect_Unload] ==>\n"));
    g_hMeID = NULL;

    if (pShareMM_User && pShareMM_MDL && pShareMM_SYS) {
        MmUnmapLockedPages(pShareMM_User, pShareMM_MDL);
        IoFreeMdl(pShareMM_MDL);
        ExFreePool(pShareMM_SYS);
        pShareMM_User = NULL;
        pShareMM_MDL = NULL;
        pShareMM_SYS = NULL;
    }

    ExFreePool(gMutex);
    Unhook(NtCreateUserProcessID, RealAddr.NtCreateUserProcess);
    RtlInitUnicodeString(&sysLinkName, L"\\??\\PProtect");
    pDevObj = DriverObject->DeviceObject;
    KdPrintEx((xx, "[PProtect_Unload] 删除符号链接……\n"));
    IoDeleteSymbolicLink(&sysLinkName);
    KdPrintEx((xx, "[PProtect_Unload] 删除CDO……\n"));
    IoDeleteDevice(pDevObj);
    KdPrintEx((xx, "[PProtect_Unload] <==\n"));
}

NTSTATUS PProtect_Create(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    PIO_STACK_LOCATION pIrpStack;
    NTSTATUS status = STATUS_SUCCESS;
    KdPrintEx((xx, "[PProtect_Create] ==>\n"));
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
    pIrpStack->FileObject->FsContext = NULL;
    pIrpStack->FileObject->FsContext2 = NULL;
    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);

    if (!g_hMeID) {
        KdPrintEx((xx, "[PProtect_Create] g_hMeID : 0x%x\n", g_hMeID));
        g_hMeID = PsGetCurrentProcessId();
    } else {
        if (PsGetCurrentProcessId() != g_hMeID) {
            KdPrintEx((xx, "[PProtect_Create] 非法打开驱动\n"));
            pIrp->IoStatus.Status = STATUS_UNSUCCESSFUL;
            return STATUS_UNSUCCESSFUL;
        }
    }

    KdPrintEx((xx, "[PProtect_Create] <==\n"));
    return STATUS_SUCCESS;
}

NTSTATUS PProtect_Close(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    PIO_STACK_LOCATION pIrpStack;
    NTSTATUS status = STATUS_SUCCESS;
    KdPrintEx((xx, "[PProtect_Close] ==>\n"));
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
    pIrpStack->FileObject->FsContext = NULL;
    pIrpStack->FileObject->FsContext2 = NULL;
    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    KdPrintEx((xx, "[PProtect_Close] <==\n"));
    return STATUS_SUCCESS;
}

NTSTATUS PProtect_Cleanup(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION pIrpStack;
    KdPrintEx((xx, "[PProtect_Cleanup] ==>\n"));
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
    pIrpStack->FileObject->FsContext = NULL;
    pIrpStack->FileObject->FsContext2 = NULL;
    pIrp->IoStatus.Information = 0;
    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    KdPrintEx((xx, "[PProtect_Cleanup] <==\n"));
    return status;
}

NTSTATUS PProtect_DeivceIoControl(IN PDEVICE_OBJECT DeviceObject, IN PIRP pIrp)
{
    NTSTATUS status = STATUS_SUCCESS;
    PIO_STACK_LOCATION pIrpStack;
    ULONG uControlCode = 0;
    PVOID pInBuffer = NULL;
    ULONG uTransferLen = 0;
    HANDLE EventA = 0;
    HANDLE EventB = 0;
    ULONG uFlagControl = 0;
    KdPrintEx((xx, "[PProtect_DeviceIoControl] ==>\n"));
    pIrpStack = IoGetCurrentIrpStackLocation(pIrp);
    uControlCode = pIrpStack->Parameters.DeviceIoControl.IoControlCode;
    pInBuffer = pIrp->AssociatedIrp.SystemBuffer;

    switch (uControlCode) {
    case IOCTL_SendFlagControl:
        //
        //检测参数的合法性
        //因为I/O控制方式为METHOD_NEITHER，必须检测。
        //
        _try {
            ProbeForRead(
                pIrpStack->Parameters.DeviceIoControl.Type3InputBuffer,
                sizeof(PVOID),
                sizeof(PVOID)
            );
        }
        _except(EXCEPTION_EXECUTE_HANDLER) {
            status = GetExceptionCode();
            break;
        }
        //获取控制标记
        uFlagControl = *(ULONG*)pIrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
        KdPrintEx((xx, "[IOCTL_SendFlagControl] uFlagControl:%d\n",
                   uFlagControl));
        GFlag.bPProtect = FlagOn(uFlagControl, fPProtect) ? TRUE : FALSE;
        GFlag.bAppRun = FlagOn(uFlagControl, fAppRun) ? TRUE : FALSE;

        //如果程序已经关闭了，则释放资源
        if (FALSE == GFlag.bAppRun) {
            KdPrintEx((xx, "[IOCTL_SendFlagControl] 应用程序已关闭!\n"));
            g_hMeID = NULL;

            if (gEvent_A) {
                ObDereferenceObject(gEvent_A);
                gEvent_A = NULL;
            }

            if (pShareMM_User && pShareMM_MDL && pShareMM_SYS) {
                MmUnmapLockedPages(pShareMM_User, pShareMM_MDL);
                IoFreeMdl(pShareMM_MDL);
                ExFreePool(pShareMM_SYS);
                pShareMM_User = NULL;
                pShareMM_MDL = NULL;
                pShareMM_SYS = NULL;
            }
        }

        break;

    case IOCTL_SendEventProMon:
        _try {
            ProbeForRead(
                pIrpStack->Parameters.DeviceIoControl.Type3InputBuffer,
                sizeof(PVOID),
                sizeof(PVOID)
            );
        }
        _except(EXCEPTION_EXECUTE_HANDLER) {
            status = GetExceptionCode();
            break;
        }
        //获取同步事件A，用于通知应用程序有新的进程创建动作
        EventA = (HANDLE)pIrpStack->Parameters.DeviceIoControl.Type3InputBuffer;
        status = ObReferenceObjectByHandle(
                     EventA,
                     GENERIC_ALL,
                     *ExEventObjectType,
                     KernelMode,
                     &gEvent_A,
                     NULL
                 );

        if (!NT_SUCCESS(status)) {
            KdPrintEx((xx, "[PProtect_DeivceIoControl] IOCTL_SendEventA ObReferenceObjectByHandle Failure!\n"));
            break;
        }

        break;

    case IOCTL_GetShareMemory:
        _try {
            //分配一块内核内存，用于与应用程序共享
            pShareMM_SYS = ExAllocatePool(NonPagedPool, 2048);

            if (NULL == pShareMM_SYS)
            {
                KdPrintEx((xx, "[PProtect_DeivceIoControl] IOCTL_GetShareMemory  pShareMM_SYS is NULL\n"));
                break;
            }

            RtlZeroMemory(pShareMM_SYS, 2048);

            //分配一个MDL描述这块内核内存
            pShareMM_MDL = IoAllocateMdl(
                pShareMM_SYS,
                2048,
                FALSE,
                FALSE,
                NULL
            );

            if (NULL == pShareMM_MDL)
            {
                KdPrintEx((xx, "[PProtect_DeivceIoControl] IOCTL_GetShareMemory IoAllocateMdl is NULL\n"));
                break;
            }

            //更新一下MDL数据结构，关联虚拟内存和物理内存
            MmBuildMdlForNonPagedPool(pShareMM_MDL);

            //映射此内存到应用程序
            pShareMM_User = MmMapLockedPagesSpecifyCache(
                pShareMM_MDL,
                UserMode,
                MmCached,
                NULL,
                FALSE,
                NormalPagePriority
            );

            if (NULL == pShareMM_User)
            {
                KdPrintEx((xx, "[PProtect_DeivceIoControl] IOCTL_GetShareMemory pShareMM_User is NULL\n"));
                break;
            }

            KdPrintEx((xx, "[PProtect_DeivceIoControl] pShareMM_SYS的地址为： 0x%p\n", (PUCHAR)pShareMM_SYS));
            KdPrintEx((xx, "[PProtect_DeivceIoControl] pShareMM_User的地址为： 0x%p\n", (PUCHAR)pShareMM_User));

            //将映射到用户程序中的地址返回给应用程序
            *((PVOID *)(pIrp->AssociatedIrp.SystemBuffer)) = pShareMM_User;

            status = STATUS_SUCCESS;

            uTransferLen = sizeof(PVOID);
        }
        _except(EXCEPTION_EXECUTE_HANDLER) {}
        break;

    case IOCTL_GetShareMemPointer:
        KdPrintEx((xx, "==>IOCTL_GetShareMemPointer pShareMM_User:0x%08x\n", pShareMM_User));
        *((PVOID *)(pIrp->AssociatedIrp.SystemBuffer)) = pShareMM_User;
        status = STATUS_SUCCESS;
        uTransferLen = sizeof(PVOID);
        KdPrintEx((xx, "<==IOCTL_GetShareMemPointer\n"));
        break;

    case IOCTL_Choose_T:
        //通知内核程序，判断逻辑已经做出了选择，放行此次进程创建操作
        iChoose = 1;

        if (gEvent_B) {
            KeSetEvent(gEvent_B, 0, FALSE);
        }

        break;

    case IOCTL_Choose_F:
        //通知内核程序，判断逻辑已经做出了选择，拦截此次进程创建操作
        iChoose = 0;

        if (gEvent_B) {
            KeSetEvent(gEvent_B, 0, FALSE);
        }

        break;
    }

    pIrp->IoStatus.Information = uTransferLen;
    pIrp->IoStatus.Status = status;
    IoCompleteRequest(pIrp, IO_NO_INCREMENT);
    KdPrintEx((xx, "[PProtect_DeviceIoControl] <==\n"));
    return status;
}

