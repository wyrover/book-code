#include <ntddk.h>

#define REGISTRY_POOL_TAG 'pRE'

NTKERNELAPI NTSTATUS ObQueryNameString
(
    IN  PVOID Object,
    OUT POBJECT_NAME_INFORMATION ObjectNameInfo,
    IN  ULONG Length,
    OUT PULONG ReturnLength
);

NTKERNELAPI NTSTATUS RtlUnicodeStringCopy
(
    __out  PUNICODE_STRING DestinationString,
    __in   PUNICODE_STRING SourceString
);

NTKERNELAPI UCHAR* PsGetProcessImageFileName(PEPROCESS Process);

LARGE_INTEGER CmHandle;
NTSTATUS CmSt;

BOOLEAN IsProcessName(char *string, PEPROCESS eprocess)
{
    char xx[260] = {0};
    strcpy(xx, PsGetProcessImageFileName(eprocess));

    if (!_stricmp(xx, string))
        return TRUE;
    else
        return FALSE;
}

BOOLEAN GetRegistryObjectCompleteName(PUNICODE_STRING pRegistryPath, PUNICODE_STRING pPartialRegistryPath, PVOID pRegistryObject)
{
    BOOLEAN foundCompleteName = FALSE;
    BOOLEAN partial = FALSE;

    if ((!MmIsAddressValid(pRegistryObject)) || (pRegistryObject == NULL))
        return FALSE;

    /* Check to see if the partial name is really the complete name */
    if (pPartialRegistryPath != NULL) {
        if ((((pPartialRegistryPath->Buffer[0] == '\\') || (pPartialRegistryPath->Buffer[0] == '%')) ||
             ((pPartialRegistryPath->Buffer[0] == 'T') && (pPartialRegistryPath->Buffer[1] == 'R') &&
              (pPartialRegistryPath->Buffer[2] == 'Y') && (pPartialRegistryPath->Buffer[3] == '\\')))) {
            RtlCopyUnicodeString(pRegistryPath, pPartialRegistryPath);
            partial = TRUE;
            foundCompleteName = TRUE;
        }
    }

    if (!foundCompleteName) {
        /* Query the object manager in the kernel for the complete name */
        NTSTATUS status;
        ULONG returnedLength;
        PUNICODE_STRING pObjectName = NULL;
        status = ObQueryNameString(pRegistryObject, (POBJECT_NAME_INFORMATION)pObjectName, 0, &returnedLength);

        if (status == STATUS_INFO_LENGTH_MISMATCH) {
            pObjectName = ExAllocatePoolWithTag(NonPagedPool, returnedLength, REGISTRY_POOL_TAG);
            status = ObQueryNameString(pRegistryObject, (POBJECT_NAME_INFORMATION)pObjectName, returnedLength, &returnedLength);

            if (NT_SUCCESS(status)) {
                RtlCopyUnicodeString(pRegistryPath, pObjectName);
                foundCompleteName = TRUE;
            }

            ExFreePoolWithTag(pObjectName, REGISTRY_POOL_TAG);
        }
    }

    return foundCompleteName;
}

NTSTATUS RegistryCallback
(
    IN PVOID CallbackContext,
    IN PVOID Argument1,
    IN PVOID Argument2
)
{
    long type;
    NTSTATUS CallbackStatus = STATUS_SUCCESS;
    UNICODE_STRING registryPath;
    registryPath.Length = 0;
    registryPath.MaximumLength = 2048 * sizeof(WCHAR);
    registryPath.Buffer = ExAllocatePoolWithTag(NonPagedPool, registryPath.MaximumLength, REGISTRY_POOL_TAG);

    if (registryPath.Buffer == NULL)
        return STATUS_SUCCESS;

    type = (REG_NOTIFY_CLASS)Argument1;

    switch (type) {
    case RegNtPreCreateKeyEx: { //出现两次是因为一次是OpenKey，一次是createKey
        if (IsProcessName("regedit.exe", PsGetCurrentProcess())) {
            GetRegistryObjectCompleteName(&registryPath, NULL, ((PREG_CREATE_KEY_INFORMATION)Argument2)->RootObject);
            DbgPrint("[RegNtPreCreateKeyEx]KeyPath: %wZ", &registryPath);   //新键的路径
            DbgPrint("[RegNtPreCreateKeyEx]KeyName: %wZ",
                     ((PREG_CREATE_KEY_INFORMATION)Argument2)->CompleteName);//新键的名称
            CallbackStatus = STATUS_ACCESS_DENIED;
        }

        break;
    }

    case RegNtPreDeleteKey: {
        if (IsProcessName("regedit.exe", PsGetCurrentProcess())) {
            GetRegistryObjectCompleteName(&registryPath, NULL, ((PREG_DELETE_KEY_INFORMATION)Argument2)->Object);
            DbgPrint("[RegNtPreDeleteKey]%wZ", &registryPath);              //新键的路径
            CallbackStatus = STATUS_ACCESS_DENIED;
        }

        break;
    }

    case RegNtPreSetValueKey: {
        if (IsProcessName("regedit.exe", PsGetCurrentProcess())) {
            GetRegistryObjectCompleteName(&registryPath, NULL, ((PREG_SET_VALUE_KEY_INFORMATION)Argument2)->Object);
            DbgPrint("[RegNtPreSetValueKey]KeyPath: %wZ", &registryPath);
            DbgPrint("[RegNtPreSetValueKey]ValName: %wZ", ((PREG_SET_VALUE_KEY_INFORMATION)Argument2)->ValueName);
            CallbackStatus = STATUS_ACCESS_DENIED;
        }

        break;
    }

    case RegNtPreDeleteValueKey: {
        if (IsProcessName("regedit.exe", PsGetCurrentProcess())) {
            GetRegistryObjectCompleteName(&registryPath, NULL, ((PREG_DELETE_VALUE_KEY_INFORMATION)Argument2)->Object);
            DbgPrint("[RegNtPreDeleteValueKey]KeyPath: %wZ", &registryPath);
            DbgPrint("[RegNtPreDeleteValueKey]ValName: %wZ", ((PREG_DELETE_VALUE_KEY_INFORMATION)Argument2)->ValueName);
            CallbackStatus = STATUS_ACCESS_DENIED;
        }

        break;
    }

    case RegNtPreRenameKey: {
        if (IsProcessName("regedit.exe", PsGetCurrentProcess())) {
            GetRegistryObjectCompleteName(&registryPath, NULL, ((PREG_RENAME_KEY_INFORMATION)Argument2)->Object);
            DbgPrint("[RegNtPreRenameKey]KeyPath: %wZ", &registryPath);
            DbgPrint("[RegNtPreRenameKey]NewName: %wZ", ((PREG_RENAME_KEY_INFORMATION)Argument2)->NewName);
            CallbackStatus = STATUS_ACCESS_DENIED;
        }

        break;
    }

    //『注册表编辑器』里的“重命名键值”是没有直接函数的，是先SetValueKey再DeleteValueKey
    default:
        break;
    }

    if (registryPath.Buffer != NULL)
        ExFreePoolWithTag(registryPath.Buffer, REGISTRY_POOL_TAG);

    return CallbackStatus;
}