//
//Description:
//  本程序用于展示使用IoAllocateIrp函数分配一个读请求IRP并完成该请求的过程。在XP SP3平台上测试通过。
//  使用vs2008+visualDDK+WDK7100编译
//
//Writen By: 宁妖 2011/4/21
//
//Last Write Time: 2011/4/21
//

#include "ReadFile.h"

PFILE_OBJECT KGetTargetFileObject(PUNICODE_STRING FileName);

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    NTSTATUS status;
    HANDLE hFile;
    PFILE_OBJECT pFileObj = NULL;
    UNICODE_STRING uFileName;
    PIRP pIrp = NULL;
    PDEVICE_OBJECT pFSDDevice = NULL;
    KEVENT kEvent;
    LARGE_INTEGER liOffset;
    PVOID pBuf = NULL;
    PMDL pMdl = NULL;
    IO_STATUS_BLOCK ioStatus;
    PIO_STACK_LOCATION pSP = NULL;
    DriverObject->DriverUnload = ReadFileUnload;

    do {
        //设置目标文件
        RtlInitUnicodeString(&uFileName, L"\\Device\\HarddiskVolume1\\test.txt");
        //获取目标文件的句柄
        hFile = KGetTargetFileHandle(&uFileName);

        if (INVALID_HANDLE_VALUE == hFile) {
            KdPrint(("[DriverEntry] KGetTargetFileHandle Failure!\n"));
            break;
        }

        //获取目标文件的对象地址
        status = ObReferenceObjectByHandle(hFile, GENERIC_READ, *IoFileObjectType, KernelMode,
                                           &pFileObj, NULL);

        if (!NT_SUCCESS(status)) {
            KdPrint(("[DriverEntry] ObReferenceObjectByHandle Failure!err:0x%08x\n",
                     status));
            ZwClose(hFile);
            break;
        }

        //获取目标文件关联的设备对象
        pFSDDevice = IoGetRelatedDeviceObject(pFileObj);
        KdPrint(("pFSDDevice:0x%08x\n", pFSDDevice));
        //分配一个IRP
        pIrp = IoAllocateIrp(pFSDDevice->StackSize, FALSE);

        if (NULL == pIrp) {
            KdPrint(("[DriverEntry] IoAllocateIrp Failure!\n"));
            break;
        }

        //初始化一个事件对象，用于同步完成该IRP
        KeInitializeEvent(&kEvent, SynchronizationEvent, FALSE);
        //
        //分配缓存，用于记录读信息
        //
        pBuf = ExAllocatePoolWithTag(NonPagedPool, 1024, 'wjl');

        if (NULL == pBuf) {
            KdPrint(("[DriverEntry] ExAllocatePoolWithTag Failure!\n"));
            IoFreeIrp(pIrp);
            break;
        }

        //
        //填写IRP头
        //
        pIrp->UserBuffer = pBuf;
        pIrp->UserEvent = &kEvent;
        pIrp->UserIosb = &ioStatus;
        pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
        pIrp->Tail.Overlay.OriginalFileObject = pFileObj;
        pIrp->RequestorMode = KernelMode;
        pIrp->Flags = IRP_READ_OPERATION;
        //
        //为下层驱动程序填写栈单元信息
        //
        pSP = IoGetNextIrpStackLocation(pIrp);
        pSP->MajorFunction = IRP_MJ_READ;
        pSP->MinorFunction = 0;
        pSP->DeviceObject = pFSDDevice;
        pSP->FileObject = pFileObj;
        //设置完成例程，用于获取读到的信息以及释放资源
        IoSetCompletionRoutine(pIrp, ReadIoCompletion, 0, TRUE, TRUE, TRUE);
        //设置读请求参数
        liOffset.QuadPart = 0;
        pSP->Parameters.Read.Length = 10;
        pSP->Parameters.Read.ByteOffset = liOffset;
        //下发该IRP
        IoCallDriver(pFSDDevice, pIrp);
        //同步等待该IRP
        KeWaitForSingleObject(&kEvent, Executive, KernelMode, TRUE, 0);
        KdPrint(("[DriverEntry] Read Finish!\n"));
        //关闭该文件
        ObDereferenceObject(pFileObj);
        ZwClose(hFile);
    } while (FALSE);

    return STATUS_SUCCESS;
}

void ReadFileUnload(IN PDRIVER_OBJECT DriverObject)
{
    KdPrint(("[ReadFileUnload] \n"));
}

HANDLE KGetTargetFileHandle(PUNICODE_STRING FileName)
{
    NTSTATUS status;
    OBJECT_ATTRIBUTES oa;
    HANDLE hFile;
    IO_STATUS_BLOCK ioStatus;
    PFILE_OBJECT pFileObj;
    InitializeObjectAttributes(&oa, FileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE,
                               NULL, NULL);
    status = IoCreateFile(
                 &hFile,
                 GENERIC_READ,
                 &oa,
                 &ioStatus,
                 NULL,
                 FILE_ATTRIBUTE_NORMAL,
                 FILE_SHARE_READ,
                 FILE_OPEN,
                 FILE_NON_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_ALERT,
                 NULL,
                 0,
                 CreateFileTypeNone,
                 NULL,
                 0
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[KGetTargetFileObject] IoCreateFile Failure!err:0x%08x\n",
                 status));
        return INVALID_HANDLE_VALUE;
    }

    return hFile;
}


NTSTATUS ReadIoCompletion(PDEVICE_OBJECT DeviceObject,
                          PIRP Irp,
                          PVOID Context)
{
    PVOID pBuf;
    *Irp->UserIosb = Irp->IoStatus;
    KeSetEvent(Irp->UserEvent, 0, FALSE);
    pBuf = Irp->UserBuffer;
    KdPrint(("[ReadIoCompletion] status:0x%08x infomation: %d\n", Irp->IoStatus.Status, Irp->IoStatus.Information));
    KdPrint(("[ReadIoCompletion] Read Buffer:0x%08x %s\n", pBuf, pBuf));
    ExFreePoolWithTag(pBuf, 'wjl');
    IoFreeIrp(Irp);
    return STATUS_MORE_PROCESSING_REQUIRED;
}