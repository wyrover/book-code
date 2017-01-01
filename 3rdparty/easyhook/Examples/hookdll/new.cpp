

// dllmain.cpp : 定义 DLL 应用程序的入口点。
#include "stdafx.h"
#include "HookApi.h"
#include "easyhook.h"
#include "ntstatus.h"

ptrCreateFileW realCreateFileW = NULL;
ptrCreateFileA realCreateFileA = NULL;

//ptrMessageBeep realMessageBeep = NULL;
ptrPlaySoundW   realPlaySoundW = NULL;

HMODULE                 hKernel32 = NULL;
TRACED_HOOK_HANDLE      hHookCreateFileW = new HOOK_TRACE_INFO();
TRACED_HOOK_HANDLE      hHookCreateFileA = new HOOK_TRACE_INFO();
TRACED_HOOK_HANDLE      hHookPlaySoundW  = new HOOK_TRACE_INFO();

NTSTATUS                statue;
ULONG                   HookCreateFileW_ACLEntries[1] = {0};
ULONG                   HookCreateFileA_ACLEntries[1] = {0};
ULONG                   HookPlaySoundW_ACLEntries[1]   = {0};

BOOL MyPlaySoundW(LPCWSTR pszSound, HMODULE hwnd, DWORD fdwSound);

int PrepareRealApiEntry()
{
    OutputDebugString(L"PrepareRealApiEntry()\n");
    // 获取真实函数地址
    HMODULE hKernel32 = LoadLibrary(L"Kernel32.dll");

    if (hKernel32 == NULL) {
        OutputDebugString(L"LoadLibrary(L\"Kernel32.dll\") Error\n");
        return -6002;
    }

    OutputDebugString(L"LoadLibrary(L\"Kernel32.dll\") OK\n");
    realCreateFileW = (ptrCreateFileW)GetProcAddress(hKernel32, "CreateFileW");

    if (realCreateFileW == NULL) {
        OutputDebugString(L"(ptrCreateFileW)GetProcAddress(hKernel32, \"CreateFileW\") Error\n");
        return -6007;
    }

    OutputDebugString(L"(ptrCreateFileW)GetProcAddress(hKernel32, \"CreateFileW\") OK\n");
    realCreateFileA = (ptrCreateFileA)GetProcAddress(hKernel32, "CreateFileA");

    if (realCreateFileA == NULL) {
        OutputDebugString(L"(ptrCreateFileA)GetProcAddress(hKernel32, \"CreateFileA\") Error\n");
        return -6007;
    }

    OutputDebugString(L"(ptrCreateFileA)GetProcAddress(hKernel32, \"CreateFileA\") OK\n");
    //---------------------------
    HMODULE hWinmm = LoadLibrary(L"winmm.dll");

    if (hWinmm == NULL) {
        OutputDebugString(L"LoadLibrary(L\"winmm.dll\") Error\n");
        return -6002;
    }

    OutputDebugString(L"LoadLibrary(L\"winmm.dll\") OK\n");
    realPlaySoundW = (ptrPlaySoundW)GetProcAddress(hWinmm, "PlaySoundW");

    if (realPlaySoundW == NULL) {
        OutputDebugString(L"(ptrCreateFileW)GetProcAddress(hWinmm, \"PlaySoundW\") Error\n");
        return -6007;
    }

    OutputDebugString(L"(ptrCreateFileW)GetProcAddress(hWinmm, \"PlaySoundW\") OK\n");
    //---------------------------
    return 0;
}

