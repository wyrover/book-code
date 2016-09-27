#include <stdio.h>
#include <Windows.h>
#include <Psapi.h>
//#include "pe.h"

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"psapi.lib")

typedef int (__stdcall *MSGBOXA)(HWND hwnd, char *text, char *title, UINT type);
typedef bool (__stdcall *TERMINATEPROCESS)(HANDLE hProcess, UINT uExitCode);
ULONG OriMsgBoxA;
ULONG OriTerminateProcess;

int __stdcall iatProxyMessageBoxA(HWND hwnd, char *text, char *title, UINT type)
{
    MSGBOXA orifun = (MSGBOXA)OriMsgBoxA;
    printf("[iatProxyMessageBoxA - %s][%s]\n", title, text);
    return 0;
    orifun(hwnd, text, title, type);
}

int __stdcall eatProxyMessageBoxA(HWND hwnd, char *text, char *title, UINT type)
{
    MSGBOXA orifun = (MSGBOXA)OriMsgBoxA;
    printf("[eatProxyMessageBoxA - %s][%s]\n", title, text);
    return 0;
    orifun(hwnd, text, title, type);
}

bool __stdcall eatProxyTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
    TERMINATEPROCESS orifun = (TERMINATEPROCESS)OriTerminateProcess;
    printf("eatProxyTerminateProcess\n");
    return orifun(hProcess, uExitCode);
}

bool __stdcall iatProxyTerminateProcess(HANDLE hProcess, UINT uExitCode)
{
    TERMINATEPROCESS orifun = (TERMINATEPROCESS)OriTerminateProcess;
    printf("iatProxyTerminateProcess\n");
    return orifun(hProcess, uExitCode);
}

VOID EAT_HOOK_TEST(char *ModName, char *FunName, LONG ProxyFunAddr)
{
    HANDLE hMod;
    PVOID BaseAddress = NULL;
    IMAGE_DOS_HEADER * dosheader;
    IMAGE_OPTIONAL_HEADER * opthdr;
    PIMAGE_EXPORT_DIRECTORY exports;
    USHORT index = 0 ;
    ULONG addr, i;
    PUCHAR pFuncName = NULL;
    PULONG pAddressOfFunctions;
    PULONG pAddressOfNames;
    PUSHORT pAddressOfNameOrdinals;
    BaseAddress = GetModuleHandleA(ModName);
    hMod = BaseAddress;
    dosheader = (IMAGE_DOS_HEADER *)hMod;
    opthdr = (IMAGE_OPTIONAL_HEADER *)((BYTE*)hMod + dosheader->e_lfanew + 24); //24=4+sizeof(IMAGE_FILE_HEADER)
    exports = (PIMAGE_EXPORT_DIRECTORY)((BYTE*)dosheader + opthdr->DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);
    pAddressOfFunctions = (ULONG*)((BYTE*)hMod + exports->AddressOfFunctions);
    pAddressOfNames = (ULONG*)((BYTE*)hMod + exports->AddressOfNames);
    pAddressOfNameOrdinals = (USHORT*)((BYTE*)hMod + exports->AddressOfNameOrdinals);

    for (i = 0; i < exports->NumberOfNames; i++) {
        index = pAddressOfNameOrdinals[i];
        addr = pAddressOfFunctions[index];
        pFuncName = (PUCHAR)((BYTE*)hMod + pAddressOfNames[i]);
        addr = pAddressOfFunctions[index];

        if (!strcmp((const char*)pFuncName, FunName)) {
            DWORD ass;
            VirtualProtect(&pAddressOfFunctions[index], 4, PAGE_EXECUTE_READWRITE, &ass);
            pAddressOfFunctions[index] = (ULONG)((LONG)ProxyFunAddr - (LONG)hMod);
            printf("eat fix!!!\n");
        }
    }
}

