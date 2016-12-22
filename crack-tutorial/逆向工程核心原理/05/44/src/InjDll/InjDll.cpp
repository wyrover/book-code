// InjDll.cpp
// reversecore@gmail.com
// www.reversecore.com

#include "windows.h"
#include "stdio.h"
#include "tlhelp32.h"
#include "io.h"
#include "tchar.h"

enum {INJECTION_MODE = 0, EJECTION_MODE};

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

BOOL MyCreateRemoteThread
(
    HANDLE hProcess, 
    LPTHREAD_START_ROUTINE pThreadProc, 
    LPVOID pRemoteBuf
)
{
    HANDLE      hThread = NULL;
    FARPROC     pFunc = NULL;

    if( IsVistaLater() )    // Vista, 7, Server2008
    {
        pFunc = GetProcAddress(GetModuleHandle(L"ntdll.dll"), 
                               "NtCreateThreadEx");
        if( pFunc == NULL )
        {
            _tprintf(L"MyCreateRemoteThread() : "\
                     L"GetProcAddress(\"NtCreateThreadEx\") failed!!! [%d]\n",
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
            _tprintf(L"MyCreateRemoteThread() : NtCreateThreadEx() failed!!! [%d]\n", 
                      GetLastError());
            return FALSE;
        }
    }
    else                    // 2000, XP, Server2003
    {
        hThread = CreateRemoteThread(hProcess, NULL, 0, 
                                     pThreadProc, pRemoteBuf, 0, NULL);
        if( hThread == NULL )
        {
            _tprintf(L"MyCreateRemoteThread() : CreateRemoteThread() failed!!! [%d]\n", 
                     GetLastError());
            return FALSE;
        }
    }

	if( WAIT_FAILED == WaitForSingleObject(hThread, INFINITE) )
    {
        _tprintf(L"MyCreateRemoteThread() : WaitForSingleObject() failed!!! [%d]\n", 
                 GetLastError());
        return FALSE;
    }

    return TRUE;
}

LPCTSTR GetProcName(DWORD dwPID)
{
	HANDLE                  hSnapshot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32          pe;
    BOOL                    bMore = FALSE;

	// Get the snapshot of the system
	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    if( hSnapshot == INVALID_HANDLE_VALUE )
    {
        _tprintf(L"GetProcName() : CreateToolhelp32Snapshot() failed!!! [%d]", 
                  GetLastError());
        return NULL;
    }

	// find process
	bMore = Process32First(hSnapshot, &pe);
	for( ; bMore; bMore = Process32Next(hSnapshot, &pe) )
	{
		if( dwPID == pe.th32ProcessID )
        {
        	CloseHandle(hSnapshot);
            return pe.szExeFile;
        }
	}

	CloseHandle(hSnapshot);

    return NULL;
}

BOOL CheckDllInProcess(DWORD dwPID, LPCTSTR szDllPath)
{
	BOOL                    bMore = FALSE;
	HANDLE                  hSnapshot = INVALID_HANDLE_VALUE;
	MODULEENTRY32           me = { sizeof(me), };

	if( INVALID_HANDLE_VALUE == 
        (hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID)) )
    {
        _tprintf(L"CheckDllInProcess() : CreateToolhelp32Snapshot(%d) failed!!! [%d]\n",
                  dwPID, GetLastError());
        return FALSE;
    }

	bMore = Module32First(hSnapshot, &me);
	for( ; bMore ; bMore = Module32Next(hSnapshot, &me) )
	{
		if( !_tcsicmp(me.szModule, szDllPath) || 
            !_tcsicmp(me.szExePath, szDllPath) )
        {
            CloseHandle(hSnapshot);
			return TRUE;
        }
	}

    CloseHandle(hSnapshot);
    return FALSE;
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
    DWORD                   dwDesiredAccess = 0;
    TCHAR                   szProcName[MAX_PATH] = {0,};

    dwDesiredAccess = PROCESS_ALL_ACCESS;
    //dwDesiredAccess = MAXIMUM_ALLOWED;
	if ( !(hProcess = OpenProcess(dwDesiredAccess, FALSE, dwPID)) )
    {
        _tprintf(L"InjectDll() : OpenProcess(%d) failed!!! [%d]\n", 
                  dwPID, GetLastError());
		goto INJECTDLL_EXIT;
    }

	pRemoteBuf = VirtualAllocEx(hProcess, NULL, dwBufSize, 
                                MEM_COMMIT, PAGE_READWRITE);
    if( pRemoteBuf == NULL )
    {
        _tprintf(L"InjectDll() : VirtualAllocEx() failed!!! [%d]\n", 
                  GetLastError());
        goto INJECTDLL_EXIT;
    }

	if( !WriteProcessMemory(hProcess, pRemoteBuf, 
                           (LPVOID)szDllPath, dwBufSize, NULL) )
    {
        _tprintf(L"InjectDll() : WriteProcessMemory() failed!!! [%d]\n",
                  GetLastError());
        goto INJECTDLL_EXIT;
    }

    hMod = GetModuleHandle(L"kernel32.dll");
    if( hMod == NULL )
    {
        _tprintf(L"InjectDll() : GetModuleHandle(\"kernel32.dll\") failed!!! [%d]\n",
                  GetLastError());
        goto INJECTDLL_EXIT;
    }

	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "LoadLibraryW");
    if( pThreadProc == NULL )
    {
        _tprintf(L"InjectDll() : GetProcAddress(\"LoadLibraryW\") failed!!! [%d]\n", 
                  GetLastError());
        goto INJECTDLL_EXIT;
    }

    if( !MyCreateRemoteThread(hProcess, pThreadProc, pRemoteBuf) )
    {
        _tprintf(L"InjectDll() : MyCreateRemoteThread() failed!!!\n");
        goto INJECTDLL_EXIT;
    }

    bRet = CheckDllInProcess(dwPID, szDllPath);

