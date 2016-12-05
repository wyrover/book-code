// redirect.cpp

#include "windows.h"
#include "wininet.h"
#include "stdio.h"
#include "tchar.h"

#define STR_MODULE_NAME					    (L"redirect.dll")
#define STATUS_SUCCESS						(0x00000000L) 

typedef LONG NTSTATUS;

typedef struct _CLIENT_ID {
	HANDLE UniqueProcess;
	HANDLE UniqueThread;
} CLIENT_ID;

typedef struct _THREAD_BASIC_INFORMATION {
	NTSTATUS ExitStatus;
	PVOID TebBaseAddress;
	CLIENT_ID ClientId;
	ULONG AffinityMask;
	LONG Priority;
	LONG BasePriority;
} THREAD_BASIC_INFORMATION;

typedef NTSTATUS (WINAPI *PFZWRESUMETHREAD)
(
    HANDLE ThreadHandle, 
    PULONG SuspendCount
);

typedef NTSTATUS (WINAPI *PFZWQUERYINFORMATIONTHREAD)
(
    HANDLE ThreadHandle, 
    ULONG ThreadInformationClass, 
    PVOID ThreadInformation, 
    ULONG ThreadInformationLength, 
    PULONG ReturnLength
);

typedef HINTERNET (WINAPI *PFINTERNETCONNECTW)
(
    HINTERNET hInternet,
    LPCWSTR lpszServerName,
    INTERNET_PORT nServerPort,
    LPCTSTR lpszUsername,
    LPCTSTR lpszPassword,
    DWORD dwService,
    DWORD dwFlags,
    DWORD_PTR dwContext
);

BYTE g_pZWRT[5] = {0,};
BYTE g_pICW[5] = {0,};

void DebugLog(const char *format, ...)
{
	va_list vl;
	FILE *pf = NULL;
	char szLog[512] = {0,};

	va_start(vl, format);
	wsprintfA(szLog, format, vl);
	va_end(vl);

    OutputDebugStringA(szLog);
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
        DebugLog("OpenProcessToken error: %u\n", GetLastError());
        return FALSE;
    }

    if( !LookupPrivilegeValue(NULL,             // lookup privilege on local system
                              lpszPrivilege,    // privilege to lookup 
                              &luid) )          // receives LUID of privilege
    {
        DebugLog("LookupPrivilegeValue error: %u\n", GetLastError() ); 
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
        DebugLog("AdjustTokenPrivileges error: %u\n", GetLastError() ); 
        return FALSE; 
    } 

    if( GetLastError() == ERROR_NOT_ALL_ASSIGNED )
    {
        DebugLog("The token does not have the specified privilege. \n");
        return FALSE;
    } 

    return TRUE;
}

BOOL hook_by_code(LPCSTR szDllName, LPCSTR szFuncName, PROC pfnNew, PBYTE pOrgBytes)
{
	FARPROC pFunc = NULL;
	DWORD dwOldProtect = 0, dwAddress = 0;
	BYTE pBuf[5] = {0xE9, 0, };
	PBYTE pByte = NULL;
    HMODULE hMod = NULL;

    hMod = GetModuleHandleA(szDllName);
    if( hMod == NULL )
    {
        DebugLog("hook_by_code() : GetModuleHandle(\"%s\") failed!!! [%d]\n",
                  szDllName, GetLastError());
        return FALSE;
    }

	pFunc = (FARPROC)GetProcAddress(hMod, szFuncName);
    if( pFunc == NULL )
    {
        DebugLog("hook_by_code() : GetProcAddress(\"%s\") failed!!! [%d]\n",
                  szFuncName, GetLastError());
        return FALSE;
    }

	pByte = (PBYTE)pFunc;
	if( pByte[0] == 0xE9 )
    {
        DebugLog("hook_by_code() : The API is hooked already!!!\n");
		return FALSE;
    }

	if( !VirtualProtect((LPVOID)pFunc, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect) )
    {
        DebugLog("hook_by_code() : VirtualProtect(#1) failed!!! [%d]\n", GetLastError());
        return FALSE;
    }

	memcpy(pOrgBytes, pFunc, 5);

	dwAddress = (DWORD)pfnNew - (DWORD)pFunc - 5;
	memcpy(&pBuf[1], &dwAddress, 4);

	memcpy(pFunc, pBuf, 5);

	if( !VirtualProtect((LPVOID)pFunc, 5, dwOldProtect, &dwOldProtect) )
    {
        DebugLog("hook_by_code() : VirtualProtect(#2) failed!!! [%d]\n", GetLastError());
        return FALSE;
    }

	return TRUE;
}

