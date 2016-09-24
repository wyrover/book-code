//
//Description:
//  本程序用于扫描几种键盘记录行为并做恢复处理。在XP sp3平台上测试通过。
//扫描的键盘记录行为包括：
//              1.键盘过滤驱动
//              2.KeyboardClassRead调用点Hook以及inline hook
//              3.KeyboardClassServiceCallback调用点Hook以及inline hook
//
//Writen By: 宁妖 2011/3/30
//
//Last Write Time: 2011/4/1
//
#include "stdafx.h"

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    DriverObject->DriverUnload = KeylogerScanUnload;
    ScanKeyloger();
    return STATUS_SUCCESS;
}

void KeylogerScanUnload(IN PDRIVER_OBJECT DriverObject)
{
    return ;
}


NTSTATUS ScanKeyloger()
{
    NTSTATUS status;
    PContextData pData;
    //初始化一个ContextData结构，其中保存了很多信息，用于扫描恢复
    pData = InitContextData();

    if (NULL == pData) {
        KdPrint(("[ScanKeyloger] InitContextData Failure!\n"));
        return STATUS_UNSUCCESSFUL;
    }

    //扫描键盘过滤驱动并恢复
    KdPrint(("\n[ScanKeyloger] ScanKeyboardFilterDriver:\n"));
    status = ScanKeyboardFilterDriver(pData);

    if (NT_SUCCESS(status)) {
        KdPrint(("[ScanKeyloger] ScanKeyboardFilterDriver Finish!\n"));
    } else {
        KdPrint(("[ScanKeyloger] ScanKeyboardFilterDriver Abort!\n"));
    }

    //扫描KeyboardClassRead
    KdPrint(("\n[ScanKeyloger] ScanDispatchReadHook:\n"));
    status = ScanDispatchReadHook(pData);

    if (NT_SUCCESS(status)) {
        KdPrint(("[ScanKeyloger] ScanDispatchReadHook Finish!\n"));
    } else {
        KdPrint(("[ScanKeyloger] ScanDispatchReadHook Abort!\n"));
    }

    //扫描KeyboardClassServiceCallback
    KdPrint(("\n[ScanKeyloger] ScanServiceCallbackHook:\n"));
    status = ScanServiceCallbackHook(pData);

    if (NT_SUCCESS(status)) {
        KdPrint(("[ScanKeyloger] ScanServiceCallbackHook Finish!\n"));
    } else {
        KdPrint(("[ScanKeyloger] ScanServiceCallbackHook Abort!\n"));
    }

    //释放ContextData结构
    DeInitContextData(pData);
    return status;
}

