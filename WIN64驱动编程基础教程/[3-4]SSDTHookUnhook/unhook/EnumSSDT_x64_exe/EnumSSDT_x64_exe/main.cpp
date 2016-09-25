#include <stdio.h>
#include <direct.h>
#include <Windows.h>
//#include "ntimage.h"
#include "DrvCtrl.h"
#include "GetNtos.h"

HANDLE hSSDTDrv;
ULONGLONG KiServiceTable, NtosBase, NtosInProcess = 0, NtosImageBase = 0;
CHAR NtosName[260] = {0};

typedef struct _UNHOOK_SSDT64 {
    ULONGLONG Address;
    ULONGLONG id;
} UNHOOK_SSDT64, *PUNHOOK_SSDT64;

DWORD GetSSDTFunctionIndex(char *FunctionName)
{
    return *(DWORD*)((PUCHAR)GetProcAddress(LoadLibraryW(L"ntdll.dll"), FunctionName) + 4); //极限精简
    /*
    PUCHAR fptr=(PUCHAR)GetProcAddress(LoadLibraryW(L"ntdll.dll"),FunctionName);
    DWORD index=*(DWORD*)(fptr+4);
    return index;
    */
    /*
    所有的函数都像下面一样：
    00000000`775a0110 4c8bd1          mov     r10,rcx
    00000000`775a0113 b8??000000      mov     eax,??h
    00000000`775a0118 0f05            syscall
    00000000`775a011a c3              ret
    */
}

DWORD FileLen(char *filename)
{
    WIN32_FIND_DATAA fileInfo = {0};
    DWORD fileSize = 0;
    HANDLE hFind;
    hFind = FindFirstFileA(filename , &fileInfo);

    if (hFind != INVALID_HANDLE_VALUE) {
        fileSize = fileInfo.nFileSizeLow;
        FindClose(hFind);
    }

    return fileSize;
}

CHAR *LoadDllContext(char *filename)
{
    DWORD dwReadWrite, LenOfFile = FileLen(filename);
    HANDLE hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

    if (hFile != INVALID_HANDLE_VALUE) {
        PCHAR buffer = (PCHAR)malloc(LenOfFile);
        SetFilePointer(hFile, 0, 0, FILE_BEGIN);
        ReadFile(hFile, buffer, LenOfFile, &dwReadWrite, 0);
        CloseHandle(hFile);
        return buffer;
    }

    return NULL;
}

char *cs(char *str1, char *str2) //connect string
{
    long newstrlen = strlen(str1) + strlen(str2) + 1;
    char *newstr = (char*)malloc(newstrlen);
    memcpy(newstr, str1, strlen(str1));
    memcpy(newstr + strlen(str1), str2, strlen(str2) + 1);
    return newstr;
}

VOID GetKiServiceTable()
{
    IoControl(hSSDTDrv , CTL_CODE_GEN(0x801), NULL, 0, &KiServiceTable, 8);
    printf("KiServiceTable: %llx\n", KiServiceTable);
}

VOID GetNtosImageBase()
{
    PIMAGE_NT_HEADERS64 pinths64;
    PIMAGE_DOS_HEADER pdih;
    char *NtosFileData = NULL;
    NtosFileData = LoadDllContext(NtosName);
    pdih = (PIMAGE_DOS_HEADER)NtosFileData;
    pinths64 = (PIMAGE_NT_HEADERS64)(NtosFileData + pdih->e_lfanew);
    NtosImageBase = pinths64->OptionalHeader.ImageBase;
    printf("ImageBase: %llx\n", NtosImageBase);
}

VOID GetNtosBase()
{
    char FileName[260] = {0}, *FullName;
    NtosBase = GetNtosBaseAndPath(FileName);
    FullName = cs("C:\\Windows\\system32\\", FileName);
    strcpy(NtosName, FullName);
    printf("NTOSKRNL base: %llx\n", NtosBase);
    printf("NTOSKRNL name: %s\n", NtosName);
}

ULONGLONG GetFunctionOriginalAddress(DWORD index)
{
    if (NtosInProcess == 0)
        NtosInProcess = (ULONGLONG)LoadLibraryExA(NtosName, 0, DONT_RESOLVE_DLL_REFERENCES); //"C:\\Windows\\system32\\ntkrnlmp.exe"

    ULONGLONG RVA = KiServiceTable - NtosBase;
    ULONGLONG temp = *(PULONGLONG)(NtosInProcess + RVA + 8 * (ULONGLONG)index);
    ULONGLONG RVA_index = temp - NtosImageBase; //IMAGE_OPTIONAL_HEADER64.ImageBase=0x140000000（这个值基本是固定的）
    return RVA_index + NtosBase;
}