INJECTDLL_EXIT:

    wsprintf(szProcName, L"%s", GetProcName(dwPID));
    if( szProcName[0] == '\0' )
        _tcscpy_s(szProcName, L"(no_process)");

    _tprintf(L"%s(%d) %s!!! [%d]\n", szProcName, dwPID, bRet ? L"SUCCESS" : L"-->> FAILURE", GetLastError());

    if( pRemoteBuf )
        VirtualFreeEx(hProcess, pRemoteBuf, 0, MEM_RELEASE);

    if( hThread )
	    CloseHandle(hThread);

    if( hProcess )
	    CloseHandle(hProcess);

	return bRet;
}

BOOL EjectDll(DWORD dwPID, LPCTSTR szDllPath)
{
	BOOL                    bMore = FALSE, bFound = FALSE, bRet = FALSE;
	HANDLE                  hSnapshot = INVALID_HANDLE_VALUE;
    HANDLE                  hProcess = NULL;
    HANDLE                  hThread = NULL;
	MODULEENTRY32           me = { sizeof(me), };
	LPTHREAD_START_ROUTINE  pThreadProc = NULL;
    HMODULE                 hMod = NULL;
    DWORD                   dwDesiredAccess = 0;
    TCHAR                   szProcName[MAX_PATH] = {0,};

	if( INVALID_HANDLE_VALUE == 
        (hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwPID)) )
    {
        _tprintf(L"EjectDll() : CreateToolhelp32Snapshot(%d) failed!!! [%d]\n",
                  dwPID, GetLastError());
        goto EJECTDLL_EXIT;
    }

	bMore = Module32First(hSnapshot, &me);
	for( ; bMore ; bMore = Module32Next(hSnapshot, &me) )
	{
		if( !_tcsicmp(me.szModule, szDllPath) || 
            !_tcsicmp(me.szExePath, szDllPath) )
		{
			bFound = TRUE;
			break;
		}
	}

	if( !bFound )
	{
        _tprintf(L"EjectDll() : There is not %s module in process(%d) memory!!!\n", 
                  szDllPath, dwPID);
        goto EJECTDLL_EXIT;
	}

    dwDesiredAccess = PROCESS_ALL_ACCESS;
	if( !(hProcess = OpenProcess(dwDesiredAccess, FALSE, dwPID)) )
	{
		_tprintf(L"EjectDll() : OpenProcess(%d) failed!!! [%d]\n", 
                  dwPID, GetLastError());
		goto EJECTDLL_EXIT;
	}

    hMod = GetModuleHandle(L"kernel32.dll");
    if( hMod == NULL )
    {
        _tprintf(L"EjectDll() : GetModuleHandle(\"kernel32.dll\") failed!!! [%d]\n", 
                  GetLastError());
        goto EJECTDLL_EXIT;
    }

	pThreadProc = (LPTHREAD_START_ROUTINE)GetProcAddress(hMod, "FreeLibrary");
    if( pThreadProc == NULL )
    {
        _tprintf(L"EjectDll() : GetProcAddress(\"FreeLibrary\") failed!!! [%d]\n", 
                  GetLastError());
        goto EJECTDLL_EXIT;
    }

    if( !MyCreateRemoteThread(hProcess, pThreadProc, me.modBaseAddr) )
    {
        _tprintf(L"EjectDll() : MyCreateRemoteThread() failed!!!\n");
        goto EJECTDLL_EXIT;
    }

    bRet = TRUE;

EJECTDLL_EXIT:

    _tcscpy_s(szProcName, GetProcName(dwPID));
    _tprintf(L"%s(%d) %s!!! [%d]\n", szProcName, dwPID, bRet ? L"SUCCESS" : L"-->> FAILURE", GetLastError());

    if( hThread )
        CloseHandle(hThread);

    if( hProcess )
        CloseHandle(hProcess);

    if( hSnapshot != INVALID_HANDLE_VALUE )
        CloseHandle(hSnapshot);

	return bRet;
}

