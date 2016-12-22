#include "windows.h"

DWORD WINAPI ThreadProc(LPVOID lParam)
{
    MessageBoxA(NULL, "www.reversecore.com", "ReverseCore", MB_OK);

    return 0;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH : 
            CreateThread(NULL, 0, ThreadProc, NULL, 0, NULL);
            break;
    }

    return TRUE;
}