BOOL unhook_by_code(LPCSTR szDllName, LPCSTR szFuncName, PBYTE pOrgBytes)
{
	FARPROC pFunc = NULL;
	DWORD dwOldProtect = 0;
	PBYTE pByte = NULL;
    HMODULE hMod = NULL;

    hMod = GetModuleHandleA(szDllName);
    if( hMod == NULL )
    {
        DebugLog("unhook_by_code() : GetModuleHandle(\"%s\") failed!!! [%d]\n",
                  szDllName, GetLastError());
        return FALSE;
    }

	pFunc = (FARPROC)GetProcAddress(hMod, szFuncName);
    if( pFunc == NULL )
    {
        DebugLog("unhook_by_code() : GetProcAddress(\"%s\") failed!!! [%d]\n",
                  szFuncName, GetLastError());
        return FALSE;
    }

	pByte = (PBYTE)pFunc;
	if( pByte[0] != 0xE9 )
    {
        DebugLog("unhook_by_code() : The API is unhooked already!!!");
        return FALSE;
    }

	if( !VirtualProtect((LPVOID)pFunc, 5, PAGE_EXECUTE_READWRITE, &dwOldProtect) )
    {
        DebugLog("unhook_by_code() : VirtualProtect(#1) failed!!! [%d]\n", GetLastError());
        return FALSE;
    }

	memcpy(pFunc, pOrgBytes, 5);

	if( !VirtualProtect((LPVOID)pFunc, 5, dwOldProtect, &dwOldProtect) )
    {
        DebugLog("unhook_by_code() : VirtualProtect(#2) failed!!! [%d]\n", GetLastError());
        return FALSE;
    }

	return TRUE;
}

BOOL IsVistaLater()
{
    OSVERSIONINFO osvi;

    ZeroMemory(&osvi, sizeof(OSVERSIONINFO));
    osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

    GetVersionEx(&osvi);

    if( osvi.dwMajorVersion >= 6 )
        return TRUE;

    return FALSE;
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

BOOL MyCreateRemoteThread(HANDLE hProcess, LPTHREAD_START_ROUTINE pThreadProc, LPVOID pRemoteBuf)
{
    HANDLE      hThread = NULL;
    FARPROC     pFunc = NULL;

    if( IsVistaLater() )    // Vista, 7, Server2008
    {
        pFunc = GetProcAddress(GetModuleHandleA("ntdll.dll"), "NtCreateThreadEx");
        if( pFunc == NULL )
        {
            DebugLog("MyCreateRemoteThread() : GetProcAddress() failed!!! [%d]\n",
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
            DebugLog("MyCreateRemoteThread() : NtCreateThreadEx() failed!!! [%d]\n", GetLastError());
            return FALSE;
        }
    }
    else                    // 2000, XP, Server2003
    {
        hThread = CreateRemoteThread(hProcess, NULL, 0, 
                                     pThreadProc, pRemoteBuf, 0, NULL);
        if( hThread == NULL )
        {
            DebugLog("MyCreateRemoteThread() : CreateRemoteThread() failed!!! [%d]\n", GetLastError());
            return FALSE;
        }
    }

	if( WAIT_FAILED == WaitForSingleObject(hThread, INFINITE) )
    {
        DebugLog("MyCreateRemoteThread() : WaitForSingleObject() failed!!! [%d]\n", GetLastError());
        return FALSE;
    }

    return TRUE;
}

BOOL InjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
	HANDLE                  hProcess = NULL;
    HANDLE                  hThread = NULL;
	LPVOID                  pRemoteBuf = NULL;
    DWORD                   dwBufSize = (DWORD)(_tcslen(szDllPath) + 1) * sizeof(TCHAR);
	LPTHREAD_START_ROUTINE  pThreadProc = NULL;
    BOOL                    bRet = FALSE;
    HMODULE                 hMod = NULL;

	if ( !(hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, dwPID)) )
    {
        DebugLog("InjectDll() : OpenProcess(%d) failed!!! [%d]\n", dwPID, GetLastError());
		goto INJECTDLL_EXIT;
    }

	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, 
                                MEM_COMMIT, PAGE_READWRITE);
    if( pRemoteBuf == NULL )
    {
        DebugLog("InjectDll() : VirtualAllocEx() failed!!! [%d]\n", GetLastError());
        goto INJECTDLL_EXIT;
    }

	if( !WriteProcessMemory(hProcess, pRemoteBuf, 
                           (LPVOID)szDllPath, dwBufSize, NULL) )
    {
        DebugLog("InjectDll() : WriteProcessMemory() failed!!! [%d]\n", GetLastError());
        goto INJECTDLL_EXIT;
    }

    hMod = GetModuleHandle(L"kernel32.dll");
    if( hMod == NULL )
    {
        DebugLog("InjectDll() : GetModuleHandle() failed!!! [%d]\n", GetLastError());
        goto INJECTDLL_EXIT;
    }

	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
    if( pThreadProc == NULL )
    {
        DebugLog("InjectDll() : GetProcAddress() failed!!! [%d]\n", GetLastError());
        goto INJECTDLL_EXIT;
    }

    if( !MyCreateRemoteThread(hProcess, pThreadProc, pRemoteBuf) )
    {
        DebugLog("InjectDll() : MyCreateRemoteThread() failed!!!\n");
        goto INJECTDLL_EXIT;
    }

    bRet = TRUE;

