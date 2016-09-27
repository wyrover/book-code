
ULONG64 FindPspCreateProcessNotifyRoutine()
{
    LONG            OffsetAddr = 0;
    ULONG64         i = 0, pCheckArea = 0;
    UNICODE_STRING  unstrFunc;
    //获得PsSetCreateProcessNotifyRoutine的地址
    RtlInitUnicodeString(&unstrFunc, L"PsSetCreateProcessNotifyRoutine");
    pCheckArea = (ULONG64)MmGetSystemRoutineAddress(&unstrFunc);
    //获得PspSetCreateProcessNotifyRoutine的地址
    memcpy(&OffsetAddr, (PUCHAR)pCheckArea + 4, 4);
    pCheckArea = (pCheckArea + 3) + 5 + OffsetAddr;
    DbgPrint("PspSetCreateProcessNotifyRoutine: %llx", pCheckArea);

    //获得PspCreateProcessNotifyRoutine的地址
    for (i = pCheckArea; i < pCheckArea + 0xff; i++) {
        if (*(PUCHAR)i == 0x4c && *(PUCHAR)(i + 1) == 0x8d && *(PUCHAR)(i + 2) == 0x35) { //lea r14,xxxx
            LONG OffsetAddr = 0;
            memcpy(&OffsetAddr, (PUCHAR)(i + 3), 4);
            return OffsetAddr + 7 + i;
        }
    }

    return 0;
}

void EnumCreateProcessNotify()
{
    int i = 0;
    BOOLEAN b;
    ULONG64 NotifyAddr = 0, MagicPtr = 0;
    ULONG64 PspCreateProcessNotifyRoutine = FindPspCreateProcessNotifyRoutine();
    DbgPrint("PspCreateProcessNotifyRoutine: %llx", PspCreateProcessNotifyRoutine);

    if (!PspCreateProcessNotifyRoutine)
        return;

    for (i = 0; i < 64; i++) {
        MagicPtr = PspCreateProcessNotifyRoutine + i * 8;
        NotifyAddr = *(PULONG64)(MagicPtr);

        if (MmIsAddressValid((PVOID)NotifyAddr) && NotifyAddr != 0) {
            NotifyAddr = *(PULONG64)(NotifyAddr & 0xfffffffffffffff8);
            DbgPrint("[CreateProcess]%llx", NotifyAddr);
        }
    }
}