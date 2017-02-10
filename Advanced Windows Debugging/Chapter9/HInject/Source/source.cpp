#include <windows.h>
#include <stdio.h>
#include <conio.h>

HANDLE GetToken()
{
    HANDLE hToken = INVALID_HANDLE_VALUE;
    BOOL bRet = OpenThreadToken (GetCurrentThread(),
                                  TOKEN_READ,
                                  TRUE,
                                  &hToken
                                  );
    if(bRet==FALSE)
    {
        bRet=OpenProcessToken(GetCurrentProcess(),
                              TOKEN_READ,
                              &hToken
                              );
    }
    return hToken;
}

void __cdecl wmain (int argc, wchar_t* pArgs[])
{
    DWORD dwProcessId=0;
    BOOL bContinue=TRUE;
    while(bContinue)
    {
        wprintf(L"\nEnter process ID to inject handle into: ");
        wscanf(L"%d", &dwProcessId);
        HANDLE hProcess=OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwProcessId);
        if(hProcess==NULL)
        {
            wprintf(L"Failed to open process handle [PID:%d, Error:%d]\n", dwProcessId, GetLastError());
        }
        else
        {
            HANDLE hHandle=GetToken();
            HANDLE hDup=NULL;
            BOOL bRet=DuplicateHandle(GetCurrentProcess(), hHandle, hProcess, &hDup, 0, FALSE, DUPLICATE_SAME_ACCESS);
            if(bRet==FALSE)
            {
                wprintf(L"Failed to duplicate handle [Error=%d]\n",GetLastError());
            }
            CloseHandle(hHandle);
            CloseHandle(hProcess);
        }
    }
}

