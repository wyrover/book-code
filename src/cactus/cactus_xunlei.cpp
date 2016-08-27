#include <cactus/cactus_xunlei.h>
#include <cactus/cactus_ping.h>

#include <tlhelp32.h>
#include <Psapi.h>
#include <Shlwapi.h>

#pragma comment(lib, "psapi.lib")
#pragma comment(lib, "shlwapi.lib")
//#include <publish/md5/KMD5.h>



static const int kMaxLogLen = 16 * 1024;
#define MAX_LEN (kMaxLogLen + 1)

void CCLog(const char* pszFormat, ...)
{
    char szBuf[MAX_LEN];
    va_list ap;
    va_start(ap, pszFormat);
    vsnprintf_s(szBuf, MAX_LEN, MAX_LEN, pszFormat, ap);
    va_end(ap);
    WCHAR wszBuf[MAX_LEN] = {0};
    MultiByteToWideChar(CP_ACP, 0, szBuf, -1, wszBuf, sizeof(wszBuf));
    OutputDebugStringW(wszBuf);
    OutputDebugStringA("\n");
    WideCharToMultiByte(CP_ACP, 0, wszBuf, sizeof(wszBuf), szBuf, sizeof(szBuf), NULL, FALSE);
    printf("%s\n", szBuf);
}

void CCLog(const wchar_t* pszFormat, ...)
{
    va_list ap;
    wchar_t buffer[MAX_LEN];
    va_start(ap, pszFormat);
    _vsnwprintf_s(buffer, MAX_LEN, MAX_LEN, pszFormat, ap);
    va_end(ap);
    OutputDebugStringW(buffer);
    OutputDebugStringA("\n");
    wprintf_s(L"%s\n", buffer);
}


EXTERN_C IMAGE_DOS_HEADER __ImageBase;

std::wstring GetCfgPath(LPCWSTR szBuffer)
{
    WCHAR szCachePath[MAX_PATH + 1] = {0};
    ::GetModuleFileName((HMODULE)&__ImageBase, szCachePath, MAX_PATH);
    ::PathRemoveFileSpec(szCachePath);

    if (szBuffer)
        ::PathAppend(szCachePath, szBuffer);
    else
        ::PathAddBackslash(szCachePath);

    return std::wstring(szCachePath);
}


namespace cactus
{




KFirewallRuleRegister::KFirewallRuleRegister(void)
{
}

KFirewallRuleRegister::~KFirewallRuleRegister(void)
{
}

DWORD KFirewallRuleRegister::EnableNetWorkCommunication(
    __in LPCTSTR lpctModuleFullPath)
{
    if (!lpctModuleFullPath) {
        return ERROR_INVALID_PARAMETER;
    }

    DWORD dwRet = -1;

    do {
        CString strCmd, strParam;
        _ParseCmd(lpctModuleFullPath, Act_EnableNewCommunication, strCmd, strParam);
        SHELLEXECUTEINFO ShellInfo;
        memset(&ShellInfo, 0, sizeof(ShellInfo));
        ShellInfo.cbSize = sizeof(ShellInfo);
        ShellInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
        ShellInfo.hwnd = NULL;
        ShellInfo.lpVerb = _T("open");
        ShellInfo.lpFile = strCmd;
        ShellInfo.lpParameters = strParam;
        ShellInfo.nShow = SW_HIDE;

        if (ShellExecuteEx(&ShellInfo)) {
            DWORD dwWaitRet = WaitForSingleObject(ShellInfo.hProcess, 5 * 1000);

            if (WAIT_OBJECT_0 == dwWaitRet) {
                dwRet = ERROR_SUCCESS;
            }
        } else {
            dwRet = GetLastError();
        }
    } while (0);

    return dwRet;
}

void KFirewallRuleRegister::_ParseCmd(
    __in LPCTSTR lpctModuleFullPath,
    __in Action enAction,
    __out CString& strCmd,
    __out CString& strParam)
{
    if (!lpctModuleFullPath)
        return;

    CString strFullPath = lpctModuleFullPath,
            strFileName;
    int nPos = strFullPath.ReverseFind(L'\\');
    int nPosSubFix = strFullPath.ReverseFind(L'.');

    if (-1 != nPos) {
        if (-1 != nPosSubFix) {
            strFileName = strFullPath.Mid(nPos + 1, nPosSubFix - nPos - 1);
        } else {
            strFileName = strFullPath.Mid(nPos + 1);
        }
    }

    switch (enAction) {
    case Act_EnableNewCommunication: {
        //netsh firewall add allowedprogram "c:\xxx\yyy.exe" "yyy" enable
        strCmd = L"netsh";
        strParam = L"firewall add allowedprogram \"" +
                   strFullPath + L"\" \"" +
                   strFileName + L"\" enable";
    }
    break;

    default:
        break;
    }
}

}

