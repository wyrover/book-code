
BOOLEAN VxkCopyMemory(PVOID pDestination, PVOID pSourceAddress, SIZE_T SizeOfCopy)
{
    PMDL pMdl = NULL;
    PVOID pSafeAddress = NULL;
    pMdl = IoAllocateMdl(pSourceAddress, (ULONG)SizeOfCopy, FALSE, FALSE, NULL);

    if (!pMdl) return FALSE;

    __try {
        MmProbeAndLockPages(pMdl, KernelMode, IoReadAccess);
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        IoFreeMdl(pMdl);
        return FALSE;
    }

    pSafeAddress = MmGetSystemAddressForMdlSafe(pMdl, NormalPagePriority);

    if (!pSafeAddress) return FALSE;

    RtlCopyMemory(pDestination, pSafeAddress, SizeOfCopy);
    MmUnlockPages(pMdl);
    IoFreeMdl(pMdl);
    return TRUE;
}

ULONG64 rwkm_adr = 0, rwkm_len = 0;