void DoHook()
{
    OutputDebugString(L"DoHook()\n");
    //statue = LhInstallHook(realCreateFileW,
    //  MyCreateFileW,
    //  /*(PVOID)0x12345678*/NULL,
    //  hHookCreateFileW);
    //if(!SUCCEEDED(statue))
    //{
    //  switch (statue)
    //  {
    //  case STATUS_NO_MEMORY:
    //      OutputDebugString(L"STATUS_NO_MEMORY\n");
    //      break;
    //  case STATUS_NOT_SUPPORTED:
    //      OutputDebugString(L"STATUS_NOT_SUPPORTED\n");
    //      break;
    //  case STATUS_INSUFFICIENT_RESOURCES:
    //      OutputDebugString(L"STATUS_INSUFFICIENT_RESOURCES\n");
    //      break;
    //  default:
    //      WCHAR dbgstr[512] = {0};
    //      wsprintf(dbgstr, L"%d\n", statue);
    //      OutputDebugString(dbgstr);
    //  }
    //  OutputDebugString(L"LhInstallHook(GetProcAddress(hKernel32, \"CreateFileW\"),MyCreateFileW,(PVOID)0x12345678,hHookCreateFileW); Error\n");
    //  return;
    //}
    //OutputDebugString(L"Hook CreateFileW OK\n");
    //statue = LhInstallHook(realCreateFileA,
    //                      MyCreateFileA,
    //                      /*(PVOID)0x12345678*/NULL,
    //                      hHookCreateFileA);
    //if(!SUCCEEDED(statue))
    //{
    //  switch (statue)
    //  {
    //  case STATUS_NO_MEMORY:
    //      OutputDebugString(L"STATUS_NO_MEMORY\n");
    //      break;
    //  case STATUS_NOT_SUPPORTED:
    //      OutputDebugString(L"STATUS_NOT_SUPPORTED\n");
    //      break;
    //  case STATUS_INSUFFICIENT_RESOURCES:
    //      OutputDebugString(L"STATUS_INSUFFICIENT_RESOURCES\n");
    //      break;
    //  default:
    //      WCHAR dbgstr[512] = {0};
    //      wsprintf(dbgstr, L"%d\n", statue);
    //      OutputDebugString(dbgstr);
    //  }
    //  OutputDebugString(L"LhInstallHook(GetProcAddress(hKernel32, \"CreateFileA\"),MyCreateFileA,(PVOID)0x12345678,hHookCreateFileA); Error\n");
    //  return;
    //}
    //OutputDebugString(L"Hook CreateFileA OK\n");
    statue = LhInstallHook(realPlaySoundW,
                           MyPlaySoundW,
                           /*(PVOID)0x12345678*/NULL,
                           hHookPlaySoundW);

    if (!SUCCEEDED(statue)) {
        switch (statue) {
        case STATUS_NO_MEMORY:
            OutputDebugString(L"STATUS_NO_MEMORY\n");
            break;

        case STATUS_NOT_SUPPORTED:
            OutputDebugString(L"STATUS_NOT_SUPPORTED\n");
            break;

        case STATUS_INSUFFICIENT_RESOURCES:
            OutputDebugString(L"STATUS_INSUFFICIENT_RESOURCES\n");
            break;

        default:
            WCHAR dbgstr[512] = {0};
            wsprintf(dbgstr, L"%d\n", statue);
            OutputDebugString(dbgstr);
        }

        OutputDebugString(L"LhInstallHook(GetProcAddress(hWinmm, \"PlaySoundW\"),MyPlaySoundW,NULL,hHookPlaySoundW); Error\n");
        return;
    }

    OutputDebugString(L"Hook PlaySoundW() OK\n");
    // 一定要调用这个函数，否则注入的钩子无法正常运行。
    //LhSetExclusiveACL(HookCreateFileA_ACLEntries, 1, hHookCreateFileA);
    //LhSetExclusiveACL(HookCreateFileW_ACLEntries, 1, hHookCreateFileW);
    LhSetExclusiveACL(HookPlaySoundW_ACLEntries, 1, hHookPlaySoundW);
}

void DoneHook()
{
    OutputDebugString(L"DoneHook()\n");
    // this will also invalidate "hHook", because it is a traced handle...
    LhUninstallAllHooks();
    // this will do nothing because the hook is already removed...
    LhUninstallHook(hHookCreateFileA);
    LhUninstallHook(hHookCreateFileW);
    // now we can safely release the traced handle
    delete hHookCreateFileA;
    hHookCreateFileA = NULL;
    delete hHookCreateFileW;
    hHookCreateFileW = NULL;
    // even if the hook is removed, we need to wait for memory release
    LhWaitForPendingRemovals();
}

