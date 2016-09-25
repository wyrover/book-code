#include <ntddk.h>

#define dprintf             if (DBG) DbgPrint

#define DEVICE_NAME         L"\\Device\\MyDriver"
#define LINK_NAME           L"\\DosDevices\\MyDriver"
#define LINK_GLOBAL_NAME    L"\\DosDevices\\Global\\MyDriver"

#define IOCTL_ULR3IN    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x800, METHOD_BUFFERED, FILE_ANY_ACCESS) //In LONG
#define IOCTL_USR3IN    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x801, METHOD_BUFFERED, FILE_ANY_ACCESS) //In BSTR
#define IOCTL_GetKPEB   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x802, METHOD_BUFFERED, FILE_ANY_ACCESS) //Out LONG
#define IOCTL_GetBSTR   CTL_CODE(FILE_DEVICE_UNKNOWN, 0x804, METHOD_BUFFERED, FILE_ANY_ACCESS) //Out BSTR
#define IOCTL_ReInline  CTL_CODE(FILE_DEVICE_UNKNOWN, 0x803, METHOD_BUFFERED, FILE_ANY_ACCESS) //Test Call Only
#define IOCTL_Struct    CTL_CODE(FILE_DEVICE_UNKNOWN, 0x805, METHOD_BUFFERED, FILE_ANY_ACCESS) //I+O Struct

NTKERNELAPI NTSTATUS ZwQueryDirectoryFile
(
    HANDLE FileHandle,
    HANDLE Event,
    PIO_APC_ROUTINE ApcRoutine,
    PVOID ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    PVOID FileInformation,
    ULONG Length,
    FILE_INFORMATION_CLASS FileInformationClass,
    BOOLEAN ReturnSingleEntry,
    PUNICODE_STRING FileName,
    BOOLEAN RestartScan
);

#define INVALID_HANDLE_VALUE (HANDLE)-1
#define MAX_PATH2 4096
#define kmalloc(_s) ExAllocatePoolWithTag(NonPagedPool, _s, 'SYSQ')
#define kfree(_p)   ExFreePool(_p)

typedef struct _FILE_BOTH_DIR_INFORMATION {
    ULONG NextEntryOffset;
    ULONG FileIndex;
    LARGE_INTEGER CreationTime;
    LARGE_INTEGER LastAccessTime;
    LARGE_INTEGER LastWriteTime;
    LARGE_INTEGER ChangeTime;
    LARGE_INTEGER EndOfFile;
    LARGE_INTEGER AllocationSize;
    ULONG FileAttributes;
    ULONG FileNameLength;
    ULONG EaSize;
    CCHAR ShortNameLength;
    WCHAR ShortName[12];
    WCHAR FileName[1];
} FILE_BOTH_DIR_INFORMATION, *PFILE_BOTH_DIR_INFORMATION;

