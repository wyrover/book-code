#include <ntddk.h>

void test()
{
    PVOID ptr1 = ExAllocatePool(PagedPool, 0x100);
    PVOID ptr2 = ExAllocatePool(NonPagedPool, 0x200);
    RtlFillMemory(ptr2, 0x200, 0x90);
    RtlMoveMemory(ptr1, ptr2, 0x50);
    ExFreePool(ptr1);
    ExFreePool(ptr2);
}

//以下两个函数，在HOOK修改内存时使用
KIRQL WPOFFx64()
{
    KIRQL irql = KeRaiseIrqlToDpcLevel();
    UINT64 cr0 = __readcr0();
    cr0 &= 0xfffffffffffeffff;
    __writecr0(cr0);
    _disable();
    return irql;
}

void WPONx64(KIRQL irql)
{
    UINT64 cr0 = __readcr0();
    cr0 |= 0x10000;
    _enable();
    __writecr0(cr0);
    KeLowerIrql(irql);
}

//此函数有点吹毛求疵，不推荐使用，因为效率太低了。
BOOLEAN SafeCopyMemory(PVOID pDestination, PVOID pSourceAddress, SIZE_T SizeOfCopy)
{
    PMDL pMdl = NULL;
    PVOID pSafeAddress = NULL;

    if (!MmIsAddressValid(pDestination) || !MmIsAddressValid(pSourceAddress))
        return FALSE;

    pMdl = IoAllocateMdl(pDestination, (ULONG)SizeOfCopy, FALSE, FALSE, NULL);

    if (!pMdl)
        return FALSE;

    __try {
        MmProbeAndLockPages(pMdl, KernelMode, IoReadAccess);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        IoFreeMdl(pMdl);
        return FALSE;
    }

    pSafeAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);

    if (!pSafeAddress)
        return FALSE;

    __try {
        RtlMoveMemory(pSafeAddress, pSourceAddress, SizeOfCopy);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        ;
    }

    MmUnlockPages(pMdl);
    IoFreeMdl(pMdl);
    return TRUE;
}

VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    DbgPrint("[Memory]Unload...\n");
    return;
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;
    DbgPrint("[Memory]Load...\n");
    test();
    return STATUS_SUCCESS;
}