namespace cactus
{


CCCXunleiDownloader::CCCXunleiDownloader()
    : wrapper_(NULL)
{
    WCHAR szModulePath[MAX_PATH] = {0};
    WCHAR szDllpath[MAX_PATH] = {0};
    WCHAR szThunderExePath[MAX_PATH] = {0};
    ::GetModuleFileName(NULL, szModulePath, MAX_PATH);
    ::PathRemoveFileSpec(szModulePath);
    ::PathCombine(szDllpath, szModulePath, L"xlmodule\\xldl.dll");
    ::PathCombine(szThunderExePath, szModulePath, L"xlmodule\\download\\minithunderplatform.exe");

    if (!_HasReport_Register_FireWallRule()) {
        KFirewallRuleRegister firewall_register;
        firewall_register.EnableNetWorkCommunication(szThunderExePath);
    }

    wrapper_ = new DownWrapper(szDllpath);
}

CCCXunleiDownloader::~CCCXunleiDownloader()
{
    delete wrapper_;
}

BOOL CCCXunleiDownloader::Init()
{
    BOOL bRet = FALSE;
    bRet = wrapper_->Init();

    if (bRet) {
        wrapper_->LimitUploadSpeed(1, 1);
    }

    return bRet;
}

BOOL CCCXunleiDownloader::UnInit()
{
    return wrapper_->UnInit();
}

HANDLE CCCXunleiDownloader::CreateTask(LPCWSTR url, LPCWSTR filename)
{
    // url = L"http://file.mydrivers.com/DG2015Setup_1142B.exe";
    // filename = L"D:\\MyDrivers\\hotfix\\DG2015Setup_1142B.exe";
    CString strFilePath, strFolder, strFileName;
    strFilePath = filename;
    int nPos = strFilePath.ReverseFind(L'\\');
    strFolder = strFilePath.Mid(0, nPos);
    strFileName = strFilePath.Mid(nPos + 1);
    DownTaskParam downTaskParam;
    StrCpyW(downTaskParam.szTaskUrl, url);
    StrCpyW(downTaskParam.szFilename, strFileName);
    StrCpyW(downTaskParam.szSavePath, strFolder);
    HANDLE hTask = INVALID_HANDLE_VALUE;

    do {
        hTask = wrapper_->TaskCreate(downTaskParam);

        if (!hTask) {
            CCLog("%s", "[XLengine] TaskCreate Failed!");
            break;
        }

        wrapper_->LimitUploadSpeed(100, 30);
        wrapper_->TaskStart(hTask);
    } while (0);

    return hTask;
}

BOOL CCCXunleiDownloader::TaskQueryEx(HANDLE hTask, DownTaskInfo& stTaskInfo)
{
    return wrapper_->TaskQueryEx(hTask, stTaskInfo);
}


BOOL CCCXunleiDownloader::DeleteTask(HANDLE hTask, CString saveFolder)
{
    if (wrapper_->TaskPause(hTask) && _DeleteTaskAndTmpFile(hTask, saveFolder)) {
        return TRUE;
    }

    return FALSE;
}

BOOL CCCXunleiDownloader::_DeleteTaskAndTmpFile(HANDLE hTask, CString saveFolder)
{
    DownTaskInfo info;

    if (!wrapper_->TaskQueryEx(hTask, info)) {
        return FALSE;
    }

    if (wrapper_->TaskDelete(hTask)) {
        DownTaskParam deleteParam;
        StrCpyW(deleteParam.szFilename, info.szFilename);
        StrCpyW(deleteParam.szSavePath, (LPCWSTR)saveFolder);
        wrapper_->DelTempFile(deleteParam);
        return TRUE;
    }

    return FALSE;
}

BOOL CCCXunleiDownloader::_HasReport_Register_FireWallRule()
{
    bool bHasProcessed = true;
    CRegKey regKey;

    do {
        LONG lRet = -1;
        DWORD dwValue = 0;
        lRet = regKey.Create(HKEY_LOCAL_MACHINE, _T("Software\\Kingsoft\\antivirus\\sysopt"));

        if (ERROR_SUCCESS != lRet) {
            break;
        }

        lRet = regKey.QueryDWORDValue(L"register_xldownloader", dwValue);

        if (ERROR_FILE_NOT_FOUND == lRet) {
            bHasProcessed = false;
        }
    } while (false);

    //> Mark Processed
    if (!bHasProcessed) {
        regKey.SetDWORDValue(L"register_xldownloader", 1);
    }

    return bHasProcessed;
}


}



namespace network
{
BOOL ExecutePingEx(LPCSTR lpURL, LPCSTR /*lpMatch*/)
{
    char *argv[2] = {"Ping.exe", 0};
    int ret = -1;
    size_t length = strlen(lpURL);
    argv[1] = (char*)malloc(length + 1);

    if (argv[1] != NULL) {
        strncpy(argv[1], lpURL, length + 1);
        ret = ExecutePingXP(2, argv);
        free(argv[1]);
    }

    return (ret > 0 && ret < 5);
}

BOOL CheckNormal()
{
    return (ExecutePingEx("www.baidu.com", "Reply from")
            || ExecutePingEx("www.qq.com", "Reply from"));
}
}




