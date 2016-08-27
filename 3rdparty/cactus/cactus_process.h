#ifndef __CACTUS_CACTUS_PROCESS_H__
#define __CACTUS_CACTUS_PROCESS_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <atlstr.h>

namespace cactus
{

typedef std::vector<DWORD> VEC_DORD;

#ifdef _WIN64
inline BOOL RunningInWow64()
{
    return FALSE;
}
#else
BOOL CACTUS_API RunningInWow64();
#endif


BOOL CACTUS_API IsWow64();

BOOL CACTUS_API IsProcess64Bit(HANDLE aHandle);

BOOL CACTUS_API IsOS64Bit();

DWORD CACTUS_API GetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPTSTR ptszFileName, DWORD nSize);

bool CACTUS_API GetProcessCommandLine(DWORD dwPID, LPTSTR ptszCmdLine, UINT *puSize);

BOOL CACTUS_API EnumProcessesCurrent(DWORD aProcesses[1024], DWORD& cProcesses);

DWORD CACTUS_API GetChildProcessByPID(DWORD dwPid, VEC_DORD& vecChildPID);

DWORD CACTUS_API WaitProcExitByPids(VEC_DORD& vecChildPID, LONG dwTimeOut);

BOOL CACTUS_API GetFirstExitCodeByPids(VEC_DORD& vecChildPID, LONG dwTimeOut, DWORD& dwExitCode);

DWORD CACTUS_API KGetPidByHandle(HANDLE hProcess);

DWORD CACTUS_API GetParentProcessID(DWORD dwId);



/*! @brief 获取父进程的pid
 *
 * @param[in] hCurrentProcess 进程句柄
 * @param[in] dwParentPid 参数说明
 * @return 返回TRUE表示成功，否则为失败
 */
BOOL CACTUS_API GetParentPid(HANDLE hCurrentProcess, DWORD& dwParentPid);

BOOL CACTUS_API GetProcessFullPathByPid(DWORD dwPid, string16& exe_filename, DWORD& dwSize);

CString CACTUS_API GetProcessFileName(DWORD dwProcID);

string16 CACTUS_API GetProcessNameByPID(DWORD processid);

BOOL CACTUS_API GetProcessPidByFullPath(const string16& exe_filename, DWORD& dwPid);

/*! @brief 进程是否存在，完全匹配
 *
 * @param [in] process_name 参数说明
 * @return 返回0表示成功，否则为错误码
 */
BOOL CACTUS_API IsProcessExist(const string16& process_name);

/*! @brief 进程是否存在，是否包含字符串
 *
 * @param [in] process_name 参数说明
 * @return 返回0表示成功，否则为错误码
 */
BOOL CACTUS_API HasProcessExist(const string16& process_name);

BOOL CACTUS_API GetProcessesByName(const string16& process_name, std::vector<DWORD>& list);

BOOL CACTUS_API IsProcessExistByFullPath(const string16& exe_filename);

BOOL CACTUS_API KillProcess(const wchar_t* lpClassName, const wchar_t*  lpWindowName);

BOOL CACTUS_API KillProcessByHwnd(HWND hWnd);

BOOL CACTUS_API KillProcessByPid(DWORD dwPid);

BOOL CACTUS_API KillProcessByName(const string16& appname, BOOL bForce = TRUE);
BOOL CACTUS_API KiilProcessByNameAllWin(const string16& appname, BOOL bForce = TRUE);

BOOL CACTUS_API KillProcessByPid(ULONG ulProcessId, LPCTSTR lpProcessPath);

BOOL CACTUS_API KillProcessByPath(const string16& exe_filename, int* pnProcessFound = NULL);

BOOL CACTUS_API KillProcessByFullPath(const string16& exe_filename);

BOOL CACTUS_API GetCurrentUserSID(string16& strCurrentUserSID);


/**
* @brief 创建进程
* @param[in] pszCmdLine 命令行参数
* @param[in] pszCurDir 执行进程的当前目录
* @param[in] dwCmdShow 可用SW_HIDE SW_SHOWDEFAULT等宏定义
* @param[in] dwWaitTime 执行后等待子进程的时间
* @param[out] pdwGetExitCodeErr 获取子进程返回值函数执行的错误码,为0证明执行成功
* @param[out] pdwExitCode 子进程返回值
* @return 0 成功，其它为GetLastError错误码
*/
int CACTUS_API KWinExecW(const wchar_t* pszCmdLine,
                         const wchar_t* pszCurDir,
                         DWORD dwCmdShow,
                         DWORD dwWaitTime,
                         DWORD* pdwGetExitCodeErr = NULL,
                         DWORD* pdwExitCode = NULL);

