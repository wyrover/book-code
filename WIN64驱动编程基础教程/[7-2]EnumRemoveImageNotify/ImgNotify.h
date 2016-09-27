
ULONG64 FindPspLoadImageNotifyRoutine()
{
    ULONG64         i = 0, pCheckArea = 0;
    UNICODE_STRING  unstrFunc;
    RtlInitUnicodeString(&unstrFunc, L"PsSetLoadImageNotifyRoutine");
    pCheckArea = (ULONG64)MmGetSystemRoutineAddress(&unstrFunc);
    DbgPrint("PsSetLoadImageNotifyRoutine: %llx", pCheckArea);

    for (i = pCheckArea; i < pCheckArea + 0xff; i++) {
        if (*(PUCHAR)i == 0x48 && *(PUCHAR)(i + 1) == 0x8d && *(PUCHAR)(i + 2) == 0x0d) { //lea rcx,xxxx
            LONG OffsetAddr = 0;
            memcpy(&OffsetAddr, (PUCHAR)(i + 3), 4);
            return OffsetAddr + 7 + i;
        }
    }

    return 0;
}

void EnumLoadImageNotify()
{
    int i = 0;
    BOOLEAN b;
    ULONG64 NotifyAddr = 0, MagicPtr = 0;
    ULONG64 PspLoadImageNotifyRoutine = FindPspLoadImageNotifyRoutine();
    DbgPrint("PspLoadImageNotifyRoutine: %llx", PspLoadImageNotifyRoutine);

    if (!PspLoadImageNotifyRoutine)
        return;

    for (i = 0; i < 8; i++) {
        MagicPtr = PspLoadImageNotifyRoutine + i * 8;
        NotifyAddr = *(PULONG64)(MagicPtr);

        if (MmIsAddressValid((PVOID)NotifyAddr) && NotifyAddr != 0) {
            NotifyAddr = *(PULONG64)(NotifyAddr & 0xfffffffffffffff8);
            DbgPrint("[LoadImage]%llx", NotifyAddr);
        }
    }
}