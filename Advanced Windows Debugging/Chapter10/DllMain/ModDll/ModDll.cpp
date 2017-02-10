#include <windows.h>
#include <stdio.h>


DWORD WINAPI InitDllProc( LPVOID lpParam ) 
{ 
    LoadLibrary(L"ole32.dll");                
    return 1;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    BOOL bRet=FALSE;
    switch(fdwReason)
    {
        case DLL_PROCESS_ATTACH:
        {
            DWORD dwId=0;
            HANDLE hThread=NULL;

            hThread = CreateThread(NULL, 0, InitDllProc, NULL, 0, &dwId);        
            if(hThread)
            {
                WaitForSingleObject(hThread, INFINITE);
                CloseHandle(hThread);
                bRet=TRUE;
            }
        }
        break;
    }

    return bRet;
}



