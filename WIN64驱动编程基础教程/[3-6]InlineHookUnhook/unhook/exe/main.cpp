#include <stdio.h>
#include <Windows.h>
#include "DrvCtrl.h"

#ifndef NT_SUCCESS
    #define NT_SUCCESS(Status) ((NTSTATUS)(Status) >= 0)
#endif
#ifndef STATUS_SUCCESS
    #define STATUS_SUCCESS       ((NTSTATUS)0x00000000L)
#endif
#ifndef STATUS_UNSUCCESSFUL
    #define STATUS_UNSUCCESSFUL ((NTSTATUS)0xC0000001L)
#endif

HANDLE hDriver = 0;

ULONG64 GetKernelBase64(char *NtosName)
{
    typedef long (__stdcall * ZWQUERYSYSTEMINFORMATION)
    (
        IN ULONG SystemInformationClass,
        IN PVOID SystemInformation,
        IN ULONG SystemInformationLength,
        IN PULONG ReturnLength OPTIONAL
    );
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
        ULONG Count;
        SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
    } SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;
#define SystemModuleInformation 11
#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
    ZWQUERYSYSTEMINFORMATION ZwQuerySystemInformation;
    PSYSTEM_MODULE_INFORMATION pSystemModuleInformation;
    ULONG NeedSize, BufferSize = 0x5000;
    PVOID pBuffer = NULL;
    NTSTATUS Result;
    ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwQuerySystemInformation");

    do {
        pBuffer = malloc(BufferSize);

        if (pBuffer == NULL) return 0;

        Result = ZwQuerySystemInformation(SystemModuleInformation, pBuffer, BufferSize, &NeedSize);

        if (Result == STATUS_INFO_LENGTH_MISMATCH) {
            free(pBuffer);
            BufferSize *= 2;
        } else if (!NT_SUCCESS(Result)) {
            free(pBuffer);
            return 0;
        }
    } while (Result == STATUS_INFO_LENGTH_MISMATCH);

    pSystemModuleInformation = (PSYSTEM_MODULE_INFORMATION)pBuffer;
    ULONG64 ret = (ULONG64)(pSystemModuleInformation->Module[0].Base);

    if (NtosName != NULL)
        strcpy(NtosName, pSystemModuleInformation->Module[0].ImageName + pSystemModuleInformation->Module[0].ModuleNameOffset);

    free(pBuffer);
    return ret;
}

HMODULE hKernel = 0;
ULONG64 KernelBase = 0;
CHAR    NtosFullName[260] = {0};

int Reloc(ULONG64 HandleInFile, ULONG64 BaseInKernel)
{
    PIMAGE_DOS_HEADER       pDosHeader;
    PIMAGE_NT_HEADERS64     pNtHeader;
    PIMAGE_BASE_RELOCATION  pRelocTable;
    ULONG i, dwOldProtect;
    pDosHeader = (PIMAGE_DOS_HEADER)HandleInFile;

    if (pDosHeader->e_magic != IMAGE_DOS_SIGNATURE)
        return 0;

    pNtHeader = (PIMAGE_NT_HEADERS64)((ULONG64)HandleInFile + pDosHeader->e_lfanew);  //厉行检查

    if (pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].Size) { //是否存在重定位表
        pRelocTable = (PIMAGE_BASE_RELOCATION)((ULONG64)HandleInFile + pNtHeader->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress);

        do {
            ULONG   numofReloc = (pRelocTable->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION)) / 2;
            SHORT   minioffset = 0;
            PUSHORT pRelocData = (PUSHORT)((ULONG64)pRelocTable + sizeof(IMAGE_BASE_RELOCATION));

            for (i = 0; i < numofReloc; i++) { //循环，或直接判断*pData是否为0也可以作为结束标记
                PULONG64 RelocAddress;//需要重定位的地址

                //重定位的高4位是重定位类型，
                if (((*pRelocData) >> 12) == IMAGE_REL_BASED_DIR64) { //判断重定位类型
                    //计算需要进行重定位的地址
                    //重定位数据的低12位再加上本重定位块头的RVA即真正需要重定位的数据的RVA
                    minioffset = (*pRelocData) & 0xFFF; //小偏移
                    //模块基址+重定位基址+每个数据表示的小偏移量
                    RelocAddress = (PULONG64)(HandleInFile + pRelocTable->VirtualAddress + minioffset);
                    //直接在RING3修改:原始数据+基址-IMAGE_OPTINAL_HEADER中的基址
                    VirtualProtect((PVOID)RelocAddress, 4, PAGE_EXECUTE_READWRITE, &dwOldProtect);
                    //因为是R3直接LOAD的 所以要修改一下内存权限
                    *RelocAddress = *RelocAddress + BaseInKernel - pNtHeader->OptionalHeader.ImageBase;
                    VirtualProtect((PVOID)RelocAddress, 4, dwOldProtect, NULL);
                }

                //下一个重定位数据
                pRelocData++;
            }

            //下一个重定位块
            pRelocTable = (PIMAGE_BASE_RELOCATION)((ULONG64)pRelocTable + pRelocTable->SizeOfBlock);
        } while (pRelocTable->VirtualAddress);

        return TRUE;
    }

    return FALSE;
}

