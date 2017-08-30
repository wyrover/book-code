#include <cactus/cactus_process.h>
#include <cactus/cactus_string.h>
#include <TlHelp32.h>
#include <Psapi.h>
#pragma comment(lib, "Psapi.lib")

#include <Userenv.h>
#pragma comment(lib, "Userenv.lib")

#include <set>



namespace cactus
{

#define ProcessBasicInformation 0

typedef struct _PROCESS_BASIC_INFORMATION {
    DWORD ExitStatus;
    DWORD PebBaseAddress;
    DWORD AffinityMask;
    DWORD BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
}   PROCESS_BASIC_INFORMATION;


// ntdll!NtQueryInformationProcess (NT specific!)
//
// The function copies the process information of the
// specified type into a buffer
//
// NTSYSAPI
// NTSTATUS
// NTAPI
// NtQueryInformationProcess(
//    IN HANDLE ProcessHandle,              // handle to process
//    IN PROCESSINFOCLASS InformationClass, // information type
//    OUT PVOID ProcessInformation,         // pointer to buffer
//    IN ULONG ProcessInformationLength,    // buffer size in bytes
//    OUT PULONG ReturnLength OPTIONAL      // pointer to a 32-bit
//                                          // variable that receives
//                                          // the number of bytes
//                                          // written to the buffer
// );

typedef LONG(WINAPI *PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);
typedef LONG(WINAPI *PFN_GetModuleFileNameEx)(HANDLE, HMODULE, LPTSTR, DWORD);

PROCNTQSIP NtQueryInformationProcess;

BOOL LoadNtdllAndPro()
{
    NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(
                                    GetModuleHandle(L"ntdll"), "NtQueryInformationProcess");
    return (NtQueryInformationProcess != NULL);
}

DWORD PIDConversionHandles(VEC_DORD& vecPid, HANDLE hChild[1024])
{
    for (DWORD i = 0; i < vecPid.size(); i++) {
        hChild[i] = OpenProcess(
                        PROCESS_ALL_ACCESS, FALSE, vecPid[i]);
    }

    return TRUE;
}

BOOL EnablePrivilege(LPCTSTR szPrivilege)
{
    HANDLE hToken;
    LUID sedebugnameValue;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken)) {
        return FALSE;
    }

    if (!LookupPrivilegeValue(NULL, szPrivilege, &sedebugnameValue)) {
        CloseHandle(hToken);
        return FALSE;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = sedebugnameValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    if (!AdjustTokenPrivileges(hToken, FALSE, &tkp, sizeof tkp, NULL, NULL)) {
        CloseHandle(hToken);
        return FALSE;
    }

    return TRUE;
}

#ifndef _WIN64
//------------------------------------------------------------------------------
// Returns TRUE if we are running on a 64-bit OS in WoW, FALSE otherwise.
BOOL RunningInWow64()
{
    static int s_Wow64Process;

    if (s_Wow64Process == 0) {
        BOOL fWow64Process = FALSE;

        if (!IsWow64Process(GetCurrentProcess(), &fWow64Process))
            fWow64Process = FALSE;

        s_Wow64Process = fWow64Process ? 1 : -1;
    }

    return (s_Wow64Process == 1) ? TRUE : FALSE;
}
#endif

BOOL IsWow64()
{
    typedef BOOL (WINAPI * LPFN_ISWOW64PROCESS)(HANDLE hProcess, PBOOL Wow64Process);
    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)
                                           GetProcAddress(GetModuleHandle(L"kernel32"), "IsWow64Process");
    BOOL bIsWow64 = FALSE;

    if (NULL != fnIsWow64Process) {
        if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
        }
    }

    return bIsWow64;
}

#ifndef _WIN64
// Load function dynamically to allow the program to launch on Win2k/XPSP1:
typedef BOOL (WINAPI *PFN_IsWow64Process)(HANDLE, PBOOL);
static PFN_IsWow64Process _IsWow64Process = (PFN_IsWow64Process)GetProcAddress(GetModuleHandle(_T("kernel32"))
        , "IsWow64Process");
#endif

BOOL IsProcess64Bit(HANDLE aHandle)
{
    BOOL is32on64;
#ifdef _WIN64

    // No need to load the function dynamically in this case since it should exist on all OSes
    // which are able to load 64-bit executables (such as ours):
    if (IsWow64Process(aHandle, &is32on64))
        return !is32on64; // 64-bit if not running under WOW64.

    // Since above didn't return, an error occurred.  MSDN isn't clear about what conditions can
    // cause this, so for simplicity just assume the target process is 64-bit (like this one).
    return TRUE;
#else

    if (_IsWow64Process && _IsWow64Process(GetCurrentProcess(), &is32on64)) {
        if (is32on64) {
            // We're running under WOW64.  Since WOW64 only exists on 64-bit systems and on such systems
            // 32-bit processes can run ONLY under WOW64, if the target process is also running under
            // WOW64 it must be 32-bit; otherwise it must be 64-bit.
            if (_IsWow64Process(aHandle, &is32on64))
                return !is32on64;
        }
    }

    // Since above didn't return, one of the following is true:
    //  a) IsWow64Process doesn't exist, so the OS and all running processes must be 32-bit.
    //  b) IsWow64Process failed on the first or second call.  MSDN isn't clear about what conditions
    //     can cause this, so for simplicity just assume the target process is 32-bit (like this one).
    //  c) The current process is not running under WOW64.  Since we know it is 32-bit (due to our use
    //     of conditional compilation), the OS and all running processes must be 32-bit.
    return FALSE;
#endif
}

BOOL IsOS64Bit()
{
#ifdef _WIN64
    // OS must be 64-bit to run this program.
    return TRUE;
#else
    // If OS is 64-bit, this program must be running in WOW64.
    BOOL is32on64;

    if (_IsWow64Process && _IsWow64Process(GetCurrentProcess(), &is32on64))
        return is32on64;

    return FALSE;
#endif
}



DWORD GetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPTSTR ptszFileName, DWORD nSize)
{
    typedef DWORD (WINAPI * FNGMFNEX)(HANDLE, HMODULE, LPTSTR, DWORD);
    HMODULE hDll = LoadLibrary(_T("Psapi.dll"));

    if (hDll == NULL)
        return 0;

#if defined (UNICODE) || defined (_UNICODE)
    FNGMFNEX pfn = (FNGMFNEX) GetProcAddress(hDll, "GetModuleFileNameExW");
#else
    FNGMFNEX pfn = (FNGMFNEX) GetProcAddress(hDll, "GetModuleFileNameExA");
#endif
    DWORD dwRet = 0;

    if (pfn)
        dwRet = pfn(hProcess, hModule, ptszFileName, nSize);

    FreeLibrary(hDll);
    return dwRet;
}

