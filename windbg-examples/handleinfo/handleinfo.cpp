// handleinfo.cpp : Defines the entry point for the console application.
//

#include <windows.h>
#include <iostream>
#include <sstream>
#include <string>
#include <tchar.h>
#include <TlHelp32.h>


#define NT_SUCCESS(x) ((x) >= 0)
#define STATUS_INFO_LENGTH_MISMATCH 0xc0000004

#define SystemHandleInformation 16
#define ObjectBasicInformation 0
#define ObjectNameInformation 1
#define ObjectTypeInformation 2

typedef __success(return >= 0) LONG NTSTATUS;

typedef NTSTATUS(NTAPI *_NtQuerySystemInformation)(
    ULONG SystemInformationClass,
    PVOID SystemInformation,
    ULONG SystemInformationLength,
    PULONG ReturnLength
    );
typedef NTSTATUS(NTAPI *_NtDuplicateObject)(
    HANDLE SourceProcessHandle,
    HANDLE SourceHandle,
    HANDLE TargetProcessHandle,
    PHANDLE TargetHandle,
    ACCESS_MASK DesiredAccess,
    ULONG Attributes,
    ULONG Options
    );
typedef NTSTATUS(NTAPI *_NtQueryObject)(
    HANDLE ObjectHandle,
    ULONG ObjectInformationClass,
    PVOID ObjectInformation,
    ULONG ObjectInformationLength,
    PULONG ReturnLength
    );

typedef struct _UNICODE_STRING
{
    USHORT Length;
    USHORT MaximumLength;
    PWSTR Buffer;
} UNICODE_STRING, *PUNICODE_STRING;

typedef struct _SYSTEM_HANDLE
{
    ULONG ProcessId;
    BYTE ObjectTypeNumber;
    BYTE Flags;
    USHORT Handle;
    PVOID Object;
    ACCESS_MASK GrantedAccess;
} SYSTEM_HANDLE, *PSYSTEM_HANDLE;

typedef struct _SYSTEM_HANDLE_INFORMATION
{
    ULONG HandleCount;
    SYSTEM_HANDLE Handles[1];
} SYSTEM_HANDLE_INFORMATION, *PSYSTEM_HANDLE_INFORMATION;

typedef enum _POOL_TYPE
{
    NonPagedPool,
    PagedPool,
    NonPagedPoolMustSucceed,
    DontUseThisType,
    NonPagedPoolCacheAligned,
    PagedPoolCacheAligned,
    NonPagedPoolCacheAlignedMustS
} POOL_TYPE, *PPOOL_TYPE;

typedef struct _OBJECT_TYPE_INFORMATION
{
    UNICODE_STRING Name;
    ULONG TotalNumberOfObjects;
    ULONG TotalNumberOfHandles;
    ULONG TotalPagedPoolUsage;
    ULONG TotalNonPagedPoolUsage;
    ULONG TotalNamePoolUsage;
    ULONG TotalHandleTableUsage;
    ULONG HighWaterNumberOfObjects;
    ULONG HighWaterNumberOfHandles;
    ULONG HighWaterPagedPoolUsage;
    ULONG HighWaterNonPagedPoolUsage;
    ULONG HighWaterNamePoolUsage;
    ULONG HighWaterHandleTableUsage;
    ULONG InvalidAttributes;
    GENERIC_MAPPING GenericMapping;
    ULONG ValidAccess;
    BOOLEAN SecurityRequired;
    BOOLEAN MaintainHandleCount;
    USHORT MaintainTypeList;
    POOL_TYPE PoolType;
    ULONG PagedPoolUsage;
    ULONG NonPagedPoolUsage;
} OBJECT_TYPE_INFORMATION, *POBJECT_TYPE_INFORMATION;

//---------------------------------------------------------------------------------------------------------------------------
static bool GetProcName(DWORD aPid, std::wstring* out)
{
    PROCESSENTRY32 processInfo;
    processInfo.dwSize = sizeof(processInfo);
    HANDLE processesSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
    if (processesSnapshot == INVALID_HANDLE_VALUE)
    {
        return false;
    }

    std::wstring procName;
    for (BOOL bok = Process32First(processesSnapshot, &processInfo); bok; bok = Process32Next(processesSnapshot, &processInfo))
    {
        if (aPid == processInfo.th32ProcessID)
        {
            CloseHandle(processesSnapshot);
            procName = std::wstring(processInfo.szExeFile);
            break;
        }
    }
    CloseHandle(processesSnapshot);

    *out = procName;
    return !procName.empty();
}

