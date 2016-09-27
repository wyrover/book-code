typedef struct _CM_NOTIFY_ENTRY {
    LIST_ENTRY      ListEntryHead;
    ULONG           UnKnown1;
    ULONG           UnKnown2;
    LARGE_INTEGER   Cookie;
    ULONG64         Context;
    ULONG64         Function;
} CM_NOTIFY_ENTRY, *PCM_NOTIFY_ENTRY;

LARGE_INTEGER Cookie;
ULONG64 CmCallbackListHead = 0;

NTSTATUS MyRegistryCallback(
    IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
)
{
    return STATUS_SUCCESS;
}

ULONG CountCmpCallbackAfterXP(ULONG64* pPspLINotifyRoutine)
{
    ULONG               sum = 0;
    ULONG64             dwNotifyItemAddr;
    ULONG64*            pNotifyFun;
    ULONG64*            baseNotifyAddr;
    ULONG64             dwNotifyFun;
    LARGE_INTEGER       cmpCookie;
    PLIST_ENTRY         notifyList;
    PCM_NOTIFY_ENTRY    notify;
    dwNotifyItemAddr = *pPspLINotifyRoutine;
    notifyList = (LIST_ENTRY *)dwNotifyItemAddr;

    do {
        notify = (CM_NOTIFY_ENTRY *)notifyList;

        if (MmIsAddressValid(notify)) {
            if (MmIsAddressValid((PVOID)(notify->Function)) && notify->Function > 0x8000000000000000) {
                DbgPrint("[CmCallback]Function=%p\tCookie=%p", (PVOID)(notify->Function), (PVOID)(notify->Cookie.QuadPart));
                //notify->Function=(ULONG64)MyRegistryCallback;
                sum ++;
            }
        }

        notifyList = notifyList->Flink;
    } while (notifyList != ((LIST_ENTRY*)(*pPspLINotifyRoutine)));

    return sum;
}

LONG64 MakeLong64ByLong32(LONG lng32)
{
    LONG64 lng64 = 0;

    if (lng32 > 0) {
        lng64 = (LONG64)lng32;
    } else {
        lng64 = 0xffffffffffffffff;
        memcpy(&lng64, &lng32, 4);
    }

    return lng64;
}

ULONG64 FindCmpCallbackAfterXP()
{
    ULONG64         uiAddress = 0;
    PUCHAR          pCheckArea = NULL, i = 0, j = 0, StartAddress = 0, EndAddress = 0;
    ULONG64         dwCheckAddr = 0;
    UNICODE_STRING  unstrFunc;
    UCHAR b1 = 0, b2 = 0, b3 = 0;
    ULONG templong = 0, QuadPart = 0xfffff800;
    RtlInitUnicodeString(&unstrFunc, L"CmUnRegisterCallback");
    pCheckArea = (UCHAR*)MmGetSystemRoutineAddress(&unstrFunc) ;

    if (!pCheckArea) {
        KdPrint(("MmGetSystemRoutineAddress failed."));
        return 0;
    }

    StartAddress = (PUCHAR)pCheckArea;
    EndAddress = (PUCHAR)pCheckArea + PAGE_SIZE;

    for (i = StartAddress; i < EndAddress; i++) {
        if (MmIsAddressValid(i) && MmIsAddressValid(i + 1) && MmIsAddressValid(i + 2)) {
            b1 = *i;
            b2 = *(i + 1);
            b3 = *(i + 2);

            if (b1 == 0x48 && b2 == 0x8d && b3 == 0x0d) { //488d0d(lea rcx,)
                j = i - 5;
                b1 = *j;
                b2 = *(j + 1);
                b3 = *(j + 2);

                if (b1 == 0x48 && b2 == 0x8d && b3 == 0x54) { //488d54(lea rdx,)
                    memcpy(&templong, i + 3, 4);
                    uiAddress = MakeLong64ByLong32(templong) + (ULONGLONG)i + 7;
                    return uiAddress;
                }
            }
        }
    }

    return 0;
}

void EnumCmCallback64()
{
    //test to add my reg callback
    CmRegisterCallback(MyRegistryCallback, NULL, &Cookie);
    DbgPrint("[MY FUNCTION]: %p", (PVOID)MyRegistryCallback);
    DbgPrint("[MY COOKIE]: %p", (PVOID)Cookie.QuadPart);
    //get CmCallbackListHead address
    CmCallbackListHead = FindCmpCallbackAfterXP();
    DbgPrint("CmCallbackListHead: %p", (PVOID)CmCallbackListHead);
    //enum callback address
    CountCmpCallbackAfterXP((ULONG64*)CmCallbackListHead);
    //unregister my callback
    CmUnRegisterCallback(Cookie);
}

/*
    给函数头写RET废除函数功能，这份代码可以用于对抗任意回调函数，但仅限于WIN64系统。
    DisableFunctionWithReturnValue用于有返回值的回调
    DisableFunctionWithoutReturnValue用于无返回值的回调
*/

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

VOID DisableFunctionWithReturnValue(PVOID Address)
{
    KIRQL irql;
    CHAR patchCode[] = "\x33\xC0\xC3";  //xor eax,eax + ret

    if (MmIsAddressValid(Address)) {
        irql = WPOFFx64();
        memcpy(Address, patchCode, 3);
        WPONx64(irql);
    }
}

VOID DisableFunctionWithoutReturnValue(PVOID Address)
{
    KIRQL irql;

    if (MmIsAddressValid(Address)) {
        irql = WPOFFx64();
        RtlFillMemory(Address, 1, 0xC3);
        WPONx64(irql);
    }
}