bool GetProcessCommandLine(DWORD dwPID, LPTSTR ptszCmdLine, UINT *puSize)
{
    HANDLE hProcess = OpenProcess(PROCESS_VM_READ, FALSE, dwPID);

    if (hProcess == NULL)
        return false;

    HMODULE hKernel32 = GetModuleHandle(_T("kernel32.dll"));
#if defined (UNICODE) || defined (_UNICODE)
    FARPROC pfnGCL = GetProcAddress(hKernel32, "GetCommandLineW");
#else
    FARPROC pfnGCL = GetProcAddress(hKernel32, "GetCommandLineA");
#endif
    BYTE abTmp [6];
    SIZE_T nRead = 0;
    ReadProcessMemory(hProcess, pfnGCL, abTmp, 6, &nRead);

    if (nRead != 6) {
        CloseHandle(hProcess);
        return false;
    }

    LPTSTR ptszProcessCmdLine = (LPTSTR)(DWORD_PTR)(* (DWORD_PTR*)(abTmp + 1));
    ReadProcessMemory(hProcess, ptszProcessCmdLine, abTmp, 4, &nRead);
    ptszProcessCmdLine = (LPTSTR)(DWORD_PTR)(* (DWORD_PTR*)(abTmp));
    UINT nSize = LOBYTE(LOWORD(::GetVersion())) <= 5 ? 8191 : 2047;
    LPTSTR ptsz = (LPTSTR)alloca(nSize + 1);
    *ptsz = 0;
    ReadProcessMemory(hProcess, ptszProcessCmdLine, ptsz, nSize, NULL);
    CloseHandle(hProcess);

    if (*puSize <= _tcslen(ptsz)) {
        *puSize = _tcslen(ptsz);
        return false;
    }

    _tcscpy(ptszCmdLine, ptsz);
    return *ptsz != 0;
}

BOOL EnumProcessesCurrent(DWORD aProcesses[1024], DWORD& cProcesses)
{
    DWORD cbNeeded = 0;

    if (!EnumProcesses(aProcesses, cProcesses, &cbNeeded))
        return FALSE;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    return TRUE;
}

DWORD GetChildProcessByPID(DWORD dwPid, VEC_DORD& vecChildPID)
{
    DWORD dwCount = 0;
    DWORD aProcesses[1024];
    DWORD cSumCount = 1024;
    EnablePrivilege(L"SeDebugPrivilege");

    if (EnumProcessesCurrent(aProcesses, cSumCount)) {
        LoadNtdllAndPro();
        DWORD dwPIDTmp = 0;

        for (DWORD i = 0; i < cSumCount; i++) {
            dwPIDTmp  = GetParentProcessID(aProcesses[i]);

            if (dwPIDTmp == dwPid) {
                vecChildPID.push_back(aProcesses[i]);
            }
        }
    }

    dwCount = vecChildPID.size();
    return dwCount;
}

DWORD WaitProcExitByPids(VEC_DORD& vecChildPID, LONG dwTimeOut)
{
    DWORD dwWait = 0;
    DWORD dwChildExitCode = 0;
    HANDLE hChild[1024] = {NULL};
    PIDConversionHandles(vecChildPID, hChild);
    dwWait = WaitForMultipleObjects(
                 vecChildPID.size(), hChild, TRUE, dwTimeOut);

    switch (dwWait) {
    case WAIT_ABANDONED_0:
    case WAIT_TIMEOUT:
        break;

    default: {
        if (dwWait < 1024)
            GetExitCodeProcess(hChild[dwWait], &dwChildExitCode);
    }
    }

    return dwChildExitCode;
}

BOOL GetFirstExitCodeByPids(VEC_DORD& vecChildPID, LONG dwTimeOut, DWORD& dwExitCode)
{
    BOOL    bRet            = FALSE;
    DWORD   dwWait          = 0;
    DWORD   dwChildExitCode = 0;
    HANDLE  hChild[1024]    = {NULL};
    PIDConversionHandles(vecChildPID, hChild);
    dwWait = WaitForMultipleObjects(
                 vecChildPID.size(), hChild, TRUE, dwTimeOut);

    switch (dwWait) {
    case WAIT_ABANDONED_0:
    case WAIT_TIMEOUT:
        break;

    default: {
        if (dwWait < 1024) {
            if (GetExitCodeProcess(hChild[dwWait], &dwChildExitCode)) {
                bRet        = TRUE;
                dwExitCode  = dwChildExitCode;
            }
        }
    }
    }

    return bRet;
}

DWORD KGetPidByHandle(HANDLE hProcess)
{
    if (!NtQueryInformationProcess) {
        LoadNtdllAndPro();
    }

    // Retrieve information
    LONG status = 0;
    PROCESS_BASIC_INFORMATION pbi = {0};
    status = NtQueryInformationProcess(hProcess,
                                       ProcessBasicInformation,
                                       (PVOID)&pbi,
                                       sizeof(PROCESS_BASIC_INFORMATION),
                                       NULL);

    // Copy parent Id on success
    if (!status)
        return pbi.UniqueProcessId;

    return -1;
}

DWORD GetParentProcessID(DWORD dwId)
{
    LONG                      status;
    DWORD                     dwParentPID = (DWORD) - 1;
    HANDLE                    hProcess;
    PROCESS_BASIC_INFORMATION pbi = {0};

    if (!NtQueryInformationProcess) {
        return -1;
    }

    // Get process handle
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwId);

    if (!hProcess)
        return (DWORD) - 1;

    // Retrieve information
    status = NtQueryInformationProcess(hProcess,
                                       ProcessBasicInformation,
                                       (PVOID)&pbi,
                                       sizeof(PROCESS_BASIC_INFORMATION),
                                       NULL);

    // Copy parent Id on success
    if (!status)
        dwParentPID = pbi.InheritedFromUniqueProcessId;

    CloseHandle(hProcess);
    return dwParentPID;
}



BOOL GetParentPid(HANDLE hCurrentProcess, DWORD& dwParentPid)
{
    typedef LONG(WINAPI * PFN_NtQueryInformationProcess)(HANDLE, UINT, PVOID, ULONG, PULONG);
    BOOL bRet = FALSE;
    HMODULE hNtdll = GetModuleHandleW(L"ntdll");

    if (hNtdll) {
        PROCESS_BASIC_INFORMATION pbi = {0};
        PFN_NtQueryInformationProcess pfnNtQueryInformationProcess =
            (PFN_NtQueryInformationProcess)GetProcAddress(hNtdll, "NtQueryInformationProcess");
        LONG lStatus = -1;

        if (pfnNtQueryInformationProcess) {
            lStatus = pfnNtQueryInformationProcess(
                          hCurrentProcess,
                          ProcessBasicInformation,
                          (void*)&pbi,
                          sizeof(PROCESS_BASIC_INFORMATION),
                          NULL
                      );
        }

        if (lStatus >= 0) {
            dwParentPid = pbi.InheritedFromUniqueProcessId;
            bRet = TRUE;
        }

        FreeLibrary(hNtdll);
    }

    return bRet;
}

BOOL GetProcessFullPathByPid(DWORD dwPid, string16& exe_filename, DWORD& dwSize)
{
    BOOL bRet = FALSE;
    HANDLE hProcess = NULL;
    HMODULE hModule = NULL;     

    do 
    {
        hProcess = ::OpenProcess(
            PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
            FALSE,
            dwPid
            );

        if (!hProcess) {
            OutputDebugString(TEXT("open process failed."));
            break;      
        }
        

        HMODULE hModule = LoadLibraryW(L"Psapi.dll");

        if (!hModule) {
            OutputDebugString(TEXT("load Psapi.dll failed."));
            break;
        }
        
        PFN_GetModuleFileNameEx pfnGetModuleFileNameEx =
            (PFN_GetModuleFileNameEx)GetProcAddress(hModule, "GetModuleFileNameExW");
        char16 buffer[MAX_PATH + 1] = {0};

        if (!pfnGetModuleFileNameEx) {
            OutputDebugString(TEXT("GetModuleFileNameExW not found."));
            break;
        }   
      
        dwSize = GetModuleFileNameEx(
            hProcess,
            NULL,
            buffer,
            MAX_PATH
            );
        

        bRet = (dwSize > 0);
        exe_filename = string16(buffer);               
        

    } while (0);


    if (hProcess) {
        ::CloseHandle(hProcess);
    }

    if (hModule) {
        ::FreeLibrary(hModule);     
    }

    return bRet;
}