namespace //> in milliseconds
{
const DWORD Callback_Inteval = 500;
const DWORD TaskCancel_WaitTime = 20 * 1000;
const DWORD AsyncInit_WaitTime = 2 * 1000;  //> Wait 3 times
}

namespace cactus
{




CXunleiDownloader CXunleiDownloader::m_Instance;

CXunleiDownloader& CXunleiDownloader::GetInstance()
{
    return m_Instance;
}

CXunleiDownloader::CXunleiDownloader()
    : m_hDownloadThread(NULL)
    , m_hJobEvent(NULL)
    , m_hPauseEvent(NULL)
    , m_hStopEvent(NULL)
    , m_pxlEngineWrapper(NULL)
    , m_bCanInit(TRUE)
    , m_hInitEngineThread(NULL)
    , m_TimeOutCount(0)
    , m_bInitEngine(FALSE)
{
}

CXunleiDownloader::~CXunleiDownloader()
{
}



DWORD CXunleiDownloader::_ProcessTask(DWORD dwTaskHandle, EnumAction enAction)
{
    if (!_CheckInit()) {
//      ZLOG_DEBUG(_T("_ProcessTask _CheckInit() fiald !!!!!!!!!!!!!!!!!"));
        return xlDownloaderError::xlDownErr_Init_Failed;
    }

    DWORD dwRet = -1;

    do {
        const TaskItem* ptaskItem = _GetTaskItem((HANDLE)dwTaskHandle);

        if (!ptaskItem) {
            dwRet = xlDownloaderError::xlDownErr_Task_Not_Exist;
            break;
        }

        switch (enAction) {
        case Act_Pause: {
            {
                CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskPause);
                m_TasksforPause.push((HANDLE)dwTaskHandle);
            }
            _NotifyDownloadThreadPause();
            dwRet = ERROR_SUCCESS;
        }
        break;

        case Act_Resume: {
            {
                CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskStart);
                m_TasksforStart.push((HANDLE)dwTaskHandle);
            }
            _NotifyJob();
            dwRet = ERROR_SUCCESS;
        }
        break;

        case Act_Delete: {
            //SetEvent(ptaskItem->_hFinish);
            if (m_pxlEngineWrapper->TaskPause(ptaskItem->_hTask)
                && _DeleteTaskAndTmpFile(*ptaskItem)) {
                dwRet = ERROR_SUCCESS;
            } else {
                //LOGS << L"[XLengine] TaskDelete Failed!" << enumLogEnd;
            }
        }
        break;

        default:
            break;
        }
    } while (false);

    return dwRet;
}

DWORD CXunleiDownloader::DeleteTask(DWORD dwTaskHandle)
{
    return _ProcessTask(dwTaskHandle, Act_Delete);
}

DWORD CXunleiDownloader::PauseTask(DWORD dwTaskHandle)
{
    return _ProcessTask(dwTaskHandle, Act_Pause);
}

DWORD CXunleiDownloader::ResumeTask(DWORD dwTaskHandle)
{
    return _ProcessTask(dwTaskHandle, Act_Resume);
}

DWORD CXunleiDownloader::CreateTask(
    LPCWSTR lpUrl,
    LPCWSTR lpMd5,
    LPCWSTR lpFilePath,
    __in CallBack* pCallBackObj,
    __out DWORD* pdwTaskHandle)
{
    if (lpUrl == NULL || lpFilePath == NULL) {
        return xlDownloaderError::xlDownErr_Invalid_DownParam;
    }

    if (!_CheckInit()) {
        //ZLOG_DEBUG(_T("CreateTask _CheckInit() fiald !!!!!!!!!!!!!!!!!"));
        return xlDownloaderError::xlDownErr_Init_Failed;
    }

    DWORD dwRet = -1;

    do {
        CString strFilePath, strFolder, strFileName;
        strFilePath = lpFilePath;
        int nPos = strFilePath.ReverseFind(L'\\');
        strFolder = strFilePath.Mid(0, nPos);
        strFileName = strFilePath.Mid(nPos + 1);
        DownTaskParam downTaskParam;
        StrCpyW(downTaskParam.szTaskUrl, lpUrl);
        StrCpyW(downTaskParam.szFilename, strFileName);
        StrCpyW(downTaskParam.szSavePath, strFolder);
        TaskItem taskItem;
        taskItem._hTask = m_pxlEngineWrapper->TaskCreate(downTaskParam);

        if (!taskItem._hTask) {
            //LOGS << L"[XLengine] TaskCreate Failed!" << enumLogEnd;
            break;
        }

        taskItem._strUrl = lpUrl;
        taskItem._strSaveFolder = strFolder;
        taskItem._strFileName = strFileName;
        taskItem._pCallBackObj = pCallBackObj;
        taskItem._strMd5 = lpMd5;
        _DeleteOriginFileIfExists(downTaskParam.szSavePath, downTaskParam.szFilename);

        if (ERROR_SUCCESS == _PushTaskToList(taskItem)) {
            {
                CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskStart);
                m_TasksforStart.push(taskItem._hTask);
            }
            _NotifyJob();
            dwRet = ERROR_SUCCESS;

            if (pdwTaskHandle) {
                *pdwTaskHandle = (DWORD)taskItem._hTask;
            }
        }
    } while (false);

    return dwRet;
}

