#include "stdafx.h"

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

            // BUG! Leaked NT heap memory allocation.
            ::HeapAlloc(::GetProcessHeap(), 0, 50000);
            // BUG! Leaked event handle.
            ::CreateEvent(NULL, FALSE, FALSE, NULL);

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