VOID PrintAllSSDTFunction()
{
    DWORD i = 0, fl = 0, fs_pos = 0, fe_pos = 0, PauseCnt = 0, ssdt_fun_cnt = 0;
    ULONG64 ssdt_func_addr = 0, ssdt_func_ori_addr = 0;
    CopyFileW(L"c:\\windows\\system32\\ntdll.dll", L"c:\\ntdll.txt", 0);
    fl = FileLen("c:\\ntdll.txt");
    char func_start[] = "ZwAcceptConnectPort", func_end[] = "ZwYieldExecution"; //每个函数名之间隔着\0
    char *funs = (char *)malloc(strlen(func_start));
    memcpy(funs, func_start, strlen(func_start));
    char *fune = (char *)malloc(strlen(func_end));
    memcpy(fune, func_end, strlen(func_end));
    char *ntdlltxt = LoadDllContext("c:\\ntdll.txt");

    for (i = 0; i < fl; i++) {
        if (memcmp(ntdlltxt + i, funs, strlen(func_start)) == 0)
            fs_pos = i;

        if (memcmp(ntdlltxt + i, fune, strlen(func_end)) == 0) {
            fe_pos = i;
            break;
        }
    }

    ntdlltxt = ntdlltxt + fs_pos;
    //打印头
    printf("ID\t当前地址\t  原始地址\t    函数名\n");

    //这里不能显示到最后一个函数
    while (strcmp(ntdlltxt, func_end) != 0) {
        DWORD fn_index = GetSSDTFunctionIndex(ntdlltxt);

        if (fn_index < 1000) {
            ssdt_func_ori_addr = GetFunctionOriginalAddress(fn_index);
            IoControl(hSSDTDrv , CTL_CODE_GEN(0x802), &fn_index, 4, &ssdt_func_addr, 8);
            ntdlltxt[0] = 'N';
            ntdlltxt[1] = 't';              //寻找的是Zw***，但是应该显示Nt***

            if (ssdt_func_ori_addr != ssdt_func_addr)
                printf("0x%-0.3X!\t%llx  %llx  %s\n", fn_index, ssdt_func_addr, ssdt_func_ori_addr, ntdlltxt);
            else
                printf("0x%-0.3X\t%llx  %llx  %s\n", fn_index, ssdt_func_addr, ssdt_func_ori_addr, ntdlltxt);
        } else {
            fn_index = 0x57; //不同的系统，不同的编号，由于目前只有WIN7 X64，所以这里直接硬编码了
            ssdt_func_ori_addr = GetFunctionOriginalAddress(fn_index);
            IoControl(hSSDTDrv , CTL_CODE_GEN(0x802), &fn_index, 4, &ssdt_func_addr, 8);
            ntdlltxt[0] = 'N';
            ntdlltxt[1] = 't';

            if (ssdt_func_ori_addr != ssdt_func_addr)
                printf("0x%-0.3X!\t%llx  %llx  %s\n", fn_index, ssdt_func_addr, ssdt_func_ori_addr, ntdlltxt);
            else
                printf("0x%-0.3X\t%llx  %llx  %s\n", fn_index, ssdt_func_addr, ssdt_func_ori_addr, ntdlltxt);

            /*
            0:000> u ZwQuerySystemTime
            ntdll!ZwQuerySystemTime:
            00000000`77b20450 e91b62fdff      jmp     ntdll!RtlQuerySystemTime (00000000`77af6670)
            00000000`77b20455 6666660f1f840000000000 nop word ptr [rax+rax]
            */
            /*
            nt!ZwQuerySystemTime:
            fffff800`01673fa0 488bc4          mov     rax,rsp
            fffff800`01673fa3 fa              cli
            fffff800`01673fa4 4883ec10        sub     rsp,10h
            fffff800`01673fa8 50              push    rax
            fffff800`01673fa9 9c              pushfq
            fffff800`01673faa 6a10            push    10h
            fffff800`01673fac 488d053d270000  lea     rax,[nt!KiServiceLinkage (fffff800`016766f0)]
            fffff800`01673fb3 50              push    rax
            fffff800`01673fb4 b857000000      mov     eax,57h
            fffff800`01673fb9 e9825e0000      jmp     nt!KiServiceInternal (fffff800`01679e40)
            fffff800`01673fbe 6690            xchg    ax,ax
            */
        }

        ntdlltxt = ntdlltxt + strlen(ntdlltxt) + 1;
        PauseCnt++;
        ssdt_fun_cnt++;

        if (PauseCnt == 101) {
            printf("\nPress [ENTER] to continue...\n");
            getchar();
            PauseCnt = 0;
        }
    }

    //显示完最后一个函数
    DWORD fn_index = GetSSDTFunctionIndex(ntdlltxt);
    ssdt_func_ori_addr = GetFunctionOriginalAddress(fn_index);
    IoControl(hSSDTDrv , CTL_CODE_GEN(0x802), &fn_index, 4, &ssdt_func_addr, 8);
    ntdlltxt[0] = 'N';
    ntdlltxt[1] = 't';
    printf("0x%-0.3X\t%llx  %llx  %s\n", fn_index, ssdt_func_addr, ssdt_func_ori_addr, ntdlltxt);
    ssdt_fun_cnt++;
    //显示ssdt表上的函数数
    printf("\nTotal of SSDT function: %ld\n", ssdt_fun_cnt);
    DeleteFileA("c:\\ntdll.txt");
}

VOID UnhookSSDT()
{
    ULONG id;
    ULONG64 ssdt_func_ori_addr = 0;
    UNHOOK_SSDT64 data = {0};
    printf("Input SSDT function index which you want to unhook (like 0x29): ");
    scanf("0x%x", &id);
    ssdt_func_ori_addr = GetFunctionOriginalAddress(id);
    data.Address = ssdt_func_ori_addr;
    data.id = (ULONGLONG)id;
    IoControl(hSSDTDrv , CTL_CODE_GEN(0x800), &data, sizeof(UNHOOK_SSDT64), 0, 0);
}

int main()
{
    ZwQuerySystemInformation = (ZWQUERYSYSTEMINFORMATION)GetProcAddress(GetModuleHandleA("ntdll.dll"), "ZwQuerySystemInformation");
    hSSDTDrv = openDriver();
    GetNtosBase();
    GetNtosImageBase();
    GetKiServiceTable();
    printf("\nPress [ENTER] to continue...\n");
    getchar();
    PrintAllSSDTFunction();
    getchar();
    UnhookSSDT();
    uninstallDriver();
    return 0;
}