int CACTUS_API KWinExecAsUserW(const wchar_t* pszCmdLine,
                               const wchar_t* pszCurDir,
                               DWORD dwCmdShow,
                               DWORD dwWaitTime,
                               HANDLE hToken,
                               DWORD* pdwGetExitCodeErr = NULL,
                               DWORD* pdwExitCode = NULL);


class CACTUS_API KDosNames
{
public:
    KDosNames();
    ~KDosNames();

    BOOL Init();
    BOOL DevicePathToDosPath(CString& strPath);
protected:
    BOOL GetDosDeviceNames(void);

private:
    static const int LOGICAL_DRIVE_NAME_LEN = 4;
    static const int MAX_LOGICAL_DRIVE_LENGTH = (LOGICAL_DRIVE_NAME_LEN * 26 + 1);

    BOOL m_bInited;
    std::vector<CString> m_vecDosDeviceNames;
};

class CACTUS_API KPsapiDllFunction
{
private:
    BOOL m_bInited;
    HMODULE m_hPsapi;

public:
    KPsapiDllFunction();
    ~KPsapiDllFunction();

public:
    BOOL Init();
    void UnInit();
    BOOL EnumProcesses(DWORD* pProcessIds, DWORD cb, DWORD* pBytesReturned);
    DWORD GetModule_BaseName(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName, DWORD nSize);
    BOOL EnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);
    DWORD GetModule_FileNameEx(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename, DWORD nSize);
    DWORD GetProcess_ImageFileName(HANDLE hProcess, LPTSTR lpImageFileName, DWORD nSize);

protected:
    typedef BOOL (WINAPI *PEnumProcesses)(DWORD* pProcessIds, DWORD cb, DWORD* pBytesReturned);
    typedef DWORD (WINAPI *PGetModuleBaseName)(HANDLE hProcess, HMODULE hModule, LPTSTR lpBaseName, DWORD nSize);
    typedef BOOL (WINAPI *PEnumProcessModules)(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);
    typedef DWORD (WINAPI *PGetModuleFileNameEx)(HANDLE hProcess, HMODULE hModule, LPTSTR lpFilename, DWORD nSize);
    typedef BOOL (WINAPI *PGetProcessImageFileName)(HANDLE hProcess, LPTSTR lpImageFileName, DWORD nSize);

    PEnumProcesses      m_pEnumProcesses;
    PGetModuleBaseName  m_pGetModuleBaseName;
    PEnumProcessModules  m_pEnumProcessModules;
    PGetModuleFileNameEx m_pGetModuleFileNameEx;
    PGetProcessImageFileName m_pGetProcessImageFileName;

};

class CACTUS_API KSysProcess
{
public:
    KSysProcess(void);
    ~KSysProcess(void);

    CString     GetProcessPath(int nPid);
    BOOL        GetAllProcess(std::vector<int>& vecPID);
    BOOL        GetProcessModules(int nPID, std::vector<CString> &vecModule);
    BOOL        GetDebugPrivilege(void);
    BOOL        IsWow64();
    int GetParentProcessID(int nId);

protected:
    BOOL ConvertSysPath(CString& strPath);
    CString GetProcessPath64(HANDLE hProcess);
    CString GetProcessPath32(HANDLE hProcess);

private:
    KPsapiDllFunction m_psapi;
    KDosNames m_dosNames;
};


//-------------------------------------------------------------------------

typedef struct _S_PROC_INFO {
    int                             nProcessId;         ///< 进程 id
    std::wstring                    wstrExePath;        ///< 进程 exe 全路径
    //std::vector<std::wstring>       vecModuleList;      ///< 进程模块列表

    _S_PROC_INFO() : nProcessId(-1)
    {
    }
} S_PROC_INFO;

typedef std::map<DWORD, S_PROC_INFO> MAP_PROCID_2_INFO;

interface IKSysProcessObserver {
    /**
    * @brief        :  通知观察者进程列表变化
    * @remark_1     :  若为第一次回调，则 mapNewProc 中保存了当前系统进程列表，
    *                  mapEndProc 将为空
    * @remark_2     :  观察者不应该在回调中执行耗时任务，以防影响其他观察者
    * @param[in]    :  mapNewProc 新近创建的进程列表
    * @param[in]    :  mapEndProc 新近终止的进程列表
     * @return       :  S_OK 成功，其他为HRESULT错误码
    */
    virtual HRESULT __stdcall NotifyProcessChange(
        /*[in]*/    const MAP_PROCID_2_INFO &    mapNewProc,
        /*[in]*/    const MAP_PROCID_2_INFO&    mapEndProc
    ) = 0;
};

