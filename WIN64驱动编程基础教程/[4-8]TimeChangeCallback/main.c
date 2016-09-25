#include <ntddk.h>

/*
    以下代码为功能测试
*/

NTKERNELAPI CHAR* PsGetProcessImageFileName(PEPROCESS Process);

PVOID g_CbRegistration;

VOID SetSystemTimeNotify
(
    IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
)
{
    //此回调函数的IRQL为2，大部分内核函数不能在里面使用！否则BSOD！
    DbgPrint("[SetSystemTimeNotify]IRQL: %ld\n", KeGetCurrentIrql());
}

NTSTATUS RegisterSetSystemTimeNotify(BOOLEAN IsUndo)
{
    if (!IsUndo) {
        PCALLBACK_OBJECT pCallBackObj;
        OBJECT_ATTRIBUTES oa ;
        UNICODE_STRING callbackname;
        NTSTATUS status;
        RtlInitUnicodeString(&callbackname, L"\\Callback\\SetSystemTime");
        InitializeObjectAttributes(&oa, &callbackname, OBJ_CASE_INSENSITIVE, 0, 0);
        status = ExCreateCallback(&pCallBackObj, &oa, TRUE, FALSE);

        if (!NT_SUCCESS(status))
            return status;

        g_CbRegistration = ExRegisterCallback(pCallBackObj, SetSystemTimeNotify , NULL);

        if (g_CbRegistration == NULL)
            return STATUS_UNSUCCESSFUL;

        ObDereferenceObject(pCallBackObj);
        return STATUS_SUCCESS;
    } else {
        if (g_CbRegistration == NULL)
            return STATUS_UNSUCCESSFUL;

        ExUnregisterCallback(g_CbRegistration);
        return STATUS_SUCCESS;
    }
}

/*
    以下代码为模板
*/

VOID DriverUnload(IN PDRIVER_OBJECT DriverObject)
{
    RegisterSetSystemTimeNotify(0);
    DbgPrint("[DriverUnload]\n");
}

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
    DriverObject->DriverUnload = DriverUnload;
    RegisterSetSystemTimeNotify(0);
    DbgPrint("[DriverEntry]\n");
    return STATUS_SUCCESS;
}