BOOL _IsVistaOrLater()
{
    OSVERSIONINFOEX vi = { sizeof(OSVERSIONINFOEX) };
    ::GetVersionEx((LPOSVERSIONINFO)&vi);

    if (vi.dwMajorVersion >= 6)
        return TRUE;
    else
        return FALSE;
}


CString GetProcessFileName(DWORD dwProcID)
{
    CString strResult;
    HANDLE hProcess = NULL;

    if (_IsVistaOrLater())
    {
        hProcess = ::OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, dwProcID);  
    }
    else
    {
        hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwProcID);  
    }

    if(hProcess != NULL)  
    {
        if (_IsVistaOrLater())
        {
            typedef DWORD (WINAPI *LPFN_QUERYFULLPROCESSIMAGENAME)( HANDLE hProcess,DWORD dwFlags,LPTSTR lpExeName,PDWORD lpdwSize);

            HMODULE hKernel32 = NULL;
            LPFN_QUERYFULLPROCESSIMAGENAME _QueryFullProcessImageName = NULL;

            if (hKernel32 = ::LoadLibrary(_T("kernel32.dll"))) 
            {
                _QueryFullProcessImageName = (LPFN_QUERYFULLPROCESSIMAGENAME)::GetProcAddress(hKernel32, "QueryFullProcessImageNameW");

                if (_QueryFullProcessImageName)
                {
                    DWORD size = MAX_PATH;
                    _QueryFullProcessImageName(hProcess, 0, strResult.GetBuffer(MAX_PATH), &size);
                    strResult.ReleaseBuffer();
                }

                FreeLibrary(hKernel32);
            } 
        } 
        else 
        {
            TCHAR szFileDosPath[MAX_PATH];
            ZeroMemory(szFileDosPath, sizeof(TCHAR)*(MAX_PATH));
            if (GetProcessImageFileName(hProcess, szFileDosPath, MAX_PATH) >= 0 )
            {
                // 获取Logic Drive String长度
                UINT uiLen = GetLogicalDriveStrings(0, NULL);
                if (0 == uiLen)
                {
                    goto Exit0;
                }

                PTSTR pLogicDriveString = new TCHAR[uiLen + 1];
                ZeroMemory(pLogicDriveString, uiLen + 1);
                uiLen = GetLogicalDriveStrings(uiLen, pLogicDriveString);
                if (0 == uiLen)
                {
                    delete[] pLogicDriveString;
                    goto Exit0;
                }

                TCHAR szDrive[3] = TEXT(" :");
                PTSTR pDosDriveName = new TCHAR[MAX_PATH];
                PTSTR pLogicIndex = pLogicDriveString;

                do
                {
                    szDrive[0] = *pLogicIndex;
                    uiLen = QueryDosDevice(szDrive, pDosDriveName, MAX_PATH);
                    if (0 == uiLen)
                    {
                        if (ERROR_INSUFFICIENT_BUFFER != GetLastError())
                        {
                            break;
                        }

                        delete[]pDosDriveName;
                        pDosDriveName = new TCHAR[uiLen + 1];
                        uiLen = QueryDosDevice(szDrive, pDosDriveName, uiLen + 1);
                        if (0 == uiLen)
                        {
                            break;
                        }
                    }

                    uiLen = _tcslen(pDosDriveName);
                    if (0 == _tcsnicmp(szFileDosPath, pDosDriveName, uiLen))
                    {
                        strResult.Format(_T("%s%s"), szDrive, szFileDosPath + uiLen);
                        break;
                    }

                    while (*pLogicIndex++);
                } while (*pLogicIndex);

                delete[]pLogicDriveString;
                delete[]pDosDriveName;
            }
        } 

        //else
        //{
        //    if(::GetModuleFileNameEx(hProcess, 0, strResult.GetBuffer(MAX_PATH), MAX_PATH) > 0)
        //    {
        //        strResult.ReleaseBuffer();
        //        strResult = CmDirectoryHelper::ExtractFilePath(strResult);
        //    }
        //} 
Exit0:
        ::CloseHandle(hProcess);
    }  

    return strResult;
}



string16 GetProcessNameByPID(DWORD processid)
{
    char16 szProcessName[MAX_PATH];
    // Get a handle to the process.
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION |
                                  PROCESS_VM_READ,
                                  FALSE, processid);

    // Get the process name.

    if (NULL != hProcess) {
        HMODULE hMod;
        DWORD cbNeeded;
        KPsapiDllFunction objPsapi;

        if (objPsapi.EnumProcessModules(hProcess, &hMod, sizeof(hMod), &cbNeeded)) {
            objPsapi.GetModule_BaseName(hProcess, hMod, szProcessName, _countof(szProcessName));
        }

        // _tprintf( TEXT(_T("%s  (PID: %u)\n")), szProcessName, processID );
        CloseHandle(hProcess);
        return string16(szProcessName);
    } else {
        return StringUtil::sWEmpty;
    }
}

BOOL GetProcessPidByFullPath(const string16& exe_filename, DWORD& dwPid)
{
    BOOL bFindProcess = FALSE;
    PROCESSENTRY32 ProcEntry = { 0 };
    ProcEntry.dwSize = sizeof(PROCESSENTRY32);
    HANDLE hSnapProcess = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (hSnapProcess == INVALID_HANDLE_VALUE) {
        return FALSE;
    }

    if (!::Process32First(hSnapProcess, &ProcEntry)) {
        return FALSE;
    }

    HMODULE hLibModule = ::LoadLibraryW(L"Psapi.dll");

    if (hLibModule == NULL) {
        ::CloseHandle(hSnapProcess);
        return FALSE;
    }

    PFN_GetModuleFileNameEx pfnGetModuleFileNameEx  =
        (PFN_GetModuleFileNameEx)::GetProcAddress(hLibModule, "GetModuleFileNameExW");

    if (pfnGetModuleFileNameEx == NULL) {
        ::CloseHandle(hLibModule);
        ::CloseHandle(hSnapProcess);
        return FALSE;
    }

    wchar_t szFullPath[MAX_PATH] = {0};
    DWORD dwSize = MAX_PATH;

    do {
        HANDLE hProcess = ::OpenProcess(
                              PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
                              FALSE,
                              ProcEntry.th32ProcessID
                          );

        if (hProcess != INVALID_HANDLE_VALUE) {
            szFullPath[0] = '\0';
            dwSize = MAX_PATH;
            dwSize = ::GetModuleFileNameEx(
                         hProcess,
                         NULL,
                         szFullPath,
                         dwSize
                     );

            if (dwSize > 0) {
                if (_wcsicmp(szFullPath, exe_filename.c_str()) == 0) {
                    bFindProcess = TRUE;
                    dwPid = ProcEntry.th32ProcessID;
                }
            }

            ::CloseHandle(hProcess);
        }

        if (bFindProcess) {
            break;
        }
    } while (::Process32Next(hSnapProcess, &ProcEntry));

    ::CloseHandle(hSnapProcess);
    ::FreeLibrary(hLibModule);
    return bFindProcess;
}



