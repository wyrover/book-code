#include <stdio.h>
#include <Windows.h>

typedef NTSTATUS(*ZWQUERYSYSTEMINFORMATION)
(
    IN ULONG    SystemInformationClass,
    OUT PVOID   SystemInformation,
    IN ULONG    Length,
    OUT PULONG  ReturnLength
);

typedef unsigned long DWORD;

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY {
    ULONG Unknow1;
    ULONG Unknow2;
    ULONG Unknow3;
    ULONG Unknow4;
    PVOID Base;
    ULONG Size;
    ULONG Flags;
    USHORT Index;
    USHORT NameLength;
    USHORT LoadCount;
    USHORT ModuleNameOffset;
    char ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;

typedef struct _SYSTEM_MODULE_INFORMATION {
    ULONG Count;//内核中以加载的模块的个数
    SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;

BOOLEAN EnumKM(char *HighlightDrvName)
{
    ULONG NeedSize, i, ModuleCount, HLed = 0, BufferSize = 0x5000;
    PVOID pBuffer = NULL;
    PCHAR pDrvName = NULL;
    NTSTATUS Result;
    PSYSTEM_MODULE_INFORMATION pSystemModuleInformation;

    do {
        //分配内存
        pBuffer = malloc(BufferSize);

        if (pBuffer == NULL)
            return 0;

        //查询模块信息
        Result = ZwQuerySystemInformation(11, pBuffer, BufferSize, &NeedSize);

        if (Result == 0xC0000004L) {
            free(pBuffer);
            BufferSize *= 2;
        } else if (Result < 0) {
            //查询失败则退出
            free(pBuffer);
            return 0;
        }
    } while (Result == 0xC0000004L);

    pSystemModuleInformation = (PSYSTEM_MODULE_INFORMATION)pBuffer;
    //获得模块的总数量
    ModuleCount = pSystemModuleInformation->Count;

    //遍历所有的模块
    for (i = 0; i < ModuleCount; i++) {
        if ((ULONG64)(pSystemModuleInformation->Module[i].Base) > (ULONG64)0x8000000000000000) {
            pDrvName = pSystemModuleInformation->Module[i].ImageName + pSystemModuleInformation->Module[i].ModuleNameOffset;
            printf("0x%llx\t%s", (ULONG64)pSystemModuleInformation->Module[i].Base, pDrvName);

            if (_stricmp(pDrvName, HighlightDrvName) == 0) {
                printf("\t\t<--------------------");
                HLed = 1;
            }

            printf("\n");
        }
    }

    if (HLed == 0)
        printf("\n[%s] NOT FOUND!", HighlightDrvName);

    free(pBuffer);
    return 1;
}

int main()
{
    ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(LoadLibraryW(L"ntdll.dll"), "ZwQuerySystemInformation");
    EnumKM("win32k.sys");
    getchar();
    return 0;
}