HANDLE MyFindFirstFile(LPSTR lpDirectory, PFILE_BOTH_DIR_INFORMATION pDir, ULONG uLength)
{
    char strFolder[MAX_PATH2] = {0};
    STRING astrFolder;
    UNICODE_STRING ustrFolder;
    OBJECT_ATTRIBUTES oa;
    IO_STATUS_BLOCK ioStatus;
    NTSTATUS ntStatus;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    memset(strFolder, 0, MAX_PATH2);
    strcpy(strFolder, "\\??\\");
    strcat(strFolder, lpDirectory);
    RtlInitString(&astrFolder, strFolder);

    if (RtlAnsiStringToUnicodeString(&ustrFolder, &astrFolder, TRUE) == 0) {
        InitializeObjectAttributes(&oa, &ustrFolder, OBJ_CASE_INSENSITIVE, NULL, NULL);
        ntStatus = IoCreateFile(
                       &hFind,
                       FILE_LIST_DIRECTORY | SYNCHRONIZE | FILE_ANY_ACCESS,
                       &oa,
                       &ioStatus,
                       NULL,
                       FILE_ATTRIBUTE_NORMAL,
                       FILE_SHARE_READ | FILE_SHARE_WRITE,
                       FILE_OPEN,   //FILE_OPEN
                       FILE_DIRECTORY_FILE | FILE_SYNCHRONOUS_IO_NONALERT | FILE_OPEN_FOR_BACKUP_INTENT,
                       NULL,
                       0,
                       CreateFileTypeNone,
                       NULL,
                       IO_NO_PARAMETER_CHECKING);
        RtlFreeUnicodeString(&ustrFolder);

        if (ntStatus == 0 && hFind != INVALID_HANDLE_VALUE) {
            ntStatus = ZwQueryDirectoryFile(
                           hFind, // File Handle
                           NULL, // Event
                           NULL, // Apc routine
                           NULL, // Apc context
                           &ioStatus, // IoStatusBlock
                           pDir, // FileInformation
                           uLength, // Length
                           FileBothDirectoryInformation, // FileInformationClass
                           TRUE, // ReturnSingleEntry
                           NULL, // FileName
                           FALSE //RestartScan
                       );

            if (ntStatus != 0) {
                ZwClose(hFind);
                hFind = INVALID_HANDLE_VALUE;
            }
        }
    }

    return hFind;
}

BOOLEAN MyFindNextFile(HANDLE hFind, PFILE_BOTH_DIR_INFORMATION pDir, ULONG uLength)
{
    IO_STATUS_BLOCK ioStatus;
    NTSTATUS ntStatus;
    ntStatus = ZwQueryDirectoryFile(
                   hFind, // File Handle
                   NULL, // Event
                   NULL, // Apc routine
                   NULL, // Apc context
                   &ioStatus, // IoStatusBlock
                   pDir, // FileInformation
                   uLength, // Length
                   FileBothDirectoryInformation, // FileInformationClass
                   FALSE, // ReturnSingleEntry
                   NULL, // FileName
                   FALSE //RestartScan
               );

    if (ntStatus == 0)
        return TRUE;
    else
        return FALSE;
}

ULONG SearchDirectory(LPSTR lpPath) //, PDIR_INFO pDirInfo
{
    ULONG muFileCount = 0;
    HANDLE hFind = INVALID_HANDLE_VALUE;
    PFILE_BOTH_DIR_INFORMATION pDir;
    char *strBuffer = NULL, *lpTmp = NULL;
    char strFileName[255 * 2];
    ULONG uLength = MAX_PATH2 * 2 + sizeof(FILE_BOTH_DIR_INFORMATION);
    strBuffer = (PCHAR)kmalloc(uLength);
    pDir = (PFILE_BOTH_DIR_INFORMATION)strBuffer;
    hFind = MyFindFirstFile(lpPath, pDir, uLength);

    if (hFind != INVALID_HANDLE_VALUE) {
        kfree(strBuffer);
        uLength = (MAX_PATH2 * 2 + sizeof(FILE_BOTH_DIR_INFORMATION)) * 0x2000;
        strBuffer = (PCHAR)kmalloc(uLength);
        pDir = (PFILE_BOTH_DIR_INFORMATION)strBuffer;

        if (MyFindNextFile(hFind, pDir, uLength)) {
            while (TRUE) {
                memset(strFileName, 0, 255 * 2);
                memcpy(strFileName, pDir->FileName, pDir->FileNameLength);

                if (strcmp(strFileName, "..") != 0 && strcmp(strFileName, ".") != 0) {
                    if (pDir->FileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
                        DbgPrint("[目录]%S\n", strFileName);
                    } else {
                        DbgPrint("[文件]%S\n", strFileName);
                    }

                    muFileCount++;
                }

                if (pDir->NextEntryOffset == 0) break;

                pDir = (PFILE_BOTH_DIR_INFORMATION)((char *)pDir + pDir->NextEntryOffset);
            }

            kfree(strBuffer);
        }

        ZwClose(hFind);
    }

    return muFileCount;
}