BOOL IsProcessExist(const string16& process_name)
{
    BOOL bRet           = FALSE;
    HANDLE hProcessSnap     = NULL;
    PROCESSENTRY32 pe32     = {0};
    HANDLE hProcess         = INVALID_HANDLE_VALUE;
    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
        goto Exit0;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (::Process32First(hProcessSnap, &pe32)) {
        do {
            if (_wcsicmp((LPCWSTR)pe32.szExeFile, process_name.c_str()) == 0) {
                bRet = TRUE;
                break;
            }
        } while (::Process32Next(hProcessSnap, &pe32));
    }

Exit0:

    if (hProcessSnap != INVALID_HANDLE_VALUE)
        ::CloseHandle(hProcessSnap);

    return bRet;
}

BOOL HasProcessExist(const string16& process_name)
{
    BOOL bRet = FALSE;
    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    KPsapiDllFunction objPsapi;

    if (!objPsapi.EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return bRet;

    // Calculate how many process identifiers were returned.
    cProcesses = cbNeeded / sizeof(DWORD);
    // Print the name and process identifier for each process.
    string16 strProcess;

    for (i = 0; i < cProcesses; i++) {
        strProcess = GetProcessNameByPID(aProcesses[i]);

        if (wcsistr(strProcess.c_str(), process_name.c_str()) != NULL) {
            bRet = TRUE;
            break;
        }
    }

    return bRet;
}

BOOL GetProcessesByName(const string16& process_name, std::vector<DWORD>& list)
{
    BOOL bResult            = FALSE;
    HANDLE hProcessSnap     = NULL;
    PROCESSENTRY32 pe32     = {0};
    HANDLE hProcess         = INVALID_HANDLE_VALUE;
    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE)
        goto Exit0;

    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (::Process32First(hProcessSnap, &pe32)) {
        LPCWSTR lpstrFileName;

        do {
            lpstrFileName = pe32.szExeFile;

            if (_wcsicmp(lpstrFileName, process_name.c_str()) == 0) {
                bResult = TRUE;
                list.push_back(pe32.th32ProcessID);
            }
        } while (::Process32Next(hProcessSnap, &pe32));
    }

Exit0:

    if (hProcessSnap != INVALID_HANDLE_VALUE)
        ::CloseHandle(hProcessSnap);

    return bResult;
}

BOOL IsProcessExistByFullPath(const string16& exe_filename)
{
    DWORD dwPid = 0;
    BOOL bRetCode = GetProcessPidByFullPath(exe_filename, dwPid);
    return bRetCode;
}

BOOL KillProcess(const wchar_t* lpClassName, const wchar_t* lpWindowName)
{
    HWND    hProcWnd;
    HANDLE  hProcHandle;
    DWORD   dwProcId;
    BOOL    bRet;
    hProcWnd = ::FindWindow(lpClassName, lpWindowName);

    if (hProcWnd == NULL) {
        return FALSE;
    }

    ::GetWindowThreadProcessId(hProcWnd, &dwProcId);
    hProcHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwProcId);

    if (hProcHandle == NULL) {
        return FALSE;
    }

    bRet = ::TerminateProcess(hProcHandle, 0);
    CloseHandle(hProcHandle);
    return bRet;
}

BOOL KillProcessByHwnd(HWND hWnd)
{
    ULONG nProcessID = 0;
    ::GetWindowThreadProcessId(hWnd, &nProcessID);
    HANDLE hProcessHandle = ::OpenProcess(PROCESS_TERMINATE, FALSE, nProcessID);
    return ::TerminateProcess(hProcessHandle, 0);
}

BOOL CALLBACK EnumWinHandle(HWND hWnd, LPARAM lParam)
{
    // Declare variables
    DWORD dwProcessID;
    // Check if this is the right window
    GetWindowThreadProcessId(hWnd, &dwProcessID);

    // Is it the same process?
    if (dwProcessID == lParam) {
        // Try to close application by sending it a message
        ::PostMessage(hWnd, WM_CLOSE, 0, 0);
    }

    // Keep enumerating
    return TRUE;
}

BOOL KillProcessByName(const string16& appname, BOOL bForce /*= TRUE*/)
{
    if (!HasProcessExist(appname))
        return TRUE;

    DWORD aProcesses[1024], cbNeeded, cProcesses;
    unsigned int i;
    KPsapiDllFunction objPsapi;

    if (!objPsapi.EnumProcesses(aProcesses, sizeof(aProcesses), &cbNeeded))
        return FALSE;

    cProcesses = cbNeeded / sizeof(DWORD);
    string16 strProcess;

    for (i = 0; i < cProcesses; i++) {
        strProcess = GetProcessNameByPID(aProcesses[i]);

        if (wcsistr(strProcess.c_str(), appname.c_str()) != NULL) {
            if (bForce) {
                HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, aProcesses[i]);
                // Terminate process
                DWORD exCode;
                GetExitCodeProcess(hProcess, &exCode);
                TerminateProcess(hProcess, exCode);
            } else {
                EnumWindows(EnumWinHandle, aProcesses[i]);
            }
        }
    }

    return TRUE;
}


BOOL KiilProcessByNameAllWin(const string16& appname, BOOL bForce /*= TRUE*/)
{
    // Declare variables
    HANDLE hProcessSnap = NULL;
    PROCESSENTRY32 pe32 = {NULL};
    char16 szCompare[_MAX_PATH];

    // Is app running?
    if (!IsProcessExist(appname))
        return TRUE;

    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    pe32.dwSize = sizeof(PROCESSENTRY32);

    if (Process32First(hProcessSnap, &pe32)) {
        do {
            // Compare processes
            if (wcsistr((LPCWSTR)pe32.szExeFile, appname.c_str()) != NULL) {
                // Should we force the application to close?
                if (bForce) {
                    HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
                    // Terminate process
                    DWORD exCode;
                    GetExitCodeProcess(hProcess, &exCode);
                    TerminateProcess(hProcess, exCode);
                } else {
                    // Send message to the right window
                    EnumWindows(EnumWinHandle,  pe32.th32ProcessID);
                }
            }
        } while (Process32Next(hProcessSnap, &pe32));
    }

    // Application is not running
    return TRUE;
}


BOOL KillProcessByPid(DWORD dwPid)
{
    HANDLE hProcess = ::OpenProcess(PROCESS_TERMINATE, FALSE, dwPid);

    if (NULL != hProcess) {
        ::TerminateProcess(hProcess, 0);
        ::CloseHandle(hProcess);
        hProcess = NULL;
        return TRUE;
    }

    return FALSE;
}

BOOL KillProcessByPid(ULONG ulProcessId, LPCTSTR lpProcessPath)
{
    BOOL bResult = FALSE;
    HANDLE hProcess = NULL;
    TCHAR szModuleFilePath[MAX_PATH] = {0};
    TCHAR szNormalProcessPath[MAX_PATH] = {0};

    do {
        if (8 >= ulProcessId || !lpProcessPath || 1 > _tcslen(lpProcessPath))
            break;

        hProcess = ::OpenProcess(
                       PROCESS_TERMINATE | PROCESS_VM_READ | PROCESS_QUERY_INFORMATION,
                       FALSE,
                       ulProcessId);

        if (!hProcess)
            break;

        if (!::GetModuleFileNameEx(hProcess, NULL, szModuleFilePath, MAX_PATH - 1))
            break;

        if (::GetLongPathName(szModuleFilePath, szNormalProcessPath, MAX_PATH - 1)) {
            if (0 == _tcsicmp(lpProcessPath, szNormalProcessPath)) {
                ULONG ulProcessExitCode = 0;
                ::GetExitCodeProcess(hProcess, &ulProcessExitCode);
                ::TerminateProcess(hProcess, ulProcessExitCode);
                bResult = TRUE;
            }
        }
    } while (0);

    if (hProcess) {
        ::CloseHandle(hProcess);
        hProcess = NULL;
    }

    return bResult;
}