//---------------------------------------------------------------------------------------------------------------------------
static bool wchar2long_ss(const WCHAR* str, DWORD* out)
{
    std::wstringstream buffer;
    buffer << str;
    buffer >> (*out);
    return !buffer.fail();
}


//---------------------------------------------------------------------------------------------------------------------------
int wmain(int argc, WCHAR* argv[])
{
    if (argc != 2)
    {
        return 1;
    }

    DWORD pid = 0;
    if (!wchar2long_ss(argv[1], &pid))
    {
        _tprintf(L"error parsing- %ls to int\n", argv[1]);
        return 1;
    }

    std::wstring procName;
    GetProcName(pid, &procName);


    HMODULE hMod = GetModuleHandle(L"ntdll.dll");
    HANDLE currProc = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (currProc != NULL && currProc != INVALID_HANDLE_VALUE)
    {

        _NtQuerySystemInformation NtQuerySystemInformation =
            (_NtQuerySystemInformation)GetProcAddress(hMod, "NtQuerySystemInformation");
        _NtDuplicateObject NtDuplicateObject =
            (_NtDuplicateObject)GetProcAddress(hMod, "NtDuplicateObject");
        _NtQueryObject NtQueryObject =
            (_NtQueryObject)GetProcAddress(hMod, "NtQueryObject");


        NTSTATUS status;
        ULONG handleInfoSize = 0x10000;
        HANDLE processHandle = currProc;
        PSYSTEM_HANDLE_INFORMATION handleInfo = (PSYSTEM_HANDLE_INFORMATION)malloc(handleInfoSize);

        while ((status = NtQuerySystemInformation(SystemHandleInformation, handleInfo, handleInfoSize, NULL)) == STATUS_INFO_LENGTH_MISMATCH)
            handleInfo = (PSYSTEM_HANDLE_INFORMATION)realloc(handleInfo, handleInfoSize *= 2);


        for (size_t i = 0; i < handleInfo->HandleCount; i++)
        {
            SYSTEM_HANDLE handle = handleInfo->Handles[i];
            HANDLE dupHandle = NULL;
            ULONG returnLength;

            if (handle.ProcessId != pid)
                continue;


            NT_SUCCESS(NtDuplicateObject(processHandle, (HANDLE)handle.Handle, GetCurrentProcess(), &dupHandle, 0, 0, 0));

            POBJECT_TYPE_INFORMATION objectTypeInfo = (POBJECT_TYPE_INFORMATION)malloc(0x1000);
            NT_SUCCESS(NtQueryObject(dupHandle, ObjectTypeInformation, objectTypeInfo, 0x1000, NULL));

            if (handle.GrantedAccess == 0x0012019f)
            {
                std::free(objectTypeInfo);
                CloseHandle(dupHandle);
                continue;
            }

            PVOID objectNameInfo = malloc(0x1000);
            if (!NT_SUCCESS(NtQueryObject(dupHandle, ObjectNameInformation, objectNameInfo, 0x1000, &returnLength)))
            {
                objectNameInfo = realloc(objectNameInfo, returnLength);
                if (!NT_SUCCESS(NtQueryObject(
                    dupHandle,
                    ObjectNameInformation,
                    objectNameInfo,
                    returnLength,
                    NULL
                    )))
                {
                    std::free(objectTypeInfo);
                    std::free(objectNameInfo);
                    CloseHandle(dupHandle);
                    continue;
                }
            }

            UNICODE_STRING objectName = *(PUNICODE_STRING)objectNameInfo;
            std::wstring ObjectBuffer = objectTypeInfo->Name.Buffer;
            std::wstring ObjectName = (objectName.Buffer != NULL ? objectName.Buffer : L"");

            std::wcout << L"Type: " << ObjectBuffer << L" , Name: "<< ObjectName << std::endl;

            std::free(objectTypeInfo);
            std::free(objectNameInfo);
            CloseHandle(dupHandle);
        }

        CloseHandle(currProc);
    }


    return 0;
}

