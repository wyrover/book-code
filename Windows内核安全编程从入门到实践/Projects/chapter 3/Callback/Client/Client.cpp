#include "stdafx.h"

PCALLBACK_OBJECT pCallback;
PVOID  pRegisterHandle = NULL;

VOID
CallbackFunction(
    IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
);

void ClientUnload(IN PDRIVER_OBJECT DriverObject);

#ifdef __cplusplus
    extern "C" NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath);
#endif

NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    NTSTATUS status;
    UNICODE_STRING DemoCallback;
    OBJECT_ATTRIBUTES oa;
    DriverObject->DriverUnload = ClientUnload;

    do {
        //打开回调对象
        RtlInitUnicodeString(&DemoCallback, L"\\Callback\\DemoCallback");
        InitializeObjectAttributes(&oa, &DemoCallback, OBJ_CASE_INSENSITIVE | OBJ_PERMANENT, NULL, NULL);
        //指定ExCreateCallback的Create参数为FALSE，即打开回调对象而不是创建
        status = ExCreateCallback(&pCallback, &oa, FALSE, TRUE);

        if (!NT_SUCCESS(status)) {
            KdPrint(("Client: ExCreateCallback Failure!Status:0x%08x\n", status));
            break;
        }

        KdPrint(("Client ——pCallback: 0x%08x\n", pCallback));
        //向该回调对象注册一个回调函数
        pRegisterHandle = ExRegisterCallback(pCallback, (PCALLBACK_FUNCTION)CallbackFunction, NULL);

        if (NULL == pRegisterHandle) {
            KdPrint(("Client: ExRegisterCallback Failure!\n"));
            ObDereferenceObject(pCallback);
            break;
        }

        //不需要回调对象时，减少引用次数（减少到0时，将会被删除）。
        ObDereferenceObject(pCallback);
    } while (FALSE);

    return STATUS_SUCCESS;
}

void ClientUnload(IN PDRIVER_OBJECT DriverObject)
{
    if (pRegisterHandle != NULL)
        ExUnregisterCallback(pRegisterHandle);
}

//
//回调函数
//  当有程序打开或者关闭Callback驱动程序时，Callback驱动将调用此函数。
//  本函数只是简单地输出Argument1和Argument2。Argument1和Argument2的
//语义由创建回调对象的驱动程序（本例中即Callback）定义：
//  Argument1:IRP的主功能码（IRP_MJ_CREATE或者IRP_MJ_CLOSE）。
//  Argument2:打开或关闭callback驱动的进程名
//
VOID
CallbackFunction(
    IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
)
{
    KdPrint(("Client CallbackFunction—— Argument1: %s Argument2:%s\n",
             ((LONG)Argument1 == IRP_MJ_CREATE) ? "IRP_MJ_CREATE" : (((LONG)Argument1 == IRP_MJ_CLOSE) ? "IRP_MJ_CLOSE" : "exception!"),
             Argument2));
    return ;
}