DWORD  WINAPI  CXunleiDownloader::_ThreadProc(LPVOID lpParam)
{
    CXunleiDownloader* pDownloaderManager = (CXunleiDownloader*)lpParam;
    return pDownloaderManager->_DownloadFunctionImpl();
}

DWORD  WINAPI CXunleiDownloader::_InitXlEngineThread(LPVOID lpParam)
{
    CXunleiDownloader* pDownloaderManager = (CXunleiDownloader*)lpParam;

    if (pDownloaderManager->m_pxlEngineWrapper
        && pDownloaderManager->m_pxlEngineWrapper->Init()) {
        pDownloaderManager->m_pxlEngineWrapper->LimitUploadSpeed(1, 1);
        pDownloaderManager->m_bInitEngine = TRUE;
    }

    return ERROR_SUCCESS;
}

BOOL CXunleiDownloader::_CheckInit()
{
    if (m_bInitEngine) {
        m_bCanInit = FALSE;
        return TRUE;
    }

    if (!m_bCanInit || m_TimeOutCount == 3) {
        return FALSE;
    }

    if (m_hInitEngineThread) {
        DWORD dwRet = WaitForSingleObject(m_hInitEngineThread, AsyncInit_WaitTime);

        if (WAIT_OBJECT_0 == dwRet &&
            FALSE == m_bInitEngine) {
            m_bCanInit = FALSE;
        }
    } else {
        m_bCanInit = FALSE;
    }

    if (!m_bInitEngine) {
        ++m_TimeOutCount;
    }

    return m_bInitEngine;
}

DWORD CXunleiDownloader::_DownloadFunctionImpl()
{
    if (m_hStopEvent == NULL || m_hJobEvent == NULL || m_hPauseEvent == NULL) {
        return xlDownloaderError::xlDownErr_Init_Failed;
    }

    HANDLE hEvents[3];
    hEvents[0] = m_hStopEvent;
    hEvents[1] = m_hPauseEvent;
    hEvents[2] = m_hJobEvent;
    DWORD dwWaitResult = WAIT_OBJECT_0;

    do {
        dwWaitResult = WaitForMultipleObjects(3, hEvents, FALSE, INFINITE);

        switch (dwWaitResult) {
        case WAIT_OBJECT_0: {
            _CancelUnfinishedTasks();
            return ERROR_SUCCESS;
        }
        break;

        case WAIT_OBJECT_0 + 1: {
            _PauseDownloadingTasks();
        }
        break;

        case WAIT_OBJECT_0 + 2: {
            _StartReadyTasks();
        }
        break;

        default:
            return ERROR_SUCCESS;
        }

        //> callback in every "Callback_Inteval" if any task is downloading
        _ReportTasksStatus();
        Sleep(Callback_Inteval);
    } while (TRUE);

    return ERROR_SUCCESS;
}

VOID CXunleiDownloader::_NotifyJob()
{
    if (m_hJobEvent) {
        SetEvent(m_hJobEvent);
    }
}

VOID CXunleiDownloader::_NotifyDownloadThreadPause()
{
    if (m_hPauseEvent) {
        SetEvent(m_hPauseEvent);
    }
}

VOID CXunleiDownloader::_NotifyDownloadThreadStop()
{
    if (m_hStopEvent) {
        SetEvent(m_hStopEvent);
    }
}

VOID CXunleiDownloader::_WaitDownloadThreadStop()
{
    if (m_hDownloadThread) {
        WaitForSingleObject(m_hDownloadThread, INFINITE);
    }
}

DownWrapper* CXunleiDownloader::_LoadxlEngine(LPCWSTR lpcwDllpath)
{
    return new DownWrapper(lpcwDllpath);
}

DWORD CXunleiDownloader::_PushTaskToList(const TaskItem& downloadItem)
{
    CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskJobs);
    m_mapTasks.insert(std::make_pair(downloadItem._hTask, downloadItem));
    return ERROR_SUCCESS;
}

VOID CXunleiDownloader::_PauseDownloadingTasks()
{
    CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskPause);

    while (!m_TasksforPause.empty()) {
        HANDLE taskHandle = m_TasksforPause.front();

        if (NULL != taskHandle) {
            DownTaskInfo info;
            m_pxlEngineWrapper->TaskQueryEx(taskHandle, info);

            if (info.stat == TSC_DOWNLOAD) {
                m_pxlEngineWrapper->TaskPause(taskHandle);
            }
        }

        m_TasksforPause.pop();
    }

    ::ResetEvent(m_hPauseEvent);
}

VOID CXunleiDownloader::_StartReadyTasks()
{
    CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskStart);

    while (!m_TasksforStart.empty()) {
        HANDLE taskHandle = m_TasksforStart.front();

        if (NULL != taskHandle) {
            m_pxlEngineWrapper->LimitUploadSpeed(100, 30);
            m_pxlEngineWrapper->TaskStart(taskHandle);
        }

        m_TasksforStart.pop();
    }
}

