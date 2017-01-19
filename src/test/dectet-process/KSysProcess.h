#pragma once
#include <vector>
#include <Windows.h>
#include <atlstr.h>
#include "KDosNames.h"
#include <map>

namespace pnpsub
{



class KPsapiDllFunction
{
public:
	KPsapiDllFunction();
	~KPsapiDllFunction();

	BOOL Init();
	void UnInit();

	BOOL EnumProcesses(DWORD* pProcessIds, DWORD cb,	DWORD* pBytesReturned);
	DWORD GetModuleBaseName(HANDLE hProcess, HMODULE hModule, LPWSTR lpBaseName,	DWORD nSize);
	BOOL EnumProcessModules(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);
	DWORD GetModuleFileNameEx(HANDLE hProcess, HMODULE hModule, LPWSTR lpFilename,DWORD nSize);
	DWORD GetProcessImageFileName(	HANDLE hProcess, LPWSTR lpImageFileName, DWORD nSize);

protected:
	typedef BOOL (WINAPI *PEnumProcesses)(DWORD* pProcessIds, DWORD cb,	DWORD* pBytesReturned);
	typedef DWORD (WINAPI *PGetModuleBaseName)(HANDLE hProcess, HMODULE hModule, LPWSTR lpBaseName,	DWORD nSize);
	typedef BOOL (WINAPI *PEnumProcessModules)(HANDLE hProcess, HMODULE* lphModule, DWORD cb, LPDWORD lpcbNeeded);
	typedef DWORD (WINAPI *PGetModuleFileNameEx)(HANDLE hProcess, HMODULE hModule, LPWSTR lpFilename,DWORD nSize);
	typedef BOOL (WINAPI *PGetProcessImageFileName)(HANDLE hProcess,LPWSTR lpImageFileName, DWORD nSize);

	PEnumProcesses		m_pEnumProcesses;
	PGetModuleBaseName	m_pGetModuleBaseName;
	PEnumProcessModules  m_pEnumProcessModules;
	PGetModuleFileNameEx m_pGetModuleFileNameEx;
	PGetProcessImageFileName m_pGetProcessImageFileName;

private:
	BOOL m_bInited;
	HMODULE m_hPsapi;
};


class KSysProcess
{
public:
	KSysProcess(void);
	~KSysProcess(void);

	CStringW		GetProcessPath(int nPid);
	BOOL		GetAllProcess(std::vector<int>& vecPID);
	BOOL		GetProcessModules( int nPID, std::vector<CStringW> &vecModule );
	BOOL		GetDebugPrivilege(void);
	BOOL		IsWow64();
	int	GetParentProcessID(int nId);

protected:
	BOOL ConvertSysPath(CStringW& strPath);
	CStringW GetProcessPath64(HANDLE hProcess);
	CStringW GetProcessPath32(HANDLE hProcess);

private:
	KPsapiDllFunction m_psapi;
	KDosNames m_dosNames;
};

//-------------------------------------------------------------------------

typedef struct _S_PROC_INFO
{
    int                             nProcessId;         ///< 进程 id
    std::wstring                    wstrExePath;        ///< 进程 exe 全路径
    //std::vector<std::wstring>       vecModuleList;      ///< 进程模块列表

    _S_PROC_INFO() : nProcessId(-1)
    {
    }
} S_PROC_INFO;

typedef std::map<DWORD, S_PROC_INFO> MAP_PROCID_2_INFO;

interface IKSysProcessObserver
{
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
        /*[in]*/    const MAP_PROCID_2_INFO&    mapNewProc,
        /*[in]*/    const MAP_PROCID_2_INFO&    mapEndProc
        ) = 0;
};

interface IKSysProcessSubject
{
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
class KSysProcessMonitor : public IKSysProcessSubject
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
    static DWORD WINAPI MonitorThread(LPVOID lParam);
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