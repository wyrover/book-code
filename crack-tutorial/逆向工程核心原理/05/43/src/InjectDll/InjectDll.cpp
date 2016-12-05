#include "windows.h"
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

    if( !LookupPrivilegeValue(NULL,           // lookup privilege on local system
                              lpszPrivilege,  // privilege to lookup 
                              &luid) )        // receives LUID of privilege
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

BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
    HANDLE hProcess = NULL, hThread = NULL;
    HMODULE hMod = NULL;
    LPVOID pRemoteBuf = NULL;
    DWORD dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
    LPTHREAD_START_ROUTINE pThreadProc;
    BOOL bRet = TRUE;

    // #1. dwPID 를 이용하여 대상 프로세스(notepad.exe)의 HANDLE을 구한다.
    if ( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) )
    {
        _tprintf(L"OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
        return FALSE;
    }

    // #2. 대상 프로세스(notepad.exe) 메모리에 szDllName 크기만큼 메모리를 할당한다.
    pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, MEM_COMMIT, PAGE_READWRITE);

    // #3. 할당 받은 메모리에 myhack.dll 경로("c:\\myhack.dll")를 쓴다.
    WriteProcessMemory(hProcess, pRemoteBuf, (LPVOID)szDllPath, dwBufSize, NULL);

    // #4. LoadLibraryA() API 주소를 구한다.
    hMod = GetModuleHandle(L"kernel32.dll");
    pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
	
    // #5. notepad.exe 프로세스에 스레드를 실행
    hThread = CreateRemoteThread(hProcess, NULL, 0, pThreadProc, pRemoteBuf, 0, NULL);
    if( hThread == NULL )
    {
        _tprintf(L"[ERROR] CreateRemoteThread() failed!!! [%d]\n", GetLastError());
        bRet = FALSE;
        goto _ERROR;
    }

    WaitForSingleObject(hThread, INFINITE);	

_ERROR:
    
    if( pRemoteBuf )
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);

    if( hThread )
        CloseHandle(hThread);

    if( hProcess )
        CloseHandle(hProcess);

    return bRet;
}

int _tmain(int argc, TCHAR *argv[])
{
    if( argc != 3)
    {
        _tprintf(L"USAGE : %s <pid> <dll_path>\n", argv[0]);
        return 1;
    }

    // change privilege
    if( !SetPrivilege(SE_DEBUG_NAME, TRUE) )
        return 1;

    // inject dll
    if( InjectDll((DWORD)_tstol(argv[1]), argv[2]) )
        _tprintf(L"InjectDll() success!!!\n");
    else
        _tprintf(L"InjectDll() failed!!!\n");

    return 0;
}