BOOL IAT_HOOK_TEST(char *DllName, HMODULE hMod, ULONG g_orgProc, ULONG g_newProc)
{
    IMAGE_DOS_HEADER* pDosHeader = (IMAGE_DOS_HEADER*)hMod;
    IMAGE_OPTIONAL_HEADER* pOptHeader = (IMAGE_OPTIONAL_HEADER *)((BYTE*)hMod + pDosHeader->e_lfanew + 24);  //24=4+sizeof(IMAGE_FILE_HEADER)
    IMAGE_IMPORT_DESCRIPTOR* pImportDesc = (IMAGE_IMPORT_DESCRIPTOR*)((BYTE*)hMod + pOptHeader->DataDirectory[IMAGE_DIRECTORY_ENTRY_IMPORT].VirtualAddress);

    // 在导入表中查找user32.dll模块。因为MessageBoxA函数从user32.dll模块导出
    while (pImportDesc->FirstThunk) {
        char* pszDllName = (char*)((BYTE*)hMod + pImportDesc->Name);

        if (lstrcmpiA(pszDllName, DllName) == 0) {
            break;
        }

        pImportDesc++;
    }

    if (pImportDesc->FirstThunk) {
        // 一个IMAGE_THUNK_DATA就是一个双字，它指定了一个导入函数
        // 调入地址表其实是IMAGE_THUNK_DATA结构的数组，也就是DWORD数组
        IMAGE_THUNK_DATA* pThunk = (IMAGE_THUNK_DATA*)((BYTE*)hMod + pImportDesc->FirstThunk);

        while (pThunk->u1.Function) {
            // lpAddr指向的内存保存了函数的地址
            ULONG* lpAddr = (ULONG*) & (pThunk->u1.Function);

            if (*lpAddr == g_orgProc) {
                DWORD dwOldProtect;
                VirtualProtect(lpAddr, sizeof(ULONG), PAGE_EXECUTE_READWRITE, &dwOldProtect);
                *lpAddr = (ULONG)g_newProc;
                printf("iat fix!!!\n");
                return TRUE;
            }

            pThunk++;
        }
    }

    return FALSE;
}

//IAT HOOK只对进程一个模块有效【拦截的是link lib然后CALL的方式】
//EAT HOOK进程所有模块有效【拦截的是getprocaddr获得ptr再CALL的方式】
int main()
{
    OriMsgBoxA = (ULONG)MessageBoxA;
    IAT_HOOK_TEST("user32.dll", GetModuleHandleA(0), (ULONG)MessageBoxA, (ULONG)iatProxyMessageBoxA);
    EAT_HOOK_TEST("user32.dll", "MessageBoxA", (ULONG)eatProxyMessageBoxA);
    OriTerminateProcess = (ULONG)TerminateProcess;
    IAT_HOOK_TEST("kernel32.dll", GetModuleHandleA(0), (ULONG)TerminateProcess, (ULONG)iatProxyTerminateProcess);
    EAT_HOOK_TEST("kernel32.dll", "TerminateProcess", (ULONG)eatProxyTerminateProcess);
    printf("Press any key to test.\n");
    getchar();
    //test MessageBoxA
    MessageBoxA(0, "Direct call MessageBoxA", "test", 0);
    MSGBOXA msgboxA = (MSGBOXA)GetProcAddress(GetModuleHandleA("user32.dll"), "MessageBoxA"); //printf("msgboxA: %p\n",msgboxA);getchar();
    msgboxA(0, "Call MessageBoxA_Ptr from GetProcAddress", "test", 0);
    //test TerminateProcess[输入无效句柄测试一下即可]
    TerminateProcess((HANDLE)1234, 0);
    TERMINATEPROCESS tp = (TERMINATEPROCESS)GetProcAddress(GetModuleHandleA("kernel32.dll"), "TerminateProcess");
    tp((HANDLE)1234, 0);
    getchar();
    return 0;
}
/*
注释：在WIN32程序里，对于已经运行的程序，IAT HOOK似乎是无效的
*/