struct CallBackInfo {
    CXunleiDownloader::CallBack* _pCallBack;
    DTManager_Stat  _dtStat;
    DOWN_TASK_STATUS stat;
    TASK_ERROR_TYPE fail_code;
    __int64     nTotalSize;
    __int64     nTotalDownload;
    int         nSpeed;
    int         nSpeedP2S;
    int         nSpeedP2P;
    float       fPercent;

    CallBackInfo(DownTaskInfo& info, DTManager_Stat dtstat, CXunleiDownloader::CallBack* pcallback)
        : _pCallBack(pcallback)
        , _dtStat(dtstat)
        , stat(info.stat)
        , fail_code(info.fail_code)
        , nTotalSize(info.nTotalSize)
        , nTotalDownload(info.nTotalDownload)
        , nSpeed(info.nSpeed)
        , nSpeedP2S(info.nSpeedP2S)
        , nSpeedP2P(info.nSpeedP2P)
        , fPercent(info.fPercent)
    {
    }
};

/*
 * @remark : (14-5-12)把回调动作拆分为两步：先取信息、再统一回调。因为回调中有锁，可能会造成互锁
 */
VOID CXunleiDownloader::_ReportTasksStatus()
{
    BOOL bDownloading = FALSE;
    std::map<HANDLE, CallBackInfo> mapCallBackInfo;
    {
        CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskJobs);
        std::vector<HANDLE> needEraseTask;

        for (std::map<HANDLE, TaskItem>::iterator it = m_mapTasks.begin(); it != m_mapTasks.end(); ++it) {
            TaskItem& taskItem_refer = it->second;

            if (!taskItem_refer._hTask) {
                continue;
            }

            DownTaskInfo info;
            m_pxlEngineWrapper->TaskQueryEx(taskItem_refer._hTask, info);

            switch (info.stat) {
            case TSC_DOWNLOAD:
            case TSC_STARTPENDING: {
                if (!taskItem_refer._startTime) {
                    taskItem_refer._startTime = GetTickCount();
                } else {
                    taskItem_refer._endTime = GetTickCount();
                }

                if (taskItem_refer._endTime - taskItem_refer._startTime >= 30 * 1000 && info.fPercent == 0) {
                    if (network::CheckNormal()) { // 网络正常，说明p2p被封掉了
                        //KSoftMgrCfg::Instance().SetP2pDownState(FALSE);
                        //KInfocClientWrapDef(infoc);
                        //infoc.SetTableName(L"duba_softmgr_xunlei");
                        //infoc.AddInt(L"xunlei", 1);
                        //infoc.AddInt(L"version", 1);
                        //infoc.WriteCache();
                    } else {
                        // 网络无法连通，可能是断网
                    }

                    ::OutputDebugString(TEXT("下载超时！！！！！！！！！！！"));
                    DTManager_Stat nStat = TASK_XL_TIMEOUT;
                    info.stat = TSC_ERROR;
                    mapCallBackInfo.insert(std::make_pair(taskItem_refer._hTask, CallBackInfo(info, nStat, taskItem_refer._pCallBackObj)));

                    if (_DeleteTaskAndTmpFile(taskItem_refer)) {
                        needEraseTask.push_back(it->first);
                    }

                    break;
                }

                bDownloading = TRUE;
                mapCallBackInfo.insert(std::make_pair(taskItem_refer._hTask,  CallBackInfo(info, TASK_DOWNING, taskItem_refer._pCallBackObj)));
                break;
            }

            case TSC_PAUSE: {
                taskItem_refer._startTime = 0;
                taskItem_refer._endTime = 0;
                break;
            }

            case TSC_COMPLETE: {
                DTManager_Stat nStat = TASK_DONE;
                /*
                if (!_CheckMd5(taskItem_refer)) {
                    nStat = TASK_ERROR_MD5_2;
                }*/
                ::OutputDebugStringW(L"下载完成...");
                mapCallBackInfo.insert(std::make_pair(taskItem_refer._hTask, CallBackInfo(info, nStat, taskItem_refer._pCallBackObj)));
                needEraseTask.push_back(it->first);
                break;
            }

            case TSC_ERROR: {
                DTManager_Stat nStat = TASK_ERROR;

                if (TASK_ERROR_TIMEOUT == info.fail_code) {
                    nStat = TASK_XL_TIMEOUT;
                }

                mapCallBackInfo.insert(std::make_pair(taskItem_refer._hTask, CallBackInfo(info, nStat, taskItem_refer._pCallBackObj)));

                if (_DeleteTaskAndTmpFile(taskItem_refer)) {
                    needEraseTask.push_back(it->first);
                }

                break;
            }

            default:
                break;
            }
        }

        for (std::vector<HANDLE>::iterator itEraseTask =  needEraseTask.begin();
             itEraseTask != needEraseTask.end();
             itEraseTask++) {
            std::map<HANDLE, TaskItem>::iterator itTask = m_mapTasks.find(*itEraseTask);

            if (itTask != m_mapTasks.end()) {
                m_mapTasks.erase(itTask);
            }
        }
    }

    if (!bDownloading) {
        ResetEvent(m_hJobEvent);
        m_pxlEngineWrapper->LimitUploadSpeed(1, 1);
    }

    //> CallBack
    for (std::map<HANDLE, CallBackInfo>::iterator itcb = mapCallBackInfo.begin();
         itcb != mapCallBackInfo.end();
         itcb++) {
        CallBackInfo& info_refer = itcb->second;

        if (!itcb->first || !info_refer._pCallBack) {
            continue;
        }

        switch (info_refer.stat) {
        case TSC_DOWNLOAD:
        case TSC_STARTPENDING: {
            CString output;
            //output.Format(L"%d/%d", info_refer.nTotalSize, info_refer.nTotalDownload);
            //::MessageBoxW(NULL, output, L"test", MB_OK);
            info_refer._pCallBack->OnDownloadProgress(
                (DWORD)itcb->first,
                info_refer.nTotalSize,
                info_refer.nTotalDownload,
                info_refer.nSpeed,
                info_refer.nSpeedP2S + info_refer.nSpeedP2P,
                info_refer.fPercent
            );
            break;
        }

        case TSC_COMPLETE: {
            info_refer._pCallBack->OnDownloadProgress(
                (DWORD)itcb->first,
                info_refer.nTotalSize,
                info_refer.nTotalDownload,
                info_refer.nSpeed,
                info_refer.nSpeedP2S + info_refer.nSpeedP2P,
                info_refer.fPercent
            );
            info_refer._pCallBack->OnDownloadComplete(
                (DWORD)itcb->first,
                info_refer._dtStat
            );
        }
        break;

        case TSC_ERROR: {
            info_refer._pCallBack->OnDownloadComplete(
                (DWORD)itcb->first,
                info_refer._dtStat
            );
        }

        default:
            break;
        }
    }
}