interface IKSysProcessSubject {
    virtual HRESULT __stdcall RegisterObserver(
        IKSysProcessObserver* pIObserver
    ) = 0;

    virtual HRESULT __stdcall UnregisterObserver(
        IKSysProcessObserver* pIObserver
    ) = 0;
};

/*
*@brief  : 当前系统进程信息提供者
*@remark : 1. 内部每2秒钟刷新一次，并回调通知观察者。若进程列表无变化，则回调出去的
*             进程状态列表为空
*          2. 只要外部有有效的观察者注册过来，则内部就启动线程来监控进程列表。外部无
*             需手动启动监控
*          3. 当观察者列表为空时，停止进程监控
*          4. 本实现为线程安全
*/
class CACTUS_API KSysProcessMonitor : public IKSysProcessSubject
{
public:
    static KSysProcessMonitor& Instance();
    ~KSysProcessMonitor();

public:
    virtual HRESULT __stdcall RegisterObserver(
        IKSysProcessObserver* pIObserver
    );

    virtual HRESULT __stdcall UnregisterObserver(
        IKSysProcessObserver* pIObserver
    );

private:
    typedef std::map<IKSysProcessObserver*, bool>   MAP_OBSERVER_2_NOTIFIED;

private:
    KSysProcessMonitor();
    static unsigned int WINAPI MonitorThread(LPVOID lParam);
    DWORD WINAPI MonitorThreadRun();
    HRESULT WaitStopMonitor(DWORD dwTimeout);
    BOOL RefreshCurProcList(
        /*[out]*/   MAP_PROCID_2_INFO& mapProcNew,  // 新增的进程列表
        /*[out]*/   MAP_PROCID_2_INFO& mapProcEnd   // 结束的进程列表
    );
    BOOL SleepWithVigilance();  // 返回真表示需要退出线程
    void NotifyObservers(
        const MAP_PROCID_2_INFO& mapProcNew,
        const MAP_PROCID_2_INFO& mapProcEnd
    );

private:
    ATL::CComAutoCriticalSection        m_csMonitorThread;
    HANDLE                              m_hMonitorThread;

    ATL::CComAutoCriticalSection        m_csProcData;
    MAP_PROCID_2_INFO                   m_mapProcId2Info;

    ATL::CComAutoCriticalSection        m_csObserver;
    MAP_OBSERVER_2_NOTIFIED             m_mapObserver;

    bool                                m_bStop;

    KSysProcess                         m_sysProcess;
};

}


namespace cactus
{
class CACTUS_API CBkProcPrivilege
{
public:
    CBkProcPrivilege()
    {
    }

    BOOL SetDebug(BOOL bSet)
    {
        BOOL bRet  = FALSE;
        HANDLE hTokenProc = NULL;
        bRet = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hTokenProc);

        if (!bRet)
            goto Exit0;

        bRet = _SetPrivilege(hTokenProc, SE_DEBUG_NAME, bSet);

        if (!bRet)
            goto Exit0;

Exit0:

        if (hTokenProc) {
            ::CloseHandle(hTokenProc);
            hTokenProc = NULL;
        }

        return bRet;
    }

    BOOL EnableShutdown()
    {
        BOOL bRet  = FALSE;
        HANDLE hTokenProc = NULL;
        bRet = ::OpenProcessToken(::GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hTokenProc);

        if (!bRet)
            goto Exit0;

        bRet = _SetPrivilege(hTokenProc, SE_SHUTDOWN_NAME, TRUE);

        if (!bRet)
            goto Exit0;

Exit0:

        if (hTokenProc) {
            ::CloseHandle(hTokenProc);
            hTokenProc = NULL;
        }

        return bRet;
    }

private:

    BOOL _SetPrivilege(HANDLE hToken, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege)
    {
        BOOL RetVal = FALSE;
        BOOL bResult  = FALSE;
        TOKEN_PRIVILEGES tp;
        LUID luid;
        bResult = ::LookupPrivilegeValue(NULL, lpszPrivilege, &luid);

        if (!bResult) {
            goto Exit0;
        }

        tp.PrivilegeCount       = 1;
        tp.Privileges[0].Luid   = luid;

        if (bEnablePrivilege)
            tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
        else
            tp.Privileges[0].Attributes = SE_PRIVILEGE_REMOVED;

        bResult = ::AdjustTokenPrivileges(
                      hToken,
                      FALSE,
                      &tp,
                      sizeof(TOKEN_PRIVILEGES),
                      (PTOKEN_PRIVILEGES)NULL,
                      (PDWORD)NULL
                  );

        if (!bResult)
            goto Exit0;

        RetVal = TRUE;
Exit0:
        return RetVal;
    }
};
}


#endif // __CACTUS_CACTUS_PROCESS_H__
