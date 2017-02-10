#include "stdafx.h"

DWORD
WINAPI
WorkerThread(
    __in LPVOID pParam
    )
{
    wprintf(L"Inside WorkerThread.\n");
    return EXIT_SUCCESS;
    UNREFERENCED_PARAMETER(pParam);
}

BOOL
WINAPI
DllMain(
    __in HINSTANCE hInstance,
    __in DWORD dwReason,
    __in PVOID pReserved
    )
{
    CHandle shThread;

    ChkProlog();

    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            ::DisableThreadLibraryCalls(hInstance);

            shThread.Attach(::CreateThread(NULL, 0, WorkerThread, NULL, 0, NULL));
            ChkWin32(shThread);
            VERIFY(::WaitForSingleObject(shThread, INFINITE) == WAIT_OBJECT_0);
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            ASSERT(FALSE);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    ChkNoCleanupRet(SUCCEEDED(ChkGetHr()));
    UNREFERENCED_PARAMETER(pReserved);
}
