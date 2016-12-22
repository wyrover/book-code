#include "windows.h"
#include "stdio.h"
#include "tchar.h"

BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) 
{
    TOKEN_PRIVILEGES tp;
    HANDLE hToken;
    LUID luid;

    if( !OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, 
			              &hToken) )
    {
        _tprintf(L"OpenProcessToken error: %u\n", GetLastError());
        return FALSE;
    }

    if( !LookupPrivilegeValue(NULL,             // lookup privilege on local system
                              lpszPrivilege,    // privilege to lookup 
                              &luid) )          // receives LUID of privilege
    {
        _tprintf(L"LookupPrivilegeValue error: %u\n", GetLastError() ); 
        return FALSE; 
    }

    tp.PrivilegeCount = 1;
    tp.Privileges[0].Luid = luid;
    if( bEnablePrivilege )
        tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    else
        tp.Privileges[0].Attributes = 0;

    // Enable the privilege or disable all privileges.
    if( !AdjustTokenPrivileges(hToken, 
                               FALSE, 
                               &tp, 
                               sizeof(TOKEN_PRIVILEGES), 
                               (PTOKEN_PRIVILEGES) NULL, 
                               (PDWORD) NULL) )
    { 
        _tprintf(L"AdjustTokenPrivileges error: %u\n", GetLastError() ); 
        return FALSE; 
    } 

    if( GetLastError() == ERROR_NOT_ALL_ASSIGNED )
    {
        _tprintf(L"The token does not have the specified privilege. \n");
        return FALSE;
    } 

    return TRUE;
}

typedef DWORD (WINAPI *PFNTCREATETHREADEX)
( 
    PHANDLE                 ThreadHandle,	
    ACCESS_MASK             DesiredAccess,	
    LPVOID                  ObjectAttributes,	
    HANDLE                  ProcessHandle,	
    LPTHREAD_START_ROUTINE  lpStartAddress,	
    LPVOID                  lpParameter,	
    BOOL	                CreateSuspended,	
    DWORD                   dwStackSize,	
    DWORD                   dw1, 
    DWORD                   dw2, 
    LPVOID                  Unknown 
); 

BOOL IsVistaOrLater()
{
    OSVERSIONINFO osvi;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    if( osvi.dwMajorVersion >= 6 )
        return TRUE;

    return FALSE;
}

BOOL MyCreateRemoteThread(HANDLE hProcess, LPTHREAD_START_ROUTINE pThreadProc, LPVOID pRemoteBuf)
{
    HANDLE      hThread = NULL;
    FARPROC     pFunc = NULL;

    if( IsVistaOrLater() )    // Vista, 7, Server2008
    {
        pFunc = GetProcAddress(GetModuleHandle(L"ntdll.dll"), "NtCreateThreadEx");
        if( pFunc == NULL )
        {
            printf("MyCreateRemoteThread() : GetProcAddress(\"NtCreateThreadEx\") failed!!! [%d]\n",
                   GetLastError());
            return FALSE;
        }

        ((PFNTCREATETHREADEX)pFunc)(&hThread,
                                    0x1FFFFF,
                                    NULL,
                                    hProcess,
                                    pThreadProc,
                                    pRemoteBuf,
                                    FALSE,
                                    NULL,
                                    NULL,
                                    NULL,
                                    NULL);
        if( hThread == NULL )
        {
            printf("MyCreateRemoteThread() : NtCreateThreadEx() failed!!! [%d]\n", GetLastError());
            return FALSE;
        }
    }
    else                    // 2000, XP, Server2003
    {
        hThread = CreateRemoteThread(hProcess, 
                                     NULL, 
                                     0, 
                                     pThreadProc, 
                                     pRemoteBuf, 
                                     0, 
                                     NULL);
        if( hThread == NULL )
        {
            printf("MyCreateRemoteThread() : CreateRemoteThread() failed!!! [%d]\n", GetLastError());
            return FALSE;
        }
    }

	if( WAIT_FAILED == WaitForSingleObject(hThread, INFINITE) )
    {
        printf("MyCreateRemoteThread() : WaitForSingleObject() failed!!! [%d]\n", GetLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL InjectDll(DWORD dwPID, char *szDllName)
{
    HANDLE hProcess = NULL;
    LPVOID pRemoteBuf = NULL;
    FARPROC pThreadProc = NULL;
    DWORD dwBufSize = strlen(szDllName)+1;

    if ( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) )
    {
        printf("[ERROR] OpenProcess(%d) failed!!! [%d]\n", 
        dwPID, GetLastError());
        return FALSE;
    }

    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, 
                                MEM_COMMIT, PAGE_READWRITE);

    WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllName, 
                       dwBufSize, NULL);

    pThreadProc = GetProcAddress(GetModuleHandle(L"kernel32.dll"), 
                                 "LoadLibraryA");

    if( !MyCreateRemoteThread(hProcess, (LPTHREAD_START_ROUTINE)pThreadProc, pRemoteBuf) )
    {
        printf("[ERROR] MyCreateRemoteThread() failed!!!\n");
        return FALSE;
    }

    VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);

    CloseHandle(hProcess);

    return TRUE;
}

int main(int argc, char *argv[])
{
	// adjust privilege
    SetPrivilege(SE_DEBUG_NAME, TRUE);

    // InjectDll.exe <PID> <dll_path>
    if( argc != 3 )
    {
        printf("usage : %s <PID> <dll_path>\n", argv[0]);
        return 1;
    }

    if( !InjectDll((DWORD)atoi(argv[1]), argv[2]) )
    {
        printf("InjectDll() failed!!!\n");
        return 1;
    }

    printf("InjectDll() succeeded!!!\n");

    return 0;
}