BOOL KillProcessByPath(const string16& exe_filename, int* pnProcessFound)
{
    if (NULL != pnProcessFound) {
        *pnProcessFound = 0;
    }

    std::vector<int>    vecPID;
    KSysProcess process;

    if (!process.GetAllProcess(vecPID)) {
        if (NULL != pnProcessFound) {
            *pnProcessFound = 0;
        }

        return FALSE;
    }

    int nResult = 0;
    std::vector<int>::iterator iterVec;

    for (iterVec = vecPID.begin(); iterVec != vecPID.end(); ++iterVec) {
        CString strProcessPath = process.GetProcessPath(*iterVec);

        if (0 == strProcessPath.CompareNoCase(exe_filename.c_str())) {
            if (KillProcessByPid(*iterVec)) {
                ++nResult;
            }
        }
    }

    if (NULL != pnProcessFound) {
        *pnProcessFound = nResult;
    }

    return ((0 == nResult) ? FALSE : TRUE);
}


BOOL KillProcessByFullPath(const string16& exe_filename)
{
    BOOL bRetCode = FALSE;
    DWORD dwPid = 0;

    if (GetProcessPidByFullPath(exe_filename, dwPid)) {
        bRetCode = KillProcessByPid(dwPid);
    }

    return bRetCode;
}


BOOL GetCurrentUserSID(string16& strCurrentUserSID)
{
    BOOL bResult = FALSE;
    LPTSTR lpUserSID = NULL;
    HANDLE hSnapshot = NULL;
    HANDLE hProcess = NULL, hProcessToken = NULL;
    PTOKEN_USER pTokenUser = NULL;
    DWORD dwPid = 0;
    DWORD dwReturnLength = 0;
    PROCESSENTRY32 structProEntry = {0};
    hSnapshot = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (!hSnapshot) {
        goto _abort;
    }

    structProEntry.dwSize = sizeof(structProEntry);

    if (::Process32First(hSnapshot, &structProEntry)) {
        do {
            if (0 == _wcsicmp(L"explorer.exe", structProEntry.szExeFile)) {
                dwPid = structProEntry.th32ProcessID;
                break;
            }
        } while (::Process32Next(hSnapshot, &structProEntry));
    }

    if (!dwPid) {
        goto _abort;
    }

    hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, dwPid);

    if (!hProcess) {
        goto _abort;
    }

    if (!::OpenProcessToken(hProcess, TOKEN_QUERY, &hProcessToken)) {
        goto _abort;
    }

    bResult = ::GetTokenInformation(hProcessToken, TokenUser, NULL, 0, &dwReturnLength);

    if (ERROR_INSUFFICIENT_BUFFER != GetLastError()) {
        goto _abort;
    }

    pTokenUser = (PTOKEN_USER)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, dwReturnLength);

    if (NULL == pTokenUser) {
        goto _abort;
    }

    bResult = ::GetTokenInformation(
                  hProcessToken,
                  TokenUser,
                  pTokenUser,
                  dwReturnLength,
                  &dwReturnLength);

    if (!bResult) {
        goto _abort;
    }

    typedef BOOL (__stdcall * PConvertSidToStringSidW)(PSID Sid, LPTSTR * StringSid);

    if (IsValidSid(pTokenUser->User.Sid)) {
        PConvertSidToStringSidW pfn_ConvertSidToStringSid = NULL;
        pfn_ConvertSidToStringSid = (PConvertSidToStringSidW)::GetProcAddress(
                                        ::LoadLibrary(L"Advapi32.dll"),
                                        "ConvertSidToStringSidW");

        if (!pfn_ConvertSidToStringSid) {
            goto _abort;
        }

        bResult = pfn_ConvertSidToStringSid(pTokenUser->User.Sid, &lpUserSID);
        pfn_ConvertSidToStringSid = NULL;

        if (bResult && lpUserSID) {
            strCurrentUserSID = lpUserSID;
            ::LocalFree(lpUserSID);
            lpUserSID = NULL;
        }
    }

_abort:

    if (hSnapshot) {
        ::CloseHandle(hSnapshot);
        hSnapshot = NULL;
    }

    if (hProcessToken) {
        ::CloseHandle(hProcessToken);
        hProcessToken = NULL;
    }

    if (hProcess) {
        ::CloseHandle(hProcess);
        hProcess = NULL;
    }

    if (NULL != pTokenUser) {
        HeapFree(GetProcessHeap(), 0, pTokenUser);
        pTokenUser = NULL;
    }

    return bResult;
}



int KWinExecW(const wchar_t* pszCmdLine,
              const wchar_t* pszCurDir,
              DWORD dwCmdShow,
              DWORD dwWaitTime,
              DWORD* pdwGetExitCodeErr,
              DWORD* pdwExitCode)
{
    if (NULL == pszCmdLine)
        return -1;

    if (!((pdwGetExitCodeErr && pdwExitCode) || (!pdwGetExitCodeErr && !pdwExitCode)))
        return -1;

    DWORD               dwRetCode   = 0;
    DWORD               dwCreateFlag = 0;
    PROCESS_INFORMATION ProcessInfo = { 0 };
    STARTUPINFO         StartupInfo = {0};
    StartupInfo.cb          = sizeof(STARTUPINFO);
    StartupInfo.dwFlags     = STARTF_USESHOWWINDOW;
    StartupInfo.wShowWindow = static_cast<WORD>(dwCmdShow);
    wchar_t* szExecCmd = NULL;
    size_t size = wcslen(pszCmdLine);
    szExecCmd = new wchar_t[size + 1];
    wcsncpy(szExecCmd, pszCmdLine, size + 1);

    if (SW_HIDE == dwCmdShow)
        dwCreateFlag = CREATE_NO_WINDOW;

    BOOL bRetCode = FALSE;
    bRetCode = ::CreateProcessW(
                   NULL,
                   szExecCmd,
                   NULL,
                   NULL,
                   FALSE,
                   dwCreateFlag,
                   NULL,
                   pszCurDir,
                   &StartupInfo,
                   &ProcessInfo
               );

    if (!bRetCode) {
        dwRetCode = ::GetLastError();
    } else {
        dwRetCode = 0;

        if (dwWaitTime)
            ::WaitForSingleObject(ProcessInfo.hProcess, dwWaitTime);

        if (pdwGetExitCodeErr && pdwExitCode) {
            if (::GetExitCodeProcess(ProcessInfo.hProcess, pdwExitCode))
                *pdwGetExitCodeErr = 0;
            else
                *pdwGetExitCodeErr = ::GetLastError();
        }

        ::CloseHandle(ProcessInfo.hProcess);
        ::CloseHandle(ProcessInfo.hThread);
    }

    if (szExecCmd) {
        delete[] szExecCmd;
        szExecCmd = NULL;
    }

    return dwRetCode;
}

