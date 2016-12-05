#include "windows.h"
#include "tchar.h"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    TCHAR   szPath[MAX_PATH]    = {0,};
    TCHAR   szMsg[1024]         = {0,};
    TCHAR   *p                  = NULL;

    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH : 
            GetModuleFileName(NULL, szPath, MAX_PATH);
            p = _tcsrchr(szPath, L'\\');
            if( p != NULL )
            {
                _stprintf_s(szMsg, 1024 - sizeof(TCHAR), 
                            L"Injected in %s(%d)", 
                            p + 1,                          // Process Name
                            GetCurrentProcessId());         // PID
                OutputDebugString(szMsg);
            }
            
            break;
    }

    return TRUE;
}