VOID CXunleiDownloader::_DeleteOriginFileIfExists(const std::wstring& strFolder, const std::wstring& strFileName)
{
    std::wstring wstrFile = strFolder;
    wstrFile.append(L"\\");
    wstrFile.append(strFileName);
    DWORD dwAttr = GetFileAttributesW(wstrFile.c_str());

    if (dwAttr != INVALID_FILE_ATTRIBUTES) {
        DeleteFileW(wstrFile.c_str());
    }
}

BOOL CXunleiDownloader::StartInitailize()
{
    if (!m_bCanInit) return TRUE;

    CComCritSecLock<CComAutoCriticalSection> stLock(m_csInit);
    WCHAR szModulePath[MAX_PATH] = {0};
    WCHAR szDllpath[MAX_PATH] = {0};
    WCHAR szThunderExePath[MAX_PATH] = {0};
    ::GetModuleFileName(NULL, szModulePath, MAX_PATH);
    ::PathRemoveFileSpec(szModulePath);
    ::PathCombine(szDllpath, szModulePath, L"xlmodule\\xldl.dll");
    ::PathCombine(szThunderExePath, szModulePath, L"xlmodule\\download\\minithunderplatform.exe");
    std::wstring strXLModulePath = szModulePath;
    strXLModulePath.append(L"xlmodule\\download\\");
    _PreparexlModules(strXLModulePath);

    if (!_HasReport_Register_FireWallRule()) {
        DWORD dwRet = m_filewallRuleRegister.EnableNetWorkCommunication(szThunderExePath);

        if (ERROR_SUCCESS != dwRet) {
            //LOGS << L"[XLengine] Register FireWall Rule Failed !" << enumLogEnd;
            return FALSE;
        }
    }

    if (m_hPauseEvent) {
        CloseHandle(m_hPauseEvent);
    }

    m_hPauseEvent = CreateEvent(NULL, TRUE, FALSE, L"");

    if (m_hJobEvent) {
        CloseHandle(m_hJobEvent);
    }

    m_hJobEvent = CreateEvent(NULL, TRUE, FALSE, L"");

    if (m_hStopEvent) {
        CloseHandle(m_hStopEvent);
    }

    m_hStopEvent = CreateEvent(NULL, TRUE, FALSE, L"");

    if (m_hJobEvent && m_hStopEvent && m_hPauseEvent
        && !m_pxlEngineWrapper) {
        m_pxlEngineWrapper = _LoadxlEngine(szDllpath);

        if (!m_pxlEngineWrapper) {
            //LOGS << L"[XLengine] LoadxlEngine Failed!" << enumLogEnd;
            return FALSE;
        }
    }

    if (m_pxlEngineWrapper && m_pxlEngineWrapper->Init()) {
        m_pxlEngineWrapper->LimitUploadSpeed(1, 1);
        m_bInitEngine = TRUE;
    }

    //if (!m_hInitEngineThread)
    //{
    //  m_hInitEngineThread = CreateThread(NULL,0,KxlDownloader::_InitXlEngineThread,(LPVOID)this,0,0);
    //  if (!m_hInitEngineThread)
    //  {
    //      LOGS << L"[XLengine] Create InitEngineThread Failed!" << enumLogEnd;
    //      return FALSE;
    //  }
    //}
    if (!m_hDownloadThread) {
        m_hDownloadThread = CreateThread(NULL, 0, CXunleiDownloader::_ThreadProc, (LPVOID)this, 0, 0);

        if (!m_hDownloadThread) {
            //LOGS << L"[XLengine] Create DownloadThread Failed!" << enumLogEnd;
            return FALSE;
        }
    }

    return TRUE;
}

