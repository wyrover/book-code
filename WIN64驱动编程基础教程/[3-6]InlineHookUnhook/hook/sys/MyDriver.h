#include <ntddk.h>

#define dprintf             DbgPrint
#define nprintf             DbgPrint

#define DEVICE_NAME         L"\\Device\\hook_ps"
#define LINK_NAME           L"\\DosDevices\\hook_ps"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\hook_ps"

#define IOCTL_TEST          CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS)

HANDLE FileHandle;

VOID OccupyTaskhost()
{
    NTSTATUS ntStatus;
    OBJECT_ATTRIBUTES ObjectAttributes;
    UNICODE_STRING UniFileName;
    IO_STATUS_BLOCK IoStatusBlock;
    PCWSTR FileName = L"\\??\\C:\\WINDOWS\\system32\\taskhost.exe";
    RtlInitUnicodeString(&UniFileName , FileName);
    InitializeObjectAttributes(&ObjectAttributes, &UniFileName, OBJ_CASE_INSENSITIVE | OBJ_KERNEL_HANDLE, NULL, NULL);
    ntStatus = ZwCreateFile(&FileHandle, GENERIC_READ, &ObjectAttributes, &IoStatusBlock, 0, FILE_ATTRIBUTE_NORMAL, 0, FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE, NULL, 0);

    if (!NT_SUCCESS(ntStatus))
        DbgPrint("[OccupyFile] = %d", ntStatus);
    else
        DbgPrint("[OccupyFile] Success.");
}