PContextData InitContextData()
{
    PContextData pData;
    ULONG uCurKbdClassReadAddr;
    boolean bRet;
    pData = (PContextData)ExAllocatePoolWithTag(NonPagedPool, sizeof(ContextData), 'wjl');

    if (NULL == pData) {
        return NULL;
    }

    RtlZeroMemory(pData, sizeof(ContextData));
    //获取\\Device\\KeyboardClass0设备对象
    pData->KbdClass0DevObj = (PDEVICE_OBJECT)GetKbdClass0DeviceObject();

    if (NULL == pData->KbdClass0DevObj) {
        KdPrint(("[InitContextData] GetKbdClass0DeviceObject Failure!\n"));
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //获取Kbdclass驱动对象
    pData->KbdClassDrvObj = pData->KbdClass0DevObj->DriverObject;
    //将PE文件内容读到内存中，从而解析原始函数地址
    pData->KbdClassDriverFileData = ReadKbdClassDriverFileData();

    if (NULL == pData->KbdClassDriverFileData) {
        KdPrint(("[InitContextData] ReadKbdClassDriverFileData Failure!\n"));
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //从PE文件（kbdclass.sys）中获取KbdClass驱动的入口函数（DriverEntry）地址
    bRet = GetReadKbdClassDriverEntryAddrFromPE(pData);

    if (false == bRet) {
        KdPrint(("[InitContextData] GetReadKbdClassDriverEntryAddrFromPE Failure!\n"));
        ExFreePoolWithTag(pData->KbdClassDriverFileData, 'wjl');
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //从PE文件中获取KeyboardClassRead的地址
    bRet = GetRealKbdClassReadAddrFromPE(pData);

    if (false == bRet) {
        KdPrint(("[InitContextData] GetRealKbdClassReadAddrFromPE Failure!\n"));
        ExFreePoolWithTag(pData->KbdClassDriverFileData, 'wjl');
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    //从PE文件中获取KeyboardClassServiceCallback的地址
    bRet = GetKbdServiceCallBackAddrFromPE(pData);

    if (false == bRet) {
        KdPrint(("[InitContextData] GetKbdServiceCallBackAddrFromPE Failure!\n"));
        ExFreePoolWithTag(pData->KbdClassDriverFileData, 'wjl');
        ExFreePoolWithTag(pData, 'wjl');
        pData = NULL;
        return NULL;
    }

    KdPrint(("[InitContextData] Init Finish! KbdClass Info:\n\
DriverObject: 0x%08x\n\
DeviceObject: 0x%08x\n\
KeyboardClassRead: 0x%08x\n\
KeyboardClassServiceCallback: 0x%08x\n",
             pData->KbdClassDrvObj, pData->KbdClass0DevObj,
             pData->KbdClassReadAddr, pData->KbdServiceCallbackAddr
            ));
    return pData;
}

VOID DeInitContextData(PContextData Data)
{
    if (!Data) {
        return ;
    }

    if (Data->KbdClassDriverFileData) {
        ExFreePoolWithTag(Data->KbdClassDriverFileData, 'wjl');
        Data->KbdClassDriverFileData = NULL;
    }

    ExFreePoolWithTag(Data, 'wjl');
    return ;
}

PDEVICE_OBJECT GetKbdClass0DeviceObject()
{
    NTSTATUS status;
    UNICODE_STRING usKbdClass0;
    PDEVICE_OBJECT pDevObj;
    PFILE_OBJECT pFileObject;
    HANDLE hFile;
    OBJECT_ATTRIBUTES oa;
    IO_STATUS_BLOCK ioStatus;
    RtlInitUnicodeString(&usKbdClass0, L"\\Device\\KeyboardClass0");
    InitializeObjectAttributes(&oa, &usKbdClass0, OBJ_CASE_INSENSITIVE, NULL, NULL);
    status = ZwCreateFile(
                 &hFile,
                 GENERIC_READ,
                 &oa,
                 &ioStatus,
                 NULL,
                 FILE_ATTRIBUTE_NORMAL,
                 FILE_SHARE_READ,
                 FILE_OPEN,
                 FILE_SYNCHRONOUS_IO_NONALERT,
                 NULL,
                 0
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[GetKbdClass0DeviceObject] ZwCreateFile Failure!err:0x%08x\n",
                 status));
        return NULL;
    }

    status = ObReferenceObjectByHandle(
                 hFile,
                 GENERIC_READ,
                 *IoFileObjectType,
                 KernelMode,
                 (PVOID*)&pFileObject,
                 NULL
             );

    if (!NT_SUCCESS(status)) {
        KdPrint(("[GetKbdClass0DeviceObject] ObReferenceObjectByHandle Failure!err:0x%08x\n",
                 status));
        ZwClose(hFile);
        return NULL;
    }

    ObDereferenceObject(pFileObject);
    ZwClose(hFile);
    pDevObj = pFileObject->DeviceObject;
    return pDevObj;
}

//
//检查是否有键盘过滤驱动，如果有直接摘除
//
NTSTATUS ScanKeyboardFilterDriver(PContextData Data)
{
    PDEVICE_OBJECT pDevObj;
    //AttachedDevice记录了绑定在其上的设备对象
    pDevObj = Data->KbdClass0DevObj->AttachedDevice;

    if (pDevObj) {
        KdPrint(("[ScanKeyboardFilterDriver] Find Keyboard Filter Driver:%wZ\n",
                 &pDevObj->DriverObject->DriverName));
        IoDetachDevice(Data->KbdClass0DevObj);
    }

    return STATUS_SUCCESS;
}

//
//检查读请求分发函数，包括调用点检测和inlinehook检测
//
NTSTATUS ScanDispatchReadHook(PContextData Data)
{
    ULONG uReadEntry, uFnRVA;
    //
    //1.调用点检测
    //
    //获取内存中的读请求函数调用点
    uReadEntry = (ULONG)&Data->KbdClassDrvObj->MajorFunction[IRP_MJ_READ];

    //对比内存中的读请求函数地址和从PE文件中获取到的读请求地址是否一致
    //如果不一致，说明被Hook了，恢复。
    if (*(ULONG*)uReadEntry != Data->KbdClassReadAddr) {
        KdPrint(("[ScanDispatchReadHook] KbdClass Dispatch Hooked!Recover...\n"));
        InterlockedExchange((LONG*)uReadEntry, Data->KbdClassReadAddr);
    }

    KdPrint(("[ScanDispatchReadHook] Scan call entry hook finish.\n"));
    //
    //2.inlinehook检测并恢复
    //
    uFnRVA = (ULONG)Data->KbdClassReadAddr - (ULONG)Data->KbdClassDrvObj->DriverStart;
    ScanInlineHook(Data, (PVOID)uFnRVA);
    return STATUS_SUCCESS;
}

//
//检查KeyboardClassServiceCallback函数，包括调用点检测和inlinehook检测
//
NTSTATUS ScanServiceCallbackHook(PContextData Data)
{
    ULONG uFnRVAAddr = 0;
    uFnRVAAddr = Data->KbdServiceCallbackAddr - (ULONG)Data->KbdClassDrvObj->DriverStart;
    //1.调用点检测
    ScanKbdServiceCallbackEntryHook(Data);
    //2.inlinehook检测
    ScanInlineHook(Data, (PVOID)uFnRVAAddr);
    return true;
}

//KeyboardClassServiceCallback函数的调用点检测函数
boolean ScanKbdServiceCallbackEntryHook(PContextData Data)
{
    PDRIVER_OBJECT pDrvObj;
    PDEVICE_OBJECT pDevObj;
    ULONG uEntryAddr;
    UNICODE_STRING usI8042;
    //本函数仅支持PS/2键盘
    RtlInitUnicodeString(&usI8042, L"\\Driver\\i8042prt");
    pDevObj = (PDEVICE_OBJECT) * (ULONG*)((ULONG)Data->KbdClass0DevObj->DeviceObjectExtension + 0x18);
    pDrvObj = pDevObj->DriverObject;

    if (RtlCompareUnicodeString(&pDrvObj->DriverName, &usI8042, true))
        return false;

    //按下键盘，触发中断，将执行DPC例程I8042KeyboardIsrDpc，
    //在I8042KeyboardIsrDpc函数中，将会调用KeyboardClassServiceCallback，
    //通过逆向获得，调用点为DeviceExtension+0xf8。
    uEntryAddr = (ULONG)pDevObj->DeviceExtension + 0xf8;

    //对比内存中调用点保存的地址和从PE文件中获取的KeyboardClassServiceCallback地址是否一致
    //如果不一致，恢复。
    if (*(ULONG*)uEntryAddr != Data->KbdServiceCallbackAddr) {
        InterlockedExchange((LONG*)uEntryAddr, Data->KbdServiceCallbackAddr);
    }

    return true ;
}

//
//inlinehook检测，使用libdasm反汇编库
//
boolean ScanInlineHook(PContextData Data, PVOID FnRVA)
{
    PIMAGE_DOS_HEADER pDosHdr;
    PIMAGE_NT_HEADERS pNtHdr;
    ULONG uFnRAW = 0;
    INSTRUCTION inst;
    ULONG uFnLen = 0, uCodeLen = 0, uOffset = 0;
    PVOID pCode = NULL;
    char cInstCode[50] = {0};
    int i = 0;
    boolean bRecover = false;
    pDosHdr = (PIMAGE_DOS_HEADER)Data->KbdClassDriverFileData;

    if (IMAGE_DOS_SIGNATURE != pDosHdr->e_magic) {
        return false;
    }

    pNtHdr = (PIMAGE_NT_HEADERS)((ULONG)pDosHdr + pDosHdr->e_lfanew);
    //将相对虚拟地址转换为文件偏移
    uFnRAW = RVA2RAW(Data->KbdClassDriverFileData, (ULONG)FnRVA);
    //获取待检测函数的长度
    uFnLen = GetFunctionLen((ULONG)Data->KbdClassDriverFileData + uFnRAW, 0xc2);
    pCode = ExAllocatePoolWithTag(PagedPool, uFnLen, 'wjl');

    if (NULL == pCode) {
        return false;
    }

    //将待检测函数代码拷贝到分配的内核缓冲区中，进行重定位。
    RtlCopyMemory(pCode, (PVOID)((ULONG)Data->KbdClassDriverFileData + uFnRAW), uFnLen);
    //重定位的偏移值：内存中实际加载的地址和PE文件中的首选加载地址之差
    uOffset = (ULONG)Data->KbdClassDrvObj->DriverStart - pNtHdr->OptionalHeader.ImageBase;
    //根据PE文件中的重定位目录信息实施重定位操作
    RelocCode(Data->KbdClassDriverFileData, FnRVA, (BYTE*)pCode, uFnLen, uOffset);
    //
    //将重定位后的代码逐条反汇编，和内存中相应的代码比对，若不一致，
    //恢复。
    //
    uFnLen = 0;

    do {
        uCodeLen = get_instruction(&inst, (BYTE*)((ULONG)pCode + uFnLen), MODE_32);

        if (0 == uCodeLen) {
            break;
        }

        if (uCodeLen != RtlCompareMemory(
                (PVOID)((ULONG)pCode + uFnLen),
                (PVOID)((ULONG)Data->KbdClassDrvObj->DriverStart + (ULONG)FnRVA + uFnLen),
                uCodeLen)
           ) {
            get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);
            KdPrint(("[ScanInlineHook] Find InlineHooke!\n"));
            KdPrint(("Clean Code: %x %s\n", uFnLen, cInstCode));
            get_instruction(&inst, (BYTE*)((ULONG)Data->KbdClassDrvObj->DriverStart + (ULONG)FnRVA + uFnLen), MODE_32);
            get_instruction_string(&inst, FORMAT_INTEL, 0, cInstCode, 50);
            KdPrint(("Dirty Code: %x %s\n", uFnLen, cInstCode));
            bRecover = true;
            break;
        }

        if ((inst.opcode == 0xc2) || (inst.opcode == 0xc3)) {
            break;
        }

        uFnLen += uCodeLen;
    } while (i++ < 1000);

    //恢复操作
    if (true == bRecover) {
        KdPrint(("Start to Recover whole Function...\n"));
        RecoverKnrlMemory((PVOID)((ULONG)Data->KbdClassDrvObj->DriverStart + (ULONG)FnRVA), pCode, uFnLen);
    }

    ExFreePoolWithTag(pCode, 'wjl');
    KdPrint(("[ScanInlineHook] Scan inlinehook finish!\n"));
    return true;
}

//
//恢复内存，将干净的代码覆盖脏的代码。
//需要注意的是，本函数有一定的危险性：提升IRQL和屏蔽中断只能保证
//单核CPU的安全，对于多核情况，如果恢复一半的时候函数被调用则有
//可能出现蓝屏，不过这种概率很小。
//
boolean RecoverKnrlMemory(PVOID Dest, PVOID Source, ULONG Offset)
{
    MDL *pMdl;
    PVOID pMdlAddr = NULL;
    ULONG uFnLen = 0;
    KIRQL OldIrql;
    uFnLen = GetFunctionLen((ULONG)Dest, 0xc2);
    //使用MDL修改代码
    pMdl = IoAllocateMdl(Dest, uFnLen, FALSE, FALSE, NULL);

    if (pMdl == NULL)
        return false;

    __try {
        MmProbeAndLockPages(pMdl, KernelMode, IoModifyAccess);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        IoFreeMdl(pMdl);
        return false;
    }

    pMdlAddr = MmGetSystemAddressForMdl(pMdl);
    //提升IRQL，阻止线程分发
    OldIrql = KeRaiseIrqlToDpcLevel();
    //屏蔽可屏蔽中断，包括键盘中断。
    //这样保证在恢复KeyboardClassServiceCallback过程中不会受键盘中断影响。
    _asm cli
    memmove((PVOID)((ULONG)pMdlAddr + Offset), (PVOID)((ULONG)Source + Offset), uFnLen - Offset);
    _asm sti
    KeLowerIrql(OldIrql);
    MmUnlockPages(pMdl);
    IoFreeMdl(pMdl);
    return true;
}