int KWinExecAsUserW(const wchar_t* pszCmdLine,
                    const wchar_t* pszCurDir,
                    DWORD dwCmdShow,
                    DWORD dwWaitTime,
                    HANDLE hToken,
                    DWORD* pdwGetExitCodeErr,
                    DWORD* pdwExitCode)
{
    if ((NULL == pszCmdLine) || (NULL == hToken))
        return -1;

    if (!((pdwGetExitCodeErr && pdwExitCode) || (!pdwGetExitCodeErr && !pdwExitCode)))
        return -1;

    DWORD               dwRetCode   = 0;
    DWORD               dwCreateFlag = 0;
    PROCESS_INFORMATION ProcessInfo = { 0 };
    STARTUPINFO         StartupInfo = {0};
    StartupInfo.cb          = sizeof(STARTUPINFO);
    StartupInfo.dwFlags     = STARTF_USESHOWWINDOW;
    StartupInfo.wShowWindow = static_cast<WORD>(SW_SHOWNORMAL);
    wchar_t* szExecCmd = NULL;
    size_t size = wcslen(pszCmdLine);
    szExecCmd = new wchar_t[size + 1];
    wcsncpy(szExecCmd, pszCmdLine, size + 1);

    if (SW_HIDE == dwCmdShow)
        dwCreateFlag = CREATE_NO_WINDOW;

    void* pEnvironment = NULL;
    BOOL bCreateEnv = CreateEnvironmentBlock(
                          &pEnvironment,
                          hToken,
                          FALSE
                      );

    if (pEnvironment)
        dwCreateFlag |= CREATE_UNICODE_ENVIRONMENT;

    BOOL bRetCode = FALSE;
    bRetCode = ::CreateProcessAsUserW(
                   hToken,
                   NULL,
                   szExecCmd,
                   NULL,
                   NULL,
                   FALSE,
                   dwCreateFlag,
                   pEnvironment,
                   pszCurDir,
                   &StartupInfo,
                   &ProcessInfo
               );

    if (!bRetCode) {
        dwRetCode = ::GetLastError();
    } else {
        dwRetCode = 0;

        if (dwWaitTime)
            ::WaitForSingleObject(ProcessInfo.hProcess, dwWaitTime);

        if (pdwGetExitCodeErr && pdwExitCode) {
            if (::GetExitCodeProcess(ProcessInfo.hProcess, pdwExitCode))
                *pdwGetExitCodeErr = 0;
            else
                *pdwGetExitCodeErr = ::GetLastError();
        }

        ::CloseHandle(ProcessInfo.hProcess);
        ::CloseHandle(ProcessInfo.hThread);
    }

    if (szExecCmd) {
        delete[] szExecCmd;
        szExecCmd = NULL;
    }

    if (pEnvironment) {
        DestroyEnvironmentBlock(pEnvironment);
    }

    return dwRetCode;
}







}

namespace cactus
{
KDosNames::KDosNames()
{
    m_bInited = FALSE;
}

KDosNames::~KDosNames()
{
}

BOOL KDosNames::Init()
{
    if (m_bInited)
        return TRUE;

    m_bInited = GetDosDeviceNames();
    return m_bInited;
}

BOOL KDosNames::DevicePathToDosPath(CString& strPath)
{
    for (size_t i = 0; i < m_vecDosDeviceNames.size(); i += 2) {
        if (_tcsnicmp(strPath, m_vecDosDeviceNames[i], m_vecDosDeviceNames[i].GetLength()) == 0) {
            strPath.Replace(m_vecDosDeviceNames[i], m_vecDosDeviceNames[i + 1]);
            return TRUE;
        }
    }

    return FALSE;
}

BOOL KDosNames::GetDosDeviceNames(void)
{
    TCHAR szDevicesName[MAX_LOGICAL_DRIVE_LENGTH + 1] = {0};
    int nLen = GetLogicalDriveStrings(MAX_LOGICAL_DRIVE_LENGTH, szDevicesName);

    if (!nLen) return FALSE;

    int nCount = nLen / LOGICAL_DRIVE_NAME_LEN;
    TCHAR szTargetPath[MAX_PATH + 1] = {0};
    TCHAR *szCurrentDevice = szDevicesName;
    szTargetPath[MAX_PATH] = '\0';
    m_vecDosDeviceNames.clear();

    for (int i = 0; i < nCount; i++) {
        szCurrentDevice[2] = '\0';

        if (QueryDosDevice(szCurrentDevice, szTargetPath, MAX_PATH)) {
            m_vecDosDeviceNames.push_back(szTargetPath);
            m_vecDosDeviceNames.push_back(szCurrentDevice);
        }

        szCurrentDevice += LOGICAL_DRIVE_NAME_LEN;
    }

    return TRUE;
}
}

namespace cactus
{

KPsapiDllFunction::KPsapiDllFunction() : m_bInited(FALSE), m_hPsapi(NULL)
{
    m_pEnumProcesses = NULL;
    m_pGetModuleBaseName = NULL;
    m_pEnumProcessModules = NULL;
    m_pGetModuleFileNameEx = NULL;
    m_pGetProcessImageFileName = NULL;
}

KPsapiDllFunction::~KPsapiDllFunction()
{
    UnInit();
}

BOOL KPsapiDllFunction::Init()
{
    if (!m_bInited) {
        m_hPsapi = ::LoadLibrary(_T("PSAPI.DLL"));

        if (m_hPsapi == NULL) goto Exit0;

        m_pEnumProcessModules           = (PEnumProcessModules)::GetProcAddress(m_hPsapi, "EnumProcessModules");
        m_pEnumProcesses                = (PEnumProcesses)::GetProcAddress(m_hPsapi, "EnumProcesses");
        m_pGetModuleBaseName            = (PGetModuleBaseName)::GetProcAddress(m_hPsapi, "GetModuleBaseNameW");
        m_pGetModuleFileNameEx      = (PGetModuleFileNameEx)::GetProcAddress(m_hPsapi, "GetModuleFileNameExW");
        m_pGetProcessImageFileName = (PGetProcessImageFileName)::GetProcAddress(m_hPsapi, "GetProcessImageFileNameW");
        m_bInited = m_pEnumProcesses
                    && m_pGetModuleBaseName
                    && m_pEnumProcessModules
                    && m_pGetModuleFileNameEx
                    && m_pGetProcessImageFileName;
    }

Exit0:
    return  m_bInited;
}

void KPsapiDllFunction::UnInit()
{
    if (!m_bInited)
        return;

    m_pEnumProcesses = NULL;
    m_pGetModuleBaseName = NULL;
    m_pEnumProcessModules = NULL;
    m_pGetModuleFileNameEx = NULL;

    if (m_hPsapi)
        FreeLibrary(m_hPsapi);

    m_bInited = FALSE;
}

BOOL KPsapiDllFunction::EnumProcesses(DWORD* pProcessIds, DWORD cb, DWORD* pBytesReturned)
{
    if (m_bInited == FALSE)
        Init();

    if (m_pEnumProcesses)
        return m_pEnumProcesses(pProcessIds, cb, pBytesReturned);

    return FALSE;
}

DWORD KPsapiDllFunction::GetModule_BaseName(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName, DWORD nSize)
{
    if (m_bInited == FALSE)
        Init();

    if (m_pGetModuleBaseName)
        return m_pGetModuleBaseName(hProcess, hModule, lpBaseName, nSize);

    return 0;
}

BOOL KPsapiDllFunction::EnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded)
{
    if (m_bInited == FALSE)
        Init();

    if (m_pEnumProcessModules)
        return m_pEnumProcessModules(hProcess, lphModule, cb, lpcbNeeded);

    return FALSE;
}

