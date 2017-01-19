#include "StdAfx.h"
#include "KSysProcess.h"
#include <set>

namespace pnpsub
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

DWORD KPsapiDllFunction::GetModuleBaseName(HANDLE hProcess, HMODULE hModule, LPWSTR lpBaseName, DWORD nSize)
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

DWORD KPsapiDllFunction::GetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPWSTR lpFilename, DWORD nSize)
{
    if (m_bInited == FALSE)
        Init();

    if (m_pGetModuleFileNameEx)
        return m_pGetModuleFileNameEx(hProcess, hModule, lpFilename, nSize);

    return 0;
}

DWORD KPsapiDllFunction::GetProcessImageFileName(HANDLE hProcess, LPWSTR lpImageFileName, DWORD nSize)
{
    if (m_bInited == FALSE)
        Init();

    if (m_pGetProcessImageFileName)
        return m_pGetProcessImageFileName(hProcess, lpImageFileName, nSize);

    return 0;
}

KSysProcess::KSysProcess(void)
{
    m_dosNames.Init();
}

KSysProcess::~KSysProcess(void)
{
}

CStringW KSysProcess::GetProcessPath(int nPid)
{
    HANDLE hProcess = NULL;
    CStringW strPath;
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

CStringW KSysProcess::GetProcessPath64(HANDLE hProcess)
{
    WCHAR szProcessPath[MAX_PATH] = {0};
    CStringW strPath;

    if (0 == m_psapi.GetProcessImageFileName(hProcess, szProcessPath, MAX_PATH))
        goto Exit0;

    strPath = szProcessPath;
    m_dosNames.DevicePathToDosPath(strPath);
Exit0:
    return strPath;
}

CStringW KSysProcess::GetProcessPath32(HANDLE hProcess)
{
    WCHAR szProcessPath[MAX_PATH] = {0};
    CStringW strPath;

    if (0 == m_psapi.GetModuleFileNameEx(hProcess, NULL, szProcessPath, MAX_PATH))
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

BOOL        KSysProcess::GetProcessModules(int nPID, std::vector<CStringW> &vecModule)
{
    BOOL bRet = FALSE;
    DWORD cbNeeded = 0;
    HMODULE* hModules = NULL;
    WCHAR szFullPath[MAX_PATH * 2 + 1] = { 0 };
    HANDLE hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, nPID);

    if (hProcess == NULL) goto Exit0;

    if (!m_psapi.EnumProcessModules(hProcess, NULL, 0, &cbNeeded))
        goto Exit0;

    hModules = new HMODULE[cbNeeded / sizeof(HMODULE)];

    if (hModules == NULL) goto Exit0;

    if (!m_psapi.EnumProcessModules(hProcess, hModules, cbNeeded, &cbNeeded))
        goto Exit0;

    for (size_t i = 0; i < cbNeeded / sizeof(HMODULE); i++) {
        if (m_psapi.GetModuleFileNameEx(hProcess, hModules[i], szFullPath, MAX_PATH * 2)) {
            CStringW strPath = szFullPath;

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

BOOL KSysProcess::ConvertSysPath(CStringW& strPath)
{
    BOOL bFunRet = FALSE;
    WCHAR szPath[MAX_PATH] = {0};
    int nLen = 0;
    int nSysLen = 0;

    if (strPath.Left(1) == L"\\") {
        nLen = (int)wcslen(L"\\??\\");
        nSysLen = (int)wcslen(L"\\SystemRoot\\");

        if (strPath.Left(nLen) == L"\\??\\") {
            strPath = strPath.Right(strPath.GetLength() - nLen);
        } else if (strPath.Left(nSysLen) == L"\\SystemRoot\\") {
            ::ExpandEnvironmentStringsW(L"%SystemRoot%", szPath, MAX_PATH);
            strPath = (CStringW)szPath + strPath.Right(strPath.GetLength() - nSysLen + 1);
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
            GetModuleHandleW(L"kernel32"), "IsWow64Process");

    if (NULL != fnIsWow64Process) {
        if (fnIsWow64Process(GetCurrentProcess(), &s_nIsWow64)) {
            return s_nIsWow64;
        }
    }

    s_nIsWow64 = FALSE;
    return s_nIsWow64;
}

#define ProcessBasicInformation 0
typedef LONG(WINAPI *PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);

typedef struct {
    DWORD ExitStatus;
    DWORD PebBaseAddress;
    DWORD AffinityMask;
    DWORD BasePriority;
    ULONG UniqueProcessId;
    ULONG InheritedFromUniqueProcessId;
}   PROCESS_BASIC_INFORMATION_EX;

int KSysProcess::GetParentProcessID(int nId)
{
    LONG                      status;
    int                     nParentPID = -1;
    HANDLE                    hProcess;
    PROCESS_BASIC_INFORMATION_EX pbi;
    // Get process handle
    hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, nId);

    if (!hProcess) return nParentPID;

    PROCNTQSIP NtQueryInformationProcess;
    NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(
                                    GetModuleHandleW(L"ntdll"),
                                    "NtQueryInformationProcess"
                                );
    // Retrieve information
    status = NtQueryInformationProcess(hProcess,
                                       ProcessBasicInformation,
                                       (PVOID)&pbi,
                                       sizeof(PROCESS_BASIC_INFORMATION_EX),
                                       NULL
                                      );

    // Copy parent Id on success
    if (!status)
        nParentPID = pbi.InheritedFromUniqueProcessId;

    CloseHandle(hProcess);
    return nParentPID;
}

//-------------------------------------------------------------------------

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
        m_hMonitorThread = ::CreateThread(
                               NULL,
                               0,
                               MonitorThread,
                               this,
                               0,
                               NULL
                           );

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

DWORD WINAPI KSysProcessMonitor::MonitorThread(LPVOID lParam)
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