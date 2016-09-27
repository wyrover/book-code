#include <ntddk.h>
#include "mydriver.h"

VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    DbgPrint("EnumAntiMiniFilter unload\n");
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;
    EnumMiniFilter();
    return STATUS_SUCCESS;
}