DWORD KPsapiDllFunction::GetModule_FileNameEx(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename, DWORD nSize)
{
    if (m_bInited == FALSE)
        Init();

    if (m_pGetModuleFileNameEx)
        return m_pGetModuleFileNameEx(hProcess, hModule, lpFilename, nSize);

    return 0;
}

DWORD KPsapiDllFunction::GetProcess_ImageFileName(HANDLE hProcess, LPTSTR lpImageFileName, DWORD nSize)
{
    if (m_bInited == FALSE)
        Init();

    if (m_pGetProcessImageFileName)
        return m_pGetProcessImageFileName(hProcess, lpImageFileName, nSize);

    return 0;
}
}

namespace cactus
{
KSysProcess::KSysProcess(void)
{
    m_dosNames.Init();
}

KSysProcess::~KSysProcess(void)
{
}

CString KSysProcess::GetProcessPath(int nPid)
{
    HANDLE hProcess = NULL;
    CString strPath;
    hProcess = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, nPid);

    if (hProcess == NULL) {
        hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nPid);

        if (hProcess == NULL) goto Exit0;
    }

    strPath = GetProcessPath64(hProcess);
Exit0:

    if (hProcess)
        CloseHandle(hProcess);

    return strPath;
}

CString KSysProcess::GetProcessPath64(HANDLE hProcess)
{
    TCHAR szProcessPath[MAX_PATH] = {0};
    CString strPath;

    if (0 == m_psapi.GetProcess_ImageFileName(hProcess, szProcessPath, MAX_PATH))
        goto Exit0;

    strPath = szProcessPath;
    m_dosNames.DevicePathToDosPath(strPath);
Exit0:
    return strPath;
}

CString KSysProcess::GetProcessPath32(HANDLE hProcess)
{
    TCHAR szProcessPath[MAX_PATH] = {0};
    CString strPath;

    if (0 == m_psapi.GetModule_FileNameEx(hProcess, NULL, szProcessPath, MAX_PATH))
        goto Exit0;

    strPath = szProcessPath;

    if (!ConvertSysPath(strPath))
        strPath.Empty();

Exit0:
    return strPath;
}

BOOL        KSysProcess::GetAllProcess(std::vector<int>& vecPID)
{
    BOOL bRet = FALSE;
    DWORD dwProcessIDArr[1024] = { 0 };
    DWORD dwNeeded = 0, dwProcessIDCount = 0;
    GetDebugPrivilege();
    bRet = m_psapi.EnumProcesses(dwProcessIDArr, sizeof(dwProcessIDArr), &dwNeeded);

    if (bRet == FALSE) goto Exit0;

    dwProcessIDCount = dwNeeded / sizeof(DWORD);

    for (DWORD i = 0; i < dwProcessIDCount; i++)
        vecPID.push_back(dwProcessIDArr[i]);

    bRet = true;
Exit0:
    return bRet;
}

BOOL KSysProcess::GetProcessModules(int nPID, std::vector<CString> &vecModule)
{
    BOOL bRet = FALSE;
    DWORD cbNeeded = 0;
    HMODULE* hModules = NULL;
    TCHAR szFullPath[MAX_PATH * 2 + 1] = { 0 };
    HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, nPID);

    if (hProcess == NULL) goto Exit0;

    if (!m_psapi.EnumProcessModules(hProcess, NULL, 0, &cbNeeded))
        goto Exit0;

    hModules = new HMODULE[cbNeeded / sizeof(HMODULE)];

    if (hModules == NULL) goto Exit0;

    if (!m_psapi.EnumProcessModules(hProcess, hModules, cbNeeded, &cbNeeded))
        goto Exit0;

    for (size_t i = 0; i < cbNeeded / sizeof(HMODULE); i++) {
        if (m_psapi.GetModule_FileNameEx(hProcess, hModules[i], szFullPath, MAX_PATH * 2)) {
            CString strPath = szFullPath;

            if (ConvertSysPath(strPath))
                vecModule.push_back(strPath);
        }
    }

    bRet = TRUE;
Exit0:

    if (hModules != NULL)
        delete [] hModules;

    if (hProcess != NULL)
        ::CloseHandle(hProcess);

    return bRet;
}

BOOL KSysProcess::GetDebugPrivilege(void)
{
    BOOL bReturn = FALSE;
    HANDLE hToken = NULL;
    int nRetCode = ERROR_SUCCESS;
    LUID CurrentLUID = { 0 };
    TOKEN_PRIVILEGES TokenPrivileges = { 0 };

    __try {
        nRetCode = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken);

        if (!nRetCode) __leave;

        nRetCode = ::LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &CurrentLUID);

        if (!nRetCode) __leave;

        TokenPrivileges.PrivilegeCount = 1;
        TokenPrivileges.Privileges[0].Luid = CurrentLUID;
        TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        nRetCode = ::AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL);

        if (!nRetCode) __leave;

        bReturn = TRUE;
    } __finally {
        if (hToken) {
            ::CloseHandle(hToken);
            hToken = NULL;
        }
    }

    return bReturn;
}

BOOL KSysProcess::ConvertSysPath(CString& strPath)
{
    BOOL bFunRet = FALSE;
    WCHAR szPath[MAX_PATH] = {0};
    int nLen = 0;
    int nSysLen = 0;

    if (strPath.Left(1) == TEXT("\\")) {
        nLen = (int)wcslen(TEXT("\\??\\"));
        nSysLen = (int)wcslen(TEXT("\\SystemRoot\\"));

        if (strPath.Left(nLen) == TEXT("\\??\\")) {
            strPath = strPath.Right(strPath.GetLength() - nLen);
        } else if (strPath.Left(nSysLen) == TEXT("\\SystemRoot\\")) {
            ::ExpandEnvironmentStrings(TEXT("%SystemRoot%"), szPath, MAX_PATH);
            strPath = (CString)szPath + strPath.Right(strPath.GetLength() - nSysLen + 1);
        }

        if (strPath.GetLength() <= 3) {
            strPath.Empty();
            goto Exit0;
        }

        if (strPath.GetAt(1) != L':') {
            strPath.Empty();
            goto Exit0;
        }
    }

    bFunRet = TRUE;
Exit0:
    return bFunRet;
}

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);
BOOL KSysProcess::IsWow64()
{
    static int s_nIsWow64 = -1;

    if (s_nIsWow64 != -1)
        return s_nIsWow64;

    LPFN_ISWOW64PROCESS fnIsWow64Process = (LPFN_ISWOW64PROCESS)GetProcAddress(
            GetModuleHandle(L"kernel32"), "IsWow64Process");

    if (NULL != fnIsWow64Process) {
        if (fnIsWow64Process(GetCurrentProcess(), &s_nIsWow64)) {
            return s_nIsWow64;
        }
    }

    s_nIsWow64 = FALSE;
    return s_nIsWow64;
}



int KSysProcess::GetParentProcessID(int nId)
{
    LONG                      status;
    int                     nParentPID = -1;
    HANDLE                    hProcess;
    PROCESS_BASIC_INFORMATION pbi;
    // Get process handle
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nId);

    if (!hProcess) return nParentPID;

    PROCNTQSIP NtQueryInformationProcess;
    NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(
                                    GetModuleHandle(L"ntdll"),
                                    "NtQueryInformationProcess"
                                );
    // Retrieve information
    status = NtQueryInformationProcess(hProcess,
                                       ProcessBasicInformation,
                                       (PVOID)&pbi,
                                       sizeof(PROCESS_BASIC_INFORMATION),
                                       NULL
                                      );

    // Copy parent Id on success
    if (!status)
        nParentPID = pbi.InheritedFromUniqueProcessId;

    CloseHandle(hProcess);
    return nParentPID;
}
}