INJECTDLL_EXIT:

    if( pRemoteBuf )
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);

    if( hThread )
	    CloseHandle(hThread);

    if( hProcess )
	    CloseHandle(hProcess);

	return bRet;
}



NTSTATUS WINAPI NewZwResumeThread(HANDLE ThreadHandle, PULONG SuspendCount)
{
    NTSTATUS status, statusThread;
    FARPROC pFunc = NULL, pFuncThread = NULL;
    DWORD dwPID = 0;
	static DWORD dwPrevPID = 0;
    THREAD_BASIC_INFORMATION tbi;
    HMODULE hMod = NULL;
    TCHAR szModPath[MAX_PATH] = {0,};

    DebugLog("NewZwResumeThread() : start!!!\n");

    hMod = GetModuleHandle(L"ntdll.dll");
    if( hMod == NULL )
    {
        DebugLog("NewZwResumeThread() : GetModuleHandle() failed!!! [%d]\n",
                  GetLastError());
        return NULL;
    }

    // call ntdll!ZwQueryInformationThread()
    pFuncThread = GetProcAddress(hMod, "ZwQueryInformationThread");
    if( pFuncThread == NULL )
    {
        DebugLog("NewZwResumeThread() : GetProcAddress() failed!!! [%d]\n",
                  GetLastError());
        return NULL;
    }

    statusThread = ((PFZWQUERYINFORMATIONTHREAD)pFuncThread)
                   (ThreadHandle, 0, &tbi, sizeof(tbi), NULL);
    if( statusThread != STATUS_SUCCESS )
    {
        DebugLog("NewZwResumeThread() : pFuncThread() failed!!! [%d]\n", 
                 GetLastError());
        return NULL;
    }

    dwPID = (DWORD)tbi.ClientId.UniqueProcess;
    if ( (dwPID != GetCurrentProcessId()) && (dwPID != dwPrevPID) )
    {
        DebugLog("NewZwResumeThread() => call InjectDll()\n");

        dwPrevPID = dwPID;

        // change privilege
       	if( !SetPrivilege(SE_DEBUG_NAME, TRUE) )
            DebugLog("NewZwResumeThread() : SetPrivilege() failed!!!\n");

        // get injection dll path
        GetModuleFileName(GetModuleHandle(STR_MODULE_NAME), 
                          szModPath, 
                          MAX_PATH);

        if( !InjectDll(dwPID, szModPath) )
            DebugLog("NewZwResumeThread() : InjectDll(%d) failed!!!\n", dwPID);
    }

    // call ntdll!ZwResumeThread()
    if( !unhook_by_code("ntdll.dll", "ZwResumeThread", g_pZWRT) )
    {
        DebugLog("NewZwResumeThread() : unhook_by_code() failed!!!\n");
        return NULL;
    }

    pFunc = GetProcAddress(hMod, "ZwResumeThread");
    if( pFunc == NULL )
    {
        DebugLog("NewZwResumeThread() : GetProcAddress() failed!!! [%d]\n",
                  GetLastError());
        goto __NTRESUMETHREAD_END;
    }

    status = ((PFZWRESUMETHREAD)pFunc)(ThreadHandle, SuspendCount);
    if( status != STATUS_SUCCESS )
    {
        DebugLog("NewZwResumeThread() : pFunc() failed!!! [%d]\n", GetLastError());
        goto __NTRESUMETHREAD_END;
    }

__NTRESUMETHREAD_END:

    if( !hook_by_code("ntdll.dll", "ZwResumeThread", 
                      (PROC)NewZwResumeThread, g_pZWRT) )
    {
        DebugLog("NewZwResumeThread() : hook_by_code() failed!!!\n");
    }

    DebugLog("NewZwResumeThread() : end!!!\n");

    return status;
}