void CXunleiDownloader::Clear()
{
    /*
    if (m_hInitEngineThread) {
        // 让系统自动退出
        // TerminateThread(m_hInitThread, 0);
        WaitForSingleObject(m_hInitEngineThread, 10 * 1000);
        CloseHandle(m_hInitEngineThread);
        m_hInitEngineThread = NULL;
    }
    */
    if (m_hDownloadThread) {
        _NotifyDownloadThreadStop();
        _WaitDownloadThreadStop();
        CloseHandle(m_hDownloadThread);
        m_hDownloadThread = NULL;
    }

    if (m_hJobEvent) {
        CloseHandle(m_hJobEvent);
        m_hJobEvent = NULL;
    }

    if (m_hStopEvent) {
        CloseHandle(m_hStopEvent);
        m_hStopEvent = NULL;
    }

    if (m_hPauseEvent) {
        CloseHandle(m_hPauseEvent);
        m_hPauseEvent = NULL;
    }

    //if (m_pxlEngineWrapper)
    //{
    //  _WaitAllTasksCanceled(TaskCancel_WaitTime);
    //  if(!m_pxlEngineWrapper->UnInit())
    //  {
    //      //LOGS << L"[XLengine] UnInit Failed!" << enumLogEnd;
    //  }
    //  delete m_pxlEngineWrapper;
    //  m_pxlEngineWrapper = NULL;
    //}
    _TerminateXLProcess();
    m_bInitEngine = FALSE;
    m_bCanInit = TRUE;
}

VOID CXunleiDownloader::_CancelUnfinishedTasks()
{
    CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskJobs);

    for (std::map<HANDLE, TaskItem>::iterator it = m_mapTasks.begin(); it != m_mapTasks.end(); ++it) {
        TaskItem& task_refer = it->second;

        if (NULL == task_refer._hTask) {
            continue;
        }

        if (!m_pxlEngineWrapper->TaskPause(task_refer._hTask)) {
            //LOGS << L"[XLengine] TaskPause Failed!" << enumLogEnd;
        }
    }

    ::ResetEvent(m_hStopEvent);
}

BOOL CXunleiDownloader::_DeleteTaskAndTmpFile(const TaskItem& task)
{
    DownTaskInfo info;

    if (!m_pxlEngineWrapper->TaskQueryEx(task._hTask, info)) {
        return FALSE;
    }

    if (m_pxlEngineWrapper->TaskDelete(task._hTask)) {
        DownTaskParam deleteParam;
        StrCpyW(deleteParam.szFilename, info.szFilename);
        StrCpyW(deleteParam.szSavePath, task._strSaveFolder);
        m_pxlEngineWrapper->DelTempFile(deleteParam);
        return TRUE;
    }

    return FALSE;
}

VOID CXunleiDownloader::_WaitAllTasksCanceled(DWORD dwWaitMs /*= INFINITE*/)
{
    DWORD dwStartTickCount = GetTickCount();
    DWORD dwTickCount = 0;
    BOOL bShouldWait = FALSE;
    CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskJobs);

    do {
        bShouldWait = FALSE;

        for (std::map<HANDLE, TaskItem>::iterator it = m_mapTasks.begin(); it != m_mapTasks.end(); ++it) {
            TaskItem& task_refer = it->second;

            if (NULL == task_refer._hTask) {
                continue;
            }

            DownTaskInfo info;

            if (!m_pxlEngineWrapper->TaskQueryEx(task_refer._hTask, info)) {
                continue;
            }

            if (info.stat == TSC_DOWNLOAD) {
                bShouldWait = TRUE;
                break;
            }
        }

        dwTickCount = GetTickCount() - dwStartTickCount;
    } while (bShouldWait && dwTickCount < dwWaitMs);

    if (bShouldWait) {
        //LOGS << L"[XLengine] Not All Task Paused Before Exit!" << enumLogEnd;
    }
}

/*
BOOL CXunleiDownloader::_CheckMd5(const TaskItem& taskItem_refer)
{

    CString strCheckSum;
    BOOL bMatch = FALSE;

    do {
        try {
            KMD5 MD5Calc;
            CString strFilePath = taskItem_refer._strSaveFolder + L"\\" + taskItem_refer._strFileName;
            strCheckSum = MD5Calc.GetMD5Str(strFilePath);
            strCheckSum.MakeUpper();
        } catch (...) {
        };

        // 跳过MD5 验证
        if (0 == taskItem_refer._strMd5.CompareNoCase(strCheckSum) || taskItem_refer._strMd5.IsEmpty()) {
            bMatch = TRUE;
        }
    } while (false);

    return bMatch;

}
*/