int InitGetOriCode(int bClear)
{
    if (bClear == 0) {
        KernelBase = GetKernelBase64(NtosFullName); //printf("KB: %llx\nKN: %s\n",KernelBase,NtosFullName);

        if (!KernelBase) return 0;

        hKernel = LoadLibraryExA(NtosFullName, 0, DONT_RESOLVE_DLL_REFERENCES); //printf("KH: %llx\n",hKernel);

        if (!hKernel) return 0;

        if (!Reloc((ULONG64)hKernel, KernelBase)) return 0;

        return 1;
    } else {
        FreeLibrary(hKernel);
        return 1;
    }
}

void GetOriCode(ULONG64 Address, PUCHAR ba, SIZE_T Length)
{
    ULONG64 OffsetAddress = Address - KernelBase + (ULONG64)hKernel;
    memcpy(ba, (PVOID)OffsetAddress, Length);
}

ULONG64 GetSystemRoutineAddress(char *FuncName)
{
    return KernelBase + (ULONG64)GetProcAddress(hKernel, FuncName) - (ULONG64)hKernel;
}

void GetCurCode(ULONG64 Address, PUCHAR ba, SIZE_T Length)
{
    ULONG64 dat[2] = {0};
    dat[0] = Address;
    dat[1] = Length;
    IoControl(hDriver , CTL_CODE_GEN(0x800), dat, 16, ba, Length);
}

void ClearInlineHook(ULONG64 Address, PUCHAR ba, SIZE_T Length)
{
    typedef struct _KF_DATA {
        PVOID Address;
        ULONG64 Length;
        UCHAR data[256];
    } KF_DATA, *PKF_DATA;
    KF_DATA dat = {0};
    dat.Address = (PVOID)Address;
    dat.Length = Length;
    memcpy(dat.data, ba, Length);
    IoControl(hDriver , CTL_CODE_GEN(0x801), &dat, sizeof(KF_DATA), 0, 0);
}

void PrintBytes(char *DescriptionString, PUCHAR ba, UINT Length)
{
    printf("%s", DescriptionString);

    for (UINT i = 0; i < Length; i++) {
        printf("%02x ", ba[i]);
    }

    printf("\n");
}

int main()
{
#define BYTE_ARRAY_LENGTH 16
    UCHAR   ori_ba[BYTE_ARRAY_LENGTH];
    UCHAR   cur_ba[BYTE_ARRAY_LENGTH];
    ULONG64 adr = 0;
    CHAR    FuncName[64] = {0};
    hDriver = openDriver();

    if (!InitGetOriCode(0) || hDriver == 0) {
        printf("Init error!");
        return 0;
    }

gc:
    memset(ori_ba, 0, BYTE_ARRAY_LENGTH);
    printf("Input function name: ");
    gets(FuncName);

    if (!stricmp(FuncName, "exit"))
        goto et;

    adr = GetSystemRoutineAddress(FuncName);
    printf("Address:  0x%llx\n", adr);

    if (adr == 0 || adr < KernelBase) {
        printf("Invalid address!\n\n");
        goto gc;
    }

    GetOriCode(adr, ori_ba, BYTE_ARRAY_LENGTH);
    PrintBytes("Original: ", ori_ba, BYTE_ARRAY_LENGTH);
    GetCurCode(adr, cur_ba, BYTE_ARRAY_LENGTH);
    PrintBytes("Current:  ", cur_ba, BYTE_ARRAY_LENGTH);

    if (memcmp(ori_ba, cur_ba, BYTE_ARRAY_LENGTH)) { //不相同
        printf("Press [ENTER] to clear inline hook!");
        getchar();
        ClearInlineHook(adr, ori_ba, BYTE_ARRAY_LENGTH);
    }

    getchar();
    goto gc;
et:
    InitGetOriCode(1);
    uninstallDriver();
}