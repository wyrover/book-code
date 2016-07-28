#include "DriverApp.h"

VOID ThreadStart(PVOID lpStartContext)
{
    PKEVENT pEvent = (PKEVENT)lpStartContext;
    DbgPrint("Hello! I am kernel thread. My ID is %u. Regards..", (ULONG)PsGetCurrentThreadId());
    KeSetEvent(pEvent, 0, 0);
    PsTerminateSystemThread(STATUS_SUCCESS);
}
NTSTATUS DriverEntry(PDRIVER_OBJECT theDriverObject, PUNICODE_STRING theRegistryPath)
{
    HANDLE hThread = NULL;
    NTSTATUS ntStatus = 0;
    OBJECT_ATTRIBUTES ThreadAttributes;
    KEVENT kEvent = { 0 };
    PETHREAD pThread = 0;
    theDriverObject->DriverUnload = OnUnload;
    DbgPrint("Entering KERNEL mode..");
    InitializeObjectAttributes(&ThreadAttributes, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);

    __try {
        KeInitializeEvent(&kEvent, SynchronizationEvent, 0);
        ntStatus = PsCreateSystemThread(&hThread, GENERIC_ALL, &ThreadAttributes, NULL, NULL, (PKSTART_ROUTINE) &ThreadStart, &kEvent);

        if (NT_SUCCESS(ntStatus)) {
            KeWaitForSingleObject(&kEvent, Executive, KernelMode, FALSE, NULL);
            ZwClose(hThread);
        } else {
            DbgPrint("Could not create system thread!");
        }
    } __except (EXCEPTION_EXECUTE_HANDLER) {
        DbgPrint("Error while creating system thread!");
    }

    return STATUS_SUCCESS;
}

VOID OnUnload(PDRIVER_OBJECT DriverObject)
{
    DbgPrint("Leaving KERNEL mode..");
}
