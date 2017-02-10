#include "stdafx.h"

BOOL g_bGlobalVar = FALSE;

BOOL
WINAPI
DllMain(
    __in HINSTANCE hInstance,
    __in DWORD dwReason,
    __in PVOID pReserved
    )
{
    switch (dwReason)
    {
        case DLL_PROCESS_ATTACH:
            ::DisableThreadLibraryCalls(hInstance);
            break;

        case DLL_THREAD_ATTACH:
        case DLL_THREAD_DETACH:
            ASSERT(FALSE);
            break;

        case DLL_PROCESS_DETACH:
            break;
    }

    return TRUE;

    UNREFERENCED_PARAMETER(pReserved);
}