const CXunleiDownloader::TaskItem* CXunleiDownloader::_GetTaskItem(HANDLE TaskHandle)
{
    CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskJobs);
    std::map<HANDLE, TaskItem>::iterator it = m_mapTasks.find((HANDLE)TaskHandle);

    if (m_mapTasks.end() != it) {
        return &it->second;
    }

    return NULL;
}

BOOL CXunleiDownloader::_HasReport_Register_FireWallRule()
{
    bool                bHasProcessed = true;
    CRegKey             regKey;

    do {
        LONG        lRet    = -1;
        DWORD       dwValue = 0;
        lRet = regKey.Create(HKEY_LOCAL_MACHINE, _T("Software\\Kingsoft\\antivirus\\sysopt"));

        if (ERROR_SUCCESS != lRet) {
            break;
        }

        lRet = regKey.QueryDWORDValue(L"register_xldownloader", dwValue);

        if (ERROR_FILE_NOT_FOUND == lRet) {
            bHasProcessed = false;
        }
    } while (false);

    //> Mark Processed
    if (!bHasProcessed) {
        regKey.SetDWORDValue(L"register_xldownloader", 1);
    }

    return bHasProcessed;
}

BOOL CXunleiDownloader::_PreparexlModules(const std::wstring& strxlModulePath)
{
    std::wstring strzlibSrcPath = strxlModulePath;
    strzlibSrcPath.append(L"xlzlib1.dll");
    std::wstring strzlibDstPath = strxlModulePath;
    strzlibDstPath.append(L"zlib1.dll");
    CopyFileW(strzlibSrcPath.c_str(), strzlibDstPath.c_str(), FALSE);
    return TRUE;
}

void CXunleiDownloader::_TerminateXLProcess()
{
    HANDLE hXLProc = NULL;
    CString XLProcPath;

    do {
        _CheckXLProcessExist(&hXLProc, XLProcPath);

        if (!hXLProc || XLProcPath.IsEmpty()) {
            break;
        }

        wchar_t szPath[MAX_PATH + 1] = {0};
        GetModuleFileNameW(NULL, szPath, MAX_PATH);
        PathRemoveFileSpecW(szPath);
        CString xlPath = szPath;
        xlPath.Append(_T("\\xlmodule\\download\\minithunderplatform.exe"));

        if (0 != XLProcPath.CompareNoCase(xlPath)) {
            break;
        }

        TerminateProcess(hXLProc, 0);
    } while (false);

    if (hXLProc) {
        CloseHandle(hXLProc);
        hXLProc = NULL;
    }
}

void CXunleiDownloader::_CheckXLProcessExist(PHANDLE pProc, CString& XLPath)
{
    const std::wstring  strProcName     = L"minithunderplatform.exe";
    BOOL                bExist          = TRUE;
    PROCESSENTRY32W     pe32            = {sizeof(PROCESSENTRY32W)};
    HANDLE              hProcess        = NULL;
    HANDLE hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

    if (hProcessSnap == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (!Process32FirstW(hProcessSnap, &pe32)) {
            break;
        }

        do {
            /* Match process name */
            if (_wcsnicmp(pe32.szExeFile,  strProcName.c_str(), strProcName.size()) == 0) {
                bExist = TRUE;
                break;
            }
        } while (Process32NextW(hProcessSnap, &pe32));
    } while (false);

    if (hProcessSnap) {
        CloseHandle(hProcessSnap);
        hProcessSnap = NULL;
    }

    if (bExist) {
        hProcess = OpenProcess(
                       PROCESS_QUERY_INFORMATION | PROCESS_VM_READ | PROCESS_TERMINATE,
                       FALSE,
                       pe32.th32ProcessID);

        /* Get the process name. */
        if (NULL != hProcess && pProc) {
            *pProc = hProcess;
            WCHAR szPath[MAX_PATH + 1] = {0};

            if (0 != GetModuleFileNameEx(hProcess, NULL, szPath, MAX_PATH)) {
                XLPath = szPath;
            }
        }
    }
}



void CXunleiDownloader::DelAllTasks()
{
    CComCritSecLock<CComAutoCriticalSection> stLock(m_csTaskJobs);

    for (std::map<HANDLE, TaskItem>::iterator it = m_mapTasks.begin(); it != m_mapTasks.end(); ++it) {
        TaskItem& task_refer = it->second;
        this->DeleteTask((DWORD)task_refer._hTask);
    }

    m_mapTasks.clear();
}

BOOL CXunleiDownloader::TaskQueryEx(HANDLE hTask, DownTaskInfo& stTaskInfo)
{
    m_pxlEngineWrapper->TaskQueryEx(hTask, stTaskInfo);
    return TRUE;
}

} // namespace cactus