//---------------------------------

HANDLE WINAPI MyCreateFileW(
    __in     LPCWSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
)
{
    HANDLE hHandle = NULL;

    // 执行钩子
    if (realCreateFileW == NULL) {
        OutputDebugString(L"realCreateFileW is NULL\n");
        return INVALID_HANDLE_VALUE;
    } else {
        OutputDebugString(L"realCreateFileW is not NULL\n");
        hHandle = (realCreateFileW)(lpFileName, dwDesiredAccess, dwShareMode,
                                    lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        OutputDebugString(L"MyCreateFileW : ");
        OutputDebugString(lpFileName);
        OutputDebugString(L"\n");
        MessageBox(0, L"MyCreateFileW ！", L"好了！", MB_OK);
    }

    return hHandle;
}

HANDLE WINAPI MyCreateFileA(
    __in     LPCSTR lpFileName,
    __in     DWORD dwDesiredAccess,
    __in     DWORD dwShareMode,
    __in_opt LPSECURITY_ATTRIBUTES lpSecurityAttributes,
    __in     DWORD dwCreationDisposition,
    __in     DWORD dwFlagsAndAttributes,
    __in_opt HANDLE hTemplateFile
)
{
    HANDLE hHandle = NULL;

    // 执行钩子
    if (realCreateFileA == NULL) {
        OutputDebugString(L"realCreateFileA is NULL\n");
        return INVALID_HANDLE_VALUE;
    } else {
        OutputDebugString(L"realCreateFileA is not NULL\n");
        hHandle = (realCreateFileA)(lpFileName, dwDesiredAccess, dwShareMode,
                                    lpSecurityAttributes, dwCreationDisposition, dwFlagsAndAttributes, hTemplateFile);
        OutputDebugString(L"MyCreateFileW : ");
        OutputDebugStringA(lpFileName);
        OutputDebugString(L"\n");
        MessageBox(0, L"MyCreateFileA！", L"好了！", MB_OK);
    }

    return hHandle;
}

BOOL MyPlaySoundW(LPCWSTR pszSound, HMODULE hwnd, DWORD fdwSound)
{
    BOOL result = 0;
    MessageBox(0, L"MyPlaySoundW ！", L"好了！", MB_OK);

    // 执行钩子
    if (realPlaySoundW == NULL) {
        OutputDebugString(L"realPlaySoundW is NULL\n");
        return -1;
    } else {
        OutputDebugString(L"realPlaySoundW is not NULL\n");
        result = (realPlaySoundW)(pszSound, hwnd, fdwSound);
        OutputDebugString(L"MyPlaySoundW : ");
        OutputDebugStringW(pszSound);
        OutputDebugString(L"\n");
    }

    return result;
}
//-----------------------------------




BOOL APIENTRY DllMain(HMODULE hModule,
                      DWORD  ul_reason_for_call,
                      LPVOID lpReserved
                     )
{
    //MessageBox(0, L"DllMain！", L"好了！", MB_OK);
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        OutputDebugString(L"DllMain::DLL_PROCESS_ATTACH\n");
        // 准备好原始地址与目的地址
        int errCode = PrepareRealApiEntry();

        if (errCode != 0) {
            OutputDebugString(L"PrepareRealApiEntry() Error\n");
            return FALSE;
        }

        // 开始挂钩
        DoHook();
        break;
    }

    case DLL_THREAD_ATTACH: {
        OutputDebugString(L"DllMain::DLL_THREAD_ATTACH\n");
        break;
    }

    case DLL_THREAD_DETACH: {
        OutputDebugString(L"DllMain::DLL_THREAD_DETACH\n");
        break;
    }

    case DLL_PROCESS_DETACH: {
        OutputDebugString(L"DllMain::DLL_PROCESS_DETACH\n");
        // 卸载钩子
        DoneHook();
        break;
    }
    }

    return TRUE;
}