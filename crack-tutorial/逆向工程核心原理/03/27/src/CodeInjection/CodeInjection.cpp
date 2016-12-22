// CodeInjection.cpp
// reversecore@gmail.com
// http://www.reversecore.com

#include "windows.h"
#include "stdio.h"

typedef struct _THREAD_PARAM 
{
    FARPROC pFunc[2];               // LoadLibraryA(), GetProcAddress()
    char    szBuf[4][128];          // "user32.dll", "MessageBoxA", "www.reversecore.com", "ReverseCore"
} THREAD_PARAM, *PTHREAD_PARAM;

typedef HMODULE (WINAPI *PFLOADLIBRARYA)
(
    LPCSTR lpLibFileName
);

typedef FARPROC (WINAPI *PFGETPROCADDRESS)
(
    HMODULE hModule,
    LPCSTR lpProcName
);

typedef int (WINAPI *PFMESSAGEBOXA)
(
    HWND hWnd,
    LPCSTR lpText,
    LPCSTR lpCaption,
    UINT uType
);

DWORD WINAPI ThreadProc(LPVOID lParam)
{
    PTHREAD_PARAM   pParam      = (PTHREAD_PARAM)lParam;
    HMODULE         hMod        = NULL;
    FARPROC         pFunc       = NULL;

    // LoadLibrary()
    hMod = ((PFLOADLIBRARYA)pParam->pFunc[0])(pParam->szBuf[0]);    // "user32.dll"
    if( !hMod )
        return 1;

    // GetProcAddress()
    pFunc = (FARPROC)((PFGETPROCADDRESS)pParam->pFunc[1])(hMod, pParam->szBuf[1]);  // "MessageBoxA"
    if( !pFunc )
        return 1;

    // MessageBoxA()
    ((PFMESSAGEBOXA)pFunc)(NULL, pParam->szBuf[2], pParam->szBuf[3], MB_OK);

    return 0;
}

BOOL InjectCode(DWORD dwPID)
{
    HMODULE         hMod            = NULL;
    THREAD_PARAM    param           = {0,};
    HANDLE          hProcess        = NULL;
    HANDLE          hThread         = NULL;
    LPVOID          pRemoteBuf[2]   = {0,};
    DWORD           dwSize          = 0;

    hMod = GetModuleHandleA("kernel32.dll");

    // set THREAD_PARAM
    param.pFunc[0] = GetProcAddress(hMod, "LoadLibraryA");
    param.pFunc[1] = GetProcAddress(hMod, "GetProcAddress");
    strcpy_s(param.szBuf[0], "user32.dll");
    strcpy_s(param.szBuf[1], "MessageBoxA");
    strcpy_s(param.szBuf[2], "www.reversecore.com");
    strcpy_s(param.szBuf[3], "ReverseCore");

    // Open Process
    if ( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS,   // dwDesiredAccess
                                  FALSE,                // bInheritHandle
                                  dwPID)) )             // dwProcessId
    {
        printf("OpenProcess() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    // Allocation for THREAD_PARAM
    dwSize = sizeof(THREAD_PARAM);
    if( !(pRemoteBuf[0] = VirtualAllocEx(hProcess,          // hProcess
                                      NULL,                 // lpAddress
                                      dwSize,               // dwSize
                                      MEM_COMMIT,           // flAllocationType
                                      PAGE_READWRITE)) )    // flProtect
    {
        printf("VirtualAllocEx() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    if( !WriteProcessMemory(hProcess,                       // hProcess
                            pRemoteBuf[0],                  // lpBaseAddress
                            (LPVOID)&param,                 // lpBuffer
                            dwSize,                         // nSize
                            NULL) )                         // [out] lpNumberOfBytesWritten
    {
        printf("WriteProcessMemory() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    // Allocation for ThreadProc()
    dwSize = (DWORD)InjectCode - (DWORD)ThreadProc;
    if( !(pRemoteBuf[1] = VirtualAllocEx(hProcess,          // hProcess
                                      NULL,                 // lpAddress
                                      dwSize,               // dwSize
                                      MEM_COMMIT,           // flAllocationType
                                      PAGE_EXECUTE_READWRITE)) )    // flProtect
    {
        printf("VirtualAllocEx() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    if( !WriteProcessMemory(hProcess,                       // hProcess
                            pRemoteBuf[1],                  // lpBaseAddress
                            (LPVOID)ThreadProc,             // lpBuffer
                            dwSize,                         // nSize
                            NULL) )                         // [out] lpNumberOfBytesWritten
    {
        printf("WriteProcessMemory() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    if( !(hThread = CreateRemoteThread(hProcess,            // hProcess
                                       NULL,                // lpThreadAttributes
                                       0,                   // dwStackSize
                                       (LPTHREAD_START_ROUTINE)pRemoteBuf[1],     // dwStackSize
                                       pRemoteBuf[0],       // lpParameter
                                       0,                   // dwCreationFlags
                                       NULL)) )             // lpThreadId
    {
        printf("CreateRemoteThread() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);	

    CloseHandle(hThread);
    CloseHandle(hProcess);

    return TRUE;
}

BOOL SetPrivilege(LPCTSTR lpszPrivilege, BOOL bEnablePrivilege) 
{
    TOKEN_PRIVILEGES tp;
    HANDLE hToken;
    LUID luid;

    if( !OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, 
			              &hToken) )
    {
        printf("OpenProcessToken error: %u\n", GetLastError());
        return FALSE;
    }

    if( !LookupPrivilegeValue(NULL,           // lookup privilege on local system
                              lpszPrivilege,  // privilege to lookup 
                              &luid) )        // receives LUID of privilege
    {
        printf("LookupPrivilegeValue error: %u\n", GetLastError() ); 
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
        printf("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
        return FALSE; 
    } 

    if( GetLastError() == ERROR_NOT_ALL_ASSIGNED )
    {
        printf("The token does not have the specified privilege. \n");
        return FALSE;
    } 

    return TRUE;
}

int main(int argc, char *argv[])
{
    DWORD dwPID     = 0;

	if( argc != 2 )
	{
	    printf("\n USAGE  : %s <pid>\n", argv[0]);
		return 1;
	}

	// change privilege
	if( !SetPrivilege(SE_DEBUG_NAME, TRUE) )
        return 1;

    // code injection
    dwPID = (DWORD)atol(argv[1]);
    InjectCode(dwPID);

	return 0;
}