HINTERNET WINAPI NewInternetConnectW
(
    HINTERNET hInternet,
    LPCWSTR lpszServerName,
    INTERNET_PORT nServerPort,
    LPCTSTR lpszUsername,
    LPCTSTR lpszPassword,
    DWORD dwService,
    DWORD dwFlags,
    DWORD_PTR dwContext
)
{
    HINTERNET hInt = NULL;
    FARPROC pFunc = NULL;
    HMODULE hMod = NULL;

    // unhook
    if( !unhook_by_code("wininet.dll", "InternetConnectW", g_pICW) )
    {
        DebugLog("NewInternetConnectW() : unhook_by_code() failed!!!\n");
        return NULL;
    }

    // call original API
    hMod = GetModuleHandle(L"wininet.dll");
    if( hMod == NULL )
    {
        DebugLog("NewInternetConnectW() : GetModuleHandle() failed!!! [%d]\n",
                  GetLastError());
        goto __INTERNETCONNECT_EXIT;
    }

    pFunc = GetProcAddress(hMod, "InternetConnectW");
    if( pFunc == NULL )
    {
        DebugLog("NewInternetConnectW() : GetProcAddress() failed!!! [%d]\n",
                  GetLastError());
        goto __INTERNETCONNECT_EXIT;
    }

    if( !_tcsicmp(lpszServerName, L"www.naver.com") ||
        !_tcsicmp(lpszServerName, L"www.daum.net") ||
        !_tcsicmp(lpszServerName, L"www.nate.com") || 
        !_tcsicmp(lpszServerName, L"www.yahoo.com") )
    {
        DebugLog("[redirect] naver, daum, nate, yahoo => reversecore\n");
        hInt = ((PFINTERNETCONNECTW)pFunc)(hInternet,
                                           L"www.reversecore.com",
                                           nServerPort,
                                           lpszUsername,
                                           lpszPassword,
                                           dwService,
                                           dwFlags,
                                           dwContext);
    }
    else
    {
        DebugLog("[no redirect]\n");
        hInt = ((PFINTERNETCONNECTW)pFunc)(hInternet,
                                           lpszServerName,
                                           nServerPort,
                                           lpszUsername,
                                           lpszPassword,
                                           dwService,
                                           dwFlags,
                                           dwContext);
    }

__INTERNETCONNECT_EXIT:

    // hook
    if( !hook_by_code("wininet.dll", "InternetConnectW", 
                      (PROC)NewInternetConnectW, g_pICW) )
    {
        DebugLog("NewInternetConnectW() : hook_by_code() failed!!!\n");
    }
    
    return hInt;
}

BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    char            szCurProc[MAX_PATH] = {0,};
    char            *p = NULL;

    switch( fdwReason )
    {
        case DLL_PROCESS_ATTACH : 
            DebugLog("DllMain() : DLL_PROCESS_ATTACH\n");

            GetModuleFileNameA(NULL, szCurProc, MAX_PATH);
            p = strrchr(szCurProc, '\\');
            if( (p != NULL) && !_stricmp(p+1, "iexplore.exe") )
            {
                DebugLog("DllMain() : current process is [iexplore.exe]\n");

                // wininet!InternetConnectW() API 를 후킹하기 전에 
                // 미리 wininet.dll 을 로딩시킴
                if( NULL == LoadLibrary(L"wininet.dll") )
                {
                    DebugLog("DllMain() : LoadLibrary() failed!!! [%d]\n",
                             GetLastError());
                }
            }

            // hook
            hook_by_code("ntdll.dll", "ZwResumeThread", 
                         (PROC)NewZwResumeThread, g_pZWRT);
            hook_by_code("wininet.dll", "InternetConnectW", 
                         (PROC)NewInternetConnectW, g_pICW);
            break;

        case DLL_PROCESS_DETACH :
            DebugLog("DllMain() : DLL_PROCESS_DETACH\n");

            // unhook
            unhook_by_code("ntdll.dll", "ZwResumeThread", 
                           g_pZWRT);
            unhook_by_code("wininet.dll", "InternetConnectW", 
                           g_pICW);
            break;
    }

    return TRUE;
}
