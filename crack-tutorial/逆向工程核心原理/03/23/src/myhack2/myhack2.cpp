// myhack2.cpp

#include "windows.h"
#include "tchar.h"

#define DEF_CMD  L"c:\\Program Files\\Internet Explorer\\iexplore.exe" 
#define DEF_ADDR L"http://www.naver.com"
#define DEF_DST_PROC L"notepad.exe"

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    TCHAR szCmd[MAX_PATH]  = {0,};
    TCHAR szPath[MAX_PATH] = {0,};
    TCHAR *p = NULL;
    STARTUPINFO si = {0,};
    PROCESS_INFORMATION pi = {0,};

    si.cb = sizeof(STARTUPINFO);
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;

    switch( fdwReason )
    {
    case DLL_PROCESS_ATTACH : 
        if( !GetModuleFileName( NULL, szPath, MAX_PATH ) )
            break;
   
        if( !(p = _tcsrchr(szPath, '\\')) )
            break;

        if( _tcsicmp(p+1, DEF_DST_PROC) )
            break;

        wsprintf(szCmd, L"%s %s", DEF_CMD, DEF_ADDR);
        if( !CreateProcess(NULL, (LPTSTR)(LPCTSTR)szCmd, 
                            NULL, NULL, FALSE, 
                            NORMAL_PRIORITY_CLASS, 
                            NULL, NULL, &si, &pi) )
            break;

        if( pi.hProcess != NULL )
            CloseHandle(pi.hProcess);

        break;
    }
   
    return TRUE;
}