BOOL InjectDllToAll(int nMode, LPCTSTR szDllPath)
{
	DWORD                   dwPID = 0;
	HANDLE                  hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32          pe;
    BOOL                    bMore = FALSE;

	// Get the snapshot of the system
	pe.dwSize = sizeof(PROCESSENTRY32);
	hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
    if( hSnapShot == INVALID_HANDLE_VALUE )
    {
        _tprintf(L"InjectDllToAll() : CreateToolhelp32Snapshot() failed!!! [%d]", 
                  GetLastError());
        return FALSE;
    }

	// find process
	bMore = Process32First(hSnapShot, &pe);
	for( ; bMore; bMore = Process32Next(hSnapShot, &pe) )
	{
		dwPID = pe.th32ProcessID;

        // 예외 프로세스 : [System Process], System, smss.exe, csrss.exe
        if( dwPID < 100 ||
            !_tcsicmp(pe.szExeFile, L"smss.exe") ||
            !_tcsicmp(pe.szExeFile, L"csrss.exe") )
        {
            _tprintf(L"%s(%d) => System Process... DLL %s is impossible!\n", 
                   pe.szExeFile, dwPID, nMode==INJECTION_MODE ? L"Injection" : L"Ejection");
            continue;
        }

        if( nMode == INJECTION_MODE )
		    InjectDll(dwPID, szDllPath);
        else
            EjectDll(dwPID, szDllPath);
	}

	CloseHandle(hSnapShot);

	return TRUE;
}

BOOL InjectDllToOne(LPCTSTR szProc, int nMode, LPCTSTR szDllPath)
{
    int                     i = 0, nLen = (int)_tcslen(szProc);
	DWORD                   dwPID = 0;
	HANDLE                  hSnapShot = INVALID_HANDLE_VALUE;
	PROCESSENTRY32          pe;
    BOOL                    bMore = FALSE;

    // check if ProcName or PID
    for(i = 0; i < nLen; i++)
        if( !_istdigit(szProc[i]) )
            break;

    if( i == nLen )     // PID
    {
        dwPID = (DWORD)_tstol(szProc);
        
        if( nMode == INJECTION_MODE )
            InjectDll(dwPID, szDllPath);
        else
            EjectDll(dwPID, szDllPath);
    }
    else                // ProcName
    {
        // Get the snapshot of the system
	    pe.dwSize = sizeof(PROCESSENTRY32);
	    hSnapShot = CreateToolhelp32Snapshot(TH32CS_SNAPALL, NULL);
        if( hSnapShot == INVALID_HANDLE_VALUE )
        {
            _tprintf(L"InjectDllToOne() : CreateToolhelp32Snapshot() failed!!! [%d]", 
                      GetLastError());
            return FALSE;
        }

	    // find process
	    bMore = Process32First(hSnapShot, &pe);
	    for( ; bMore; bMore = Process32Next(hSnapShot, &pe) )
	    {
		    dwPID = pe.th32ProcessID;

            // 시스템의 안정성을 위해서
            // PID 가 100 보다 작은 시스템 프로세스에 대해서는
            // DLL Injection 을 수행하지 않는다.
		    if( dwPID < 100 )
			    continue;

            if( !_tcsicmp(pe.szExeFile, szProc) )
            {
                if( nMode == INJECTION_MODE )
		            InjectDll(dwPID, szDllPath);
                else
                    EjectDll(dwPID, szDllPath);
            }
	    }

	    CloseHandle(hSnapShot);
    }

    return TRUE;
}

BOOL Initialize(LPCTSTR szOption, LPCTSTR szDllPath)
{
    // check Option (Injection/Ejection)
    if( _tcsicmp(szOption, L"-i") &&
        _tcsicmp(szOption, L"-e") )
        return FALSE;

    // check DLL Path
	if( _taccess(szDllPath, 0) == -1 )
        return FALSE;

    return TRUE;
}

int _tmain(int argc, TCHAR *argv[])
{
    #define BUFSIZE         (1024)
    int     nMode           = INJECTION_MODE;
    TCHAR   szPath[BUFSIZE]   = L""; 
    
	if( (argc != 4) || 
        ( _tcsicmp(argv[2], L"-i") && _tcsicmp(argv[2], L"-e")) )
	{
	    _tprintf(L"\n %s (Ver 1.1.1) - Dll Injection/Ejection Utility!!!\n"\
                 L"   www.reversecore.com\n"\
                 L"   reversecore@gmail.com\n"\
                 L"\n USAGE  : %s <procname|pid|*> <-i|-e> <dll path>\n\n", 
                 argv[0], argv[0]);
		return 1;
	}
    
    if( !GetFullPathName(argv[3], BUFSIZE, szPath, NULL) )    
    {
        _tprintf(L"GetFullPathName() failed! [%d]", GetLastError());
        return 1;
    }

    // check DLL Path
	if( _taccess(szPath, 0) == -1 )
    {
        _tprintf(L"There is no \"%s\" file!\n", szPath);
        return FALSE;
    }

	// change privilege
	if( !SetPrivilege(SE_DEBUG_NAME, TRUE) )
        return 1;

    // Mode (Injection/Ejection)
    if( !_tcsicmp(argv[2], L"-e") )
        nMode = EJECTION_MODE;

    // Inject Dll
    if( !_tcsicmp(argv[1], L"*") )
        InjectDllToAll(nMode, szPath);
    else
        InjectDllToOne(argv[1], nMode, szPath);

	return 0;
}