namespace cactus
{
KSysProcessMonitor& KSysProcessMonitor::Instance()
{
    static KSysProcessMonitor subject;
    return subject;
}

KSysProcessMonitor::~KSysProcessMonitor()
{
    WaitStopMonitor(50);
    ATL::CComCritSecLock<CComAutoCriticalSection> guard(m_csMonitorThread);

    if (NULL != m_hMonitorThread) {
        ::CloseHandle(m_hMonitorThread);
        m_hMonitorThread = NULL;
    }
}

KSysProcessMonitor::KSysProcessMonitor()
{
    m_hMonitorThread    = NULL;
    m_bStop             = false;
}

HRESULT __stdcall KSysProcessMonitor::RegisterObserver(IKSysProcessObserver* pIObserver)
{
    if (NULL == pIObserver) {
        return E_INVALIDARG;
    }

    ATL::CComCritSecLock<CComAutoCriticalSection> guard1(m_csObserver);

    if (0 == m_mapObserver.count(pIObserver)) {
        m_mapObserver.insert(std::make_pair(pIObserver, false));
    }

    ATL::CComCritSecLock<CComAutoCriticalSection> guard2(m_csMonitorThread);

    if (NULL == m_hMonitorThread) {
        m_hMonitorThread = (HANDLE)_beginthreadex(NULL, NULL, MonitorThread, this, NULL, NULL);

        if (NULL == m_hMonitorThread) {
            return HRESULT_FROM_WIN32(::GetLastError());
        }
    }

    return S_OK;
}

HRESULT __stdcall KSysProcessMonitor::UnregisterObserver(IKSysProcessObserver* pIObserver)
{
    if (NULL == pIObserver) {
        return E_INVALIDARG;
    }

    ATL::CComCritSecLock<CComAutoCriticalSection> guard(m_csObserver);

    if (0 == m_mapObserver.count(pIObserver)) {
        return E_INVALIDARG;
    }

    m_mapObserver.erase(pIObserver);
    return S_OK;
}

unsigned int WINAPI KSysProcessMonitor::MonitorThread(LPVOID lParam)
{
    return reinterpret_cast<KSysProcessMonitor*>(lParam)->MonitorThreadRun();
}

DWORD WINAPI KSysProcessMonitor::MonitorThreadRun()
{
    while (true) {
        MAP_OBSERVER_2_NOTIFIED mapObserver;
        {
            ATL::CComCritSecLock<CComAutoCriticalSection> guard(m_csObserver);
            mapObserver = m_mapObserver;
        }

        // 没有监听者则不获取进程列表
        if (mapObserver.empty()) {
            if (this->SleepWithVigilance()) {
                return 0;
            } else {
                continue;
            }
        }

        // 获取进程列表
        MAP_PROCID_2_INFO mapProcNew;
        MAP_PROCID_2_INFO mapProcEnd;
        BOOL bRet = this->RefreshCurProcList(mapProcNew, mapProcEnd);

        if (!bRet) {
            if (this->SleepWithVigilance()) {
                return 0;
            } else {
                continue;
            }
        }

        // 将进程刷新消息通知给所有观察者
        this->NotifyObservers(mapProcNew, mapProcEnd);

        // Sleep 两秒
        if (this->SleepWithVigilance()) {
            return 0;
        }
    }

    return 0;
}

HRESULT KSysProcessMonitor::WaitStopMonitor(DWORD dwTimeout)
{
    m_bStop = true;
    ATL::CComCritSecLock<CComAutoCriticalSection> guard(m_csMonitorThread);

    if (NULL == m_hMonitorThread) {
        return S_OK;
    } else {
        DWORD dwRet = ::WaitForSingleObject(m_hMonitorThread, dwTimeout);
        return (WAIT_OBJECT_0 == dwRet) ? S_OK : E_FAIL;
    }
}

BOOL KSysProcessMonitor::RefreshCurProcList(
    /*[out]*/ MAP_PROCID_2_INFO& mapProcNew,
    /*[out]*/ MAP_PROCID_2_INFO& mapProcEnd
)
{
    std::vector<int> vecCurrPID;
    BOOL bRet = m_sysProcess.GetAllProcess(vecCurrPID);

    if (!bRet) {
        return FALSE;
    }

    std::set<DWORD> setEndProcId;   // 上次保存的但目前已结束进程id列表
    ATL::CComCritSecLock<ATL::CComAutoCriticalSection> guard(m_csProcData);
    MAP_PROCID_2_INFO::iterator iterMap;

    for (iterMap = m_mapProcId2Info.begin();
         iterMap != m_mapProcId2Info.end();
         ++iterMap) {
        setEndProcId.insert(iterMap->first);
    }

    for (int i = 0; i != vecCurrPID.size(); ++i) {
        if (setEndProcId.end() != setEndProcId.find(vecCurrPID[i])) {
            setEndProcId.erase(vecCurrPID[i]);
        } else {
            S_PROC_INFO procInfo;
            procInfo.nProcessId = vecCurrPID[i];
            procInfo.wstrExePath = static_cast<LPCWSTR>(m_sysProcess.GetProcessPath(vecCurrPID[i]));
            mapProcNew.insert(std::make_pair(vecCurrPID[i], procInfo));
            m_mapProcId2Info.insert(std::make_pair(vecCurrPID[i], procInfo));
        }
    }

    std::set<DWORD>::iterator iterSet;

    for (iterSet = setEndProcId.begin(); iterSet != setEndProcId.end(); ++iterSet) {
        S_PROC_INFO procInfo;
        procInfo.nProcessId = *iterSet;
        procInfo.wstrExePath = m_mapProcId2Info[*iterSet].wstrExePath;
        mapProcEnd.insert(std::make_pair(*iterSet, procInfo));
        m_mapProcId2Info.erase(*iterSet);
    }

    return TRUE;
}

BOOL KSysProcessMonitor::SleepWithVigilance()
{
    // 等待2秒再继续，并检测是否需要退出
    for (int i = 0; i != 40; ++i) {
        if (m_bStop) {
            return TRUE;
        }

        ::Sleep(50);
    }

    return FALSE;
}

void KSysProcessMonitor::NotifyObservers(
    const MAP_PROCID_2_INFO& mapProcNew,
    const MAP_PROCID_2_INFO& mapProcEnd
)
{
    ATL::CComCritSecLock<ATL::CComAutoCriticalSection> guard(m_csObserver);
    MAP_OBSERVER_2_NOTIFIED::iterator iterMap;

    for (iterMap = m_mapObserver.begin();
         iterMap != m_mapObserver.end();
         ++iterMap) {
        if (iterMap->second) {
            HRESULT hRet = iterMap->first->NotifyProcessChange(
                               mapProcNew,
                               mapProcEnd
                           );
        } else {
            // 没有回调过的，为第一次取进程列表，则全部填充至新创建进程列表中
            HRESULT hRet = iterMap->first->NotifyProcessChange(
                               m_mapProcId2Info,
                               MAP_PROCID_2_INFO()
                           );
            iterMap->second = true;
        }
    }
}



}


