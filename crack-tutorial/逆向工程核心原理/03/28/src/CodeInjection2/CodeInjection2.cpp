// CodeInjection2.cpp
// reversecore@gmail.com
// http://www.reversecore.com

#include "windows.h"
#include "stdio.h"

typedef struct _THREAD_PARAM 
{
    FARPROC pFunc[2];               // LoadLibraryA(), GetProcAddress()
} THREAD_PARAM, *PTHREAD_PARAM;

BYTE g_InjectionCode[] = 
{
    0x55, 0x8B, 0xEC, 0x8B, 0x75, 0x08, 0x68, 0x6C, 0x6C, 0x00,
    0x00, 0x68, 0x33, 0x32, 0x2E, 0x64, 0x68, 0x75, 0x73, 0x65,
    0x72, 0x54, 0xFF, 0x16, 0x68, 0x6F, 0x78, 0x41, 0x00, 0x68,
    0x61, 0x67, 0x65, 0x42, 0x68, 0x4D, 0x65, 0x73, 0x73, 0x54,
    0x50, 0xFF, 0x56, 0x04, 0x6A, 0x00, 0xE8, 0x0C, 0x00, 0x00,
    0x00, 0x52, 0x65, 0x76, 0x65, 0x72, 0x73, 0x65, 0x43, 0x6F,
    0x72, 0x65, 0x00, 0xE8, 0x14, 0x00, 0x00, 0x00, 0x77, 0x77,
    0x77, 0x2E, 0x72, 0x65, 0x76, 0x65, 0x72, 0x73, 0x65, 0x63,
    0x6F, 0x72, 0x65, 0x2E, 0x63, 0x6F, 0x6D, 0x00, 0x6A, 0x00,
    0xFF, 0xD0, 0x33, 0xC0, 0x8B, 0xE5, 0x5D, 0xC3
};

/*
004010ED    55               PUSH EBP
004010EE    8BEC             MOV EBP,ESP
004010F0    8B75 08          MOV ESI,DWORD PTR SS:[EBP+8]       ; ESI = pParam           
004010F3    68 6C6C0000      PUSH 6C6C                      
004010F8    68 33322E64      PUSH 642E3233
004010FD    68 75736572      PUSH 72657375
00401102    54               PUSH ESP                           ; - "user32.dll"
00401103    FF16             CALL DWORD PTR DS:[ESI]            ; LoadLibraryA("user32.dll")
00401105    68 6F784100      PUSH 41786F
0040110A    68 61676542      PUSH 42656761
0040110F    68 4D657373      PUSH 7373654D
00401114    54               PUSH ESP                           ; - "MessageBoxA"
00401115    50               PUSH EAX                           ; - hMod
00401116    FF56 04          CALL DWORD PTR DS:[ESI+4]          ; GetProcAddress(hMod, "MessageBoxA")
00401119    6A 00            PUSH 0                             ; - MB_OK (0)
0040111B    E8 0C000000      CALL 0040112C
00401120                     <ASCII>                            ; - "ReverseCore", 0
0040112C    E8 14000000      CALL 00401145
00401131                     <ASCII>                            ; - "www.reversecore.com", 0
00401145    6A 00            PUSH 0                             ; - hWnd (0)
00401147    FFD0             CALL EAX                           ; MessageBoxA(0, "www.reversecore.com", "ReverseCore", 0)
00401149    33C0             XOR EAX,EAX                        
0040114B    8BE5             MOV ESP,EBP
0040114D    5D               POP EBP                            
0040114E    C3               RETN
*/

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

BOOL InjectCode(DWORD dwPID)
{
    HMODULE         hMod            = NULL;
    THREAD_PARAM    param           = {0,};
    HANDLE          hProcess        = NULL;
    HANDLE          hThread         = NULL;
    LPVOID          pRemoteBuf[2]   = {0,};

    hMod = GetModuleHandleA("kernel32.dll");

    // set THREAD_PARAM
    param.pFunc[0] = GetProcAddress(hMod, "LoadLibraryA");
    param.pFunc[1] = GetProcAddress(hMod, "GetProcAddress");

    // Open Process
    if ( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS,               // dwDesiredAccess
                                  FALSE,                            // bInheritHandle
                                  dwPID)) )                         // dwProcessId
    {
        printf("OpenProcess() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    // Allocation for THREAD_PARAM
    if( !(pRemoteBuf[0] = VirtualAllocEx(hProcess,                  // hProcess
                                         NULL,                      // lpAddress
                                         sizeof(THREAD_PARAM),      // dwSize
                                         MEM_COMMIT,                // flAllocationType
                                         PAGE_READWRITE)) )         // flProtect
    {
        printf("VirtualAllocEx() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    if( !WriteProcessMemory(hProcess,                               // hProcess
                            pRemoteBuf[0],                          // lpBaseAddress
                            (LPVOID)&param,                         // lpBuffer
                            sizeof(THREAD_PARAM),                   // nSize
                            NULL) )                                 // [out] lpNumberOfBytesWritten
    {
        printf("WriteProcessMemory() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    // Allocation for ThreadProc()
    if( !(pRemoteBuf[1] = VirtualAllocEx(hProcess,                  // hProcess
                                         NULL,                      // lpAddress
                                         sizeof(g_InjectionCode),   // dwSize
                                         MEM_COMMIT,                // flAllocationType
                                         PAGE_EXECUTE_READWRITE)) ) // flProtect
    {
        printf("VirtualAllocEx() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    if( !WriteProcessMemory(hProcess,                               // hProcess
                            pRemoteBuf[1],                          // lpBaseAddress
                            (LPVOID)&g_InjectionCode,               // lpBuffer
                            sizeof(g_InjectionCode),                // nSize
                            NULL) )                                 // [out] lpNumberOfBytesWritten
    {
        printf("WriteProcessMemory() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    if( !(hThread = CreateRemoteThread(hProcess,                    // hProcess
                                       NULL,                        // lpThreadAttributes
                                       0,                           // dwStackSize
                                       (LPTHREAD_START_ROUTINE)pRemoteBuf[1],
                                       pRemoteBuf[0],               // lpParameter
                                       0,                           // dwCreationFlags
                                       NULL)) )                     // lpThreadId
    {
        printf("CreateRemoteThread() fail : err_code = %d\n", GetLastError());
        return FALSE;
    }

    WaitForSingleObject(hThread, INFINITE);	

    CloseHandle(hThread);
    CloseHandle(hProcess);

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
