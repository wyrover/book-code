
void CreateThreadNotify1
(
    IN HANDLE  ProcessId,
    IN HANDLE  ThreadId,
    IN BOOLEAN  Create
)
{
    DbgPrint("CreateThreadNotify1\n");
}

void CreateThreadNotify2
(
    IN HANDLE  ProcessId,
    IN HANDLE  ThreadId,
    IN BOOLEAN  Create
)
{
    DbgPrint("CreateThreadNotify2\n");
}

void CreateThreadNotifyTest(BOOLEAN Remove)
{
    if (!Remove) {
        PsSetCreateThreadNotifyRoutine(CreateThreadNotify1);
        PsSetCreateThreadNotifyRoutine(CreateThreadNotify2);
    } else {
        PsRemoveCreateThreadNotifyRoutine(CreateThreadNotify1);
        PsRemoveCreateThreadNotifyRoutine(CreateThreadNotify2);
    }
}

ULONG64 FindPspCreateThreadNotifyRoutine()
{
    ULONG64         i = 0, pCheckArea = 0;
    UNICODE_STRING  unstrFunc;
    RtlInitUnicodeString(&unstrFunc, L"PsSetCreateThreadNotifyRoutine");
    pCheckArea = (ULONG64)MmGetSystemRoutineAddress(&unstrFunc);
    DbgPrint("PsSetCreateThreadNotifyRoutine: %llx", pCheckArea);

    for (i = pCheckArea; i < pCheckArea + 0xff; i++) {
        if (*(PUCHAR)i == 0x48 && *(PUCHAR)(i + 1) == 0x8d && *(PUCHAR)(i + 2) == 0x0d) { //lea rcx,xxxx
            LONG OffsetAddr = 0;
            memcpy(&OffsetAddr, (PUCHAR)(i + 3), 4);
            return OffsetAddr + 7 + i;
        }
    }

    return 0;
}

void EnumCreateThreadNotify()
{
    int i = 0;
    BOOLEAN b;
    ULONG64 NotifyAddr = 0, MagicPtr = 0;
    ULONG64 PspCreateThreadNotifyRoutine = FindPspCreateThreadNotifyRoutine();
    DbgPrint("PspCreateThreadNotifyRoutine: %llx", PspCreateThreadNotifyRoutine);

    if (!PspCreateThreadNotifyRoutine)
        return;

    for (i = 0; i < 64; i++) {
        MagicPtr = PspCreateThreadNotifyRoutine + i * 8;
        NotifyAddr = *(PULONG64)(MagicPtr);

        if (MmIsAddressValid((PVOID)NotifyAddr) && NotifyAddr != 0) {
            NotifyAddr = *(PULONG64)(NotifyAddr & 0xfffffffffffffff8);
            DbgPrint("[CreateThread]%llx", NotifyAddr);
        }
    }
}