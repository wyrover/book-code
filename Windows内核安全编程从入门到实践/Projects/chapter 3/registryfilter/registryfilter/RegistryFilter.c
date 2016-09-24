#include "ntddk.h"

LARGE_INTEGER liCookie;

NTSTATUS
ObQueryNameString(
    IN PVOID Object,
    OUT POBJECT_NAME_INFORMATION ObjectNameInfo,
    IN ULONG Length,
    OUT PULONG ReturnLength
);

void RegistryFilterUnload(IN PDRIVER_OBJECT DriverObject);


NTSTATUS
RegistryCallback(
    __in PVOID  CallbackContext,
    __in_opt PVOID  Argument1,
    __in_opt PVOID  Argument2
)
{
    NTSTATUS status;
    REG_NOTIFY_CLASS NotifyClass = (REG_NOTIFY_CLASS)Argument1;
    POBJECT_NAME_INFORMATION pNameInfo;
    ULONG uRet;

    switch (NotifyClass) {
    case RegNtDeleteKey:
        KdPrint(("\nRegNtDeleteKey:\n"));

        try {
            pNameInfo = (POBJECT_NAME_INFORMATION)ExAllocatePoolWithTag(
                            NonPagedPool,
                            sizeof(OBJECT_NAME_INFORMATION) + 1024,
                            'wjl'
                        );

            if (NULL == pNameInfo) {
                break;
            }

            status = ObQueryNameString(
                         ((PREG_DELETE_KEY_INFORMATION)Argument2)->Object,
                         pNameInfo,
                         sizeof(OBJECT_NAME_INFORMATION) + 1024,
                         &uRet
                     );

            if (!NT_SUCCESS(status)) {
                KdPrint(("ObQueryNameString Failure,err:0x%08x uRet:%d\n", status, uRet));
                ExFreePoolWithTag(pNameInfo, 'wjl');
                break;
            }

            KdPrint(("Delete Key Name: %wZ\n", &pNameInfo->Name));
            ExFreePoolWithTag(pNameInfo, 'wjl');
        }

        except(EXCEPTION_EXECUTE_HANDLER) {
            KdPrint(("RegNtDeleteKey except!!!\n"));
        }
        break;

    case RegNtSetValueKey:
        break;

    case RegNtDeleteValueKey:
        break;

    case RegNtSetInformationKey:
        break;

    case RegNtRenameKey:
        break;

    case RegNtEnumerateKey:
        break;

    case RegNtEnumerateValueKey:
        break;

    case RegNtQueryKey:
        break;

    case RegNtQueryValueKey:
        break;

    case RegNtQueryMultipleValueKey:
        break;

    case RegNtPreCreateKey:
        break;

    case RegNtPostCreateKey:
        break;

    case RegNtPreOpenKey:
        break;

    case RegNtPostOpenKey:
        break;

    case RegNtKeyHandleClose:
        break;

    default:
        KdPrint(("RegXxx\n"));
        break;
    }

    return STATUS_SUCCESS;
}


NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING  RegistryPath)
{
    NTSTATUS status;
    DriverObject->DriverUnload = RegistryFilterUnload;

    do {
        status = CmRegisterCallback(
                     (PEX_CALLBACK_FUNCTION)RegistryCallback,
                     NULL,
                     &liCookie
                 );

        if (!NT_SUCCESS(status)) {
            KdPrint(("CmRegisterCallback Failure!err:0x%08x\n", status));
            break;
        }
    } while (FALSE);

    return STATUS_SUCCESS;
}

void RegistryFilterUnload(IN PDRIVER_OBJECT DriverObject)
{
    CmUnRegisterCallback(liCookie);
}

