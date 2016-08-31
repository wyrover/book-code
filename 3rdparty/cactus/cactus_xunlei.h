#ifndef __CACTUS_XUNLEI_H__
#define __CACTUS_XUNLEI_H__

#include <cactus/cactus.h>
#include <cactus/cactus_types.h>
#include <atlcore.h>
#include <atlcomcli.h>
#include <atlstr.h>
#include <queue>
#include <boost/thread.hpp>
#include "kscbase/ksclock.h"

#pragma pack(push, 1)
struct DownTaskParam
{
    DownTaskParam()
    {
        memset(this, 0, sizeof(DownTaskParam));
        nReserved1		 = 5;
        bReserved			 = FALSE;
        DisableAutoRename	 = FALSE;
        IsOnlyOriginal		 = FALSE;
        IsResume			 = TRUE;
    }
    int nReserved;
    wchar_t szTaskUrl[2084];          // 任务URL
    wchar_t szRefUrl[2084];           // 引用页
    wchar_t szCookies[4096];          // 浏览器cookie
    wchar_t szFilename[MAX_PATH];     // 下载保存文件名.
    wchar_t szReserved0[MAX_PATH];
    wchar_t szSavePath[MAX_PATH];     // 文件保存目录
    HWND  hReserved;
    BOOL bReserved; 
    wchar_t szReserved1[64];
    wchar_t szReserved2[64];
    BOOL IsOnlyOriginal;            // 是否只从原始地址下载
    UINT nReserved1;
    BOOL DisableAutoRename;         // 禁止智能命名
    BOOL IsResume;                  // 是否用续传
    DWORD reserved[2048];
};

enum  DOWN_TASK_STATUS
{
    NOITEM = 0,
    TSC_ERROR,
    TSC_PAUSE,
    TSC_DOWNLOAD,
    TSC_COMPLETE,
    TSC_STARTPENDING,
    TSC_STOPPENDING
};

enum TASK_ERROR_TYPE
{
    TASK_ERROR_UNKNOWN	   =			0x00,   // 未知错误
    TASK_ERROR_DISK_CREATE =			0x01,   // 创建文件失败
    TASK_ERROR_DISK_WRITE =				0x02,   // 写文件失败
    TASK_ERROR_DISK_READ =				0x03,   // 读文件失败
    TASK_ERROR_DISK_RENAME =			0x04,   // 重命名失败
    TASK_ERROR_DISK_PIECEHASH =			0x05,   // 文件片校验失败
    TASK_ERROR_DISK_FILEHASH =			0x06,   // 文件全文校验失败
    TASK_ERROR_DISK_DELETE =			0x07,   // 删除文件失败失败
    TASK_ERROR_DOWN_INVALID =			0x10,   // 无效的DOWN地址
    TASK_ERROR_PROXY_AUTH_TYPE_UNKOWN = 0x20,   // 代理类型未知
    TASK_ERROR_PROXY_AUTH_TYPE_FAILED = 0x21,   // 代理认证失败
    TASK_ERROR_HTTPMGR_NOT_IP =			0x30,   // http下载中无ip可用
    TASK_ERROR_TIMEOUT =				0x40,   // 任务超时
    TASK_ERROR_CANCEL =					0x41,   // 任务取消
    TASK_ERROR_TP_CRASHED=              0x42,   // MINITP崩溃
    TASK_ERROR_ID_INVALID =             0x43,   // TaskId 非法
};

struct DownTaskInfo
{
    DownTaskInfo()
    {
        memset(this, 0, sizeof(DownTaskInfo));
        stat		= TSC_PAUSE;
        fail_code	= TASK_ERROR_UNKNOWN;
        fPercent = 0;
        bIsOriginUsable = false;
        fHashPercent = 0;
    }
    DOWN_TASK_STATUS	stat;
    TASK_ERROR_TYPE		fail_code;
    wchar_t		szFilename[MAX_PATH];
    wchar_t		szReserved0[MAX_PATH];
    __int64     nTotalSize;         // 该任务总大小(字节)
    __int64     nTotalDownload;     // 下载有效字节数(可能存在回退的情况)
    float		fPercent;           // 下载进度
    int			nReserved0;
    int			nSrcTotal;          // 总资源数
    int			nSrcUsing;          // 可用资源数
    int			nReserved1;
    int			nReserved2;
    int			nReserved3;
    int			nReserved4;
    __int64     nReserved5;
    __int64		nDonationP2P;       // p2p贡献字节数
    __int64		nReserved6;
    __int64		nDonationOrgin;		// 原始资源共享字节数
    __int64		nDonationP2S;		// 镜像资源共享字节数
    __int64		nReserved7;
    __int64     nReserved8;
    int			nSpeed;             // 即时速度(字节/秒)
    int			nSpeedP2S;          // 即时速度(字节/秒)
    int			nSpeedP2P;          // 即时速度(字节/秒)
    bool		bIsOriginUsable;    // 原始资源是否有效
    float		fHashPercent;       // 现不提供该值
    int			IsCreatingFile;     // 是否正在创建文件
    DWORD		reserved[64];
};

enum DOWN_PROXY_TYPE
{
    PROXY_TYPE_IE	 = 0,
    PROXY_TYPE_HTTP  = 1,
    PROXY_TYPE_SOCK4 = 2,
    PROXY_TYPE_SOCK5 = 3,
    PROXY_TYPE_FTP   = 4,
    PROXY_TYPE_UNKOWN  = 255,
};

enum DOWN_PROXY_AUTH_TYPE
{
    PROXY_AUTH_NONE =0,
    PROXY_AUTH_AUTO,
    PROXY_AUTH_BASE64,
    PROXY_AUTH_NTLM,
    PROXY_AUTH_DEGEST,
    PROXY_AUTH_UNKOWN,
};

struct DOWN_PROXY_INFO
{
    BOOL		bIEProxy;
    BOOL		bProxy;
    DOWN_PROXY_TYPE	stPType;
    DOWN_PROXY_AUTH_TYPE	stAType;
    wchar_t		szHost[2048];
    INT32		nPort;
    wchar_t		szUser[50];
    wchar_t		szPwd[50];
    wchar_t		szDomain[2048];
};


//struct WSAPROTOCOL_INFOW;


#pragma pack(pop)

namespace cactus
{
    extern "C" __declspec(dllimport) BOOL   XL_Init(void);
    extern "C" __declspec(dllimport) BOOL   XL_UnInit(void);
    extern "C" __declspec(dllimport) HANDLE XL_CreateTask(DownTaskParam &stParam);
    extern "C" __declspec(dllimport) BOOL   XL_DeleteTask(HANDLE hTask);
    extern "C" __declspec(dllimport) BOOL   XL_StartTask(HANDLE hTask);
    extern "C" __declspec(dllimport) BOOL   XL_StopTask(HANDLE hTask);
    extern "C" __declspec(dllimport) BOOL   XL_QueryTaskInfo(HANDLE hTask, DownTaskInfo & stTaskInfo); //旧版接口，使用Ex接口替换
    extern "C" __declspec(dllimport) BOOL   XL_QueryTaskInfoEx(HANDLE hTask, DownTaskInfo & stTaskInfo);
    extern "C" __declspec(dllimport) BOOL	XL_DelTempFile(DownTaskParam &stParam);
    extern "C" __declspec(dllimport) void	XL_SetSpeedLimit(INT32 nKBps);
    extern "C" __declspec(dllimport) void	XL_SetUploadSpeedLimit(INT32 nTcpKBps,INT32 nOtherKBps);
    extern "C" __declspec(dllimport) BOOL	XL_SetProxy(DOWN_PROXY_INFO &stProxyInfo);
    extern "C" __declspec(dllimport) void   XL_SetUserAgent(const wchar_t *pszUserAgent);
    extern "C" __declspec(dllimport) BOOL   XL_ParseThunderPrivateUrl(const wchar_t *pszThunderUrl, wchar_t *normalUrlBuffer, INT32 bufferLen);
    extern "C" __declspec(dllimport) BOOL   XL_GetFileSizeWithUrl(const wchar_t * lpURL, INT64& iFileSize);
    extern "C" __declspec(dllimport) BOOL   XL_SetFileIdAndSize(HANDLE hTask, char szFileId[40], unsigned __int64 nFileSize);
    extern "C" __declspec(dllimport) BOOL   XL_SetAdditionInfo( HANDLE task_id, WSAPROTOCOL_INFOW *sock_info, CHAR *http_resp_buf, LONG buf_len );

}

namespace cactus
{
    // 定义函数指针
    typedef BOOL (*fn_Init)(void);
    typedef BOOL (*fn_UnInit)(void);
    typedef HANDLE(*fn_TaskCreate)(DownTaskParam &stParam);
    typedef BOOL (*fn_TaskDelete)(HANDLE hTask);
    typedef BOOL (*fn_TaskStart)(HANDLE hTask);
    typedef BOOL (*fn_TaskPause)(HANDLE hTask);
    typedef BOOL (*fn_TaskQuery)(HANDLE hTask, DownTaskInfo &stTaskInfo);
    typedef BOOL (*fn_TaskQueryEx)(HANDLE hTask, DownTaskInfo &stTaskInfo);
    typedef void (*fn_LimitSpeed)(INT32 nKBps);
    typedef void (*fn_LimitUploadSpeed)(INT32 nTcpBps, INT32 nOtherBps);
    typedef BOOL (*fn_DelTempFile)(DownTaskParam &stParam);
    typedef BOOL (*fn_SetProxy)(DOWN_PROXY_INFO &stProxy);
    typedef void (*fn_SetUserAgent)(const TCHAR* pszUserAgent);
    typedef BOOL (*fn_GetFileSizeWithUrl)(const wchar_t * lpURL, INT64& iFileSize);
    typedef BOOL (*fn_ParseThunderPrivateUrl)(const TCHAR *pszThunderUrl, TCHAR *normalUrlBuffer, INT32 bufferLen);
    typedef LONG(*fn_SetAdditionInfo)(HANDLE task_id, WSAPROTOCOL_INFOW *sock_info, CHAR *http_resp_buf, LONG buf_len);
    typedef BOOL (*fn_SetFileIdAndSize)(HANDLE hTask, char szFileId[40], unsigned __int64 nFileSize);
    typedef BOOL (*fn_TaskForceStop)(HANDLE hTask);

#define CHECKFUNC(f, ret) if (f == NULL) return ret;
#define CHECKFUNC_(f) if (f == NULL) return;

    class DownWrapper
    {
    public:

        DownWrapper(LPCWSTR sPath)
            : _Init(NULL), _UnInit(NULL), _TaskCreate(NULL), _TaskStart(NULL), _TaskPause(NULL), _TaskQuery(NULL), _TaskQueryEx(NULL)
            , _LimitSpeed(NULL), _LimitUploadSpeed(NULL), _DelTempFile(NULL), _SetProxy(NULL), _SetUserAgent(NULL)
            , _GetFileSizeWithUrl(NULL), _ParseThunderPrivateUrl(NULL), _SetAdditionInfo(NULL), _SetFileIdAndSize(NULL)
        {
            m_hModule = ::LoadLibraryW(sPath);

            if (m_hModule == NULL) {
                return;
            }

            _Init                   = (fn_Init)                     ::GetProcAddress(m_hModule, "XL_Init");
            _UnInit                 = (fn_UnInit)                   ::GetProcAddress(m_hModule, "XL_UnInit");
            _TaskCreate             = (fn_TaskCreate)               ::GetProcAddress(m_hModule, "XL_CreateTask");
            _TaskDelete             = (fn_TaskDelete)               ::GetProcAddress(m_hModule, "XL_DeleteTask");
            _TaskStart              = (fn_TaskStart)                ::GetProcAddress(m_hModule, "XL_StartTask");
            _TaskPause              = (fn_TaskPause)                ::GetProcAddress(m_hModule, "XL_StopTask");
            _TaskQuery              = (fn_TaskQuery)                ::GetProcAddress(m_hModule, "XL_QueryTaskInfo");
            _TaskQueryEx            = (fn_TaskQueryEx)              ::GetProcAddress(m_hModule, "XL_QueryTaskInfoEx");
            _LimitSpeed             = (fn_LimitSpeed)               ::GetProcAddress(m_hModule, "XL_SetSpeedLimit");
            _LimitUploadSpeed       = (fn_LimitUploadSpeed)         ::GetProcAddress(m_hModule, "XL_SetUploadSpeedLimit");
            _DelTempFile            = (fn_DelTempFile)              ::GetProcAddress(m_hModule, "XL_DelTempFile");
            _SetProxy               = (fn_SetProxy)                 ::GetProcAddress(m_hModule, "XL_SetProxy");
            _SetUserAgent           = (fn_SetUserAgent)             ::GetProcAddress(m_hModule, "XL_SetUserAgent");
            _GetFileSizeWithUrl     = (fn_GetFileSizeWithUrl)       ::GetProcAddress(m_hModule, "XL_GetFileSizeWithUrl");
            _ParseThunderPrivateUrl = (fn_ParseThunderPrivateUrl)   ::GetProcAddress(m_hModule, "XL_ParseThunderPrivateUrl");
            _SetAdditionInfo        = (fn_SetAdditionInfo)          ::GetProcAddress(m_hModule, "XL_SetAdditionInfo");
            _SetFileIdAndSize       = (fn_SetFileIdAndSize)         ::GetProcAddress(m_hModule, "XL_SetFileIdAndSize");
            _TaskForceStop          = (fn_TaskForceStop)            ::GetProcAddress(m_hModule, "XL_ForceStopTask");
        }

        virtual ~DownWrapper(void)
        {
            if (m_hModule) {
                ::FreeLibrary(m_hModule);
            }
        }

        BOOL Init()
        {
            CHECKFUNC(_Init, FALSE);
            return _Init();
        }
        BOOL UnInit()
        {
            CHECKFUNC(_UnInit, FALSE);
            return _UnInit();
        }
        HANDLE TaskCreate(DownTaskParam &param)
        {
            CHECKFUNC(_TaskCreate, NULL);
            return _TaskCreate(param);
        }
        BOOL TaskDelete(HANDLE hTask)
        {
            CHECKFUNC(_TaskDelete, FALSE);
            return _TaskDelete(hTask);
        }
        BOOL TaskStart(HANDLE hTask)
        {
            CHECKFUNC(_TaskStart, FALSE);
            return _TaskStart(hTask);
        }
        BOOL TaskPause(HANDLE hTask)
        {
            CHECKFUNC(_TaskPause, FALSE);
            return _TaskPause(hTask);
        }
        BOOL TaskForceStop(HANDLE hTask)
        {
            CHECKFUNC(_TaskForceStop, FALSE);
            return _TaskForceStop(hTask);
        }
        BOOL TaskQuery(HANDLE hTask, DownTaskInfo & stTaskInfo)
        {
            CHECKFUNC(_TaskQuery, FALSE);
            return _TaskQuery(hTask, stTaskInfo);
        }
        BOOL TaskQueryEx(HANDLE hTask, DownTaskInfo & stTaskInfo)
        {
            CHECKFUNC(_TaskQueryEx, FALSE);
            return _TaskQueryEx(hTask, stTaskInfo);
        }
        void LimitSpeed(INT32 nBps)
        {
            CHECKFUNC_(_LimitSpeed);
            _LimitSpeed(nBps);
        }
        void LimitUploadSpeed(INT32 nTcpBps, INT32 nOtherBps)
        {
            CHECKFUNC_(_LimitUploadSpeed);
            _LimitUploadSpeed(nTcpBps, nOtherBps);
        }
        BOOL DelTempFile(DownTaskParam &stParam)
        {
            CHECKFUNC(_DelTempFile, FALSE);
            return _DelTempFile(stParam);
        }
        BOOL SetProxy(DOWN_PROXY_INFO &stProxy)
        {
            CHECKFUNC(_SetProxy, FALSE);
            return _SetProxy(stProxy);
        }
        void SetUserAgent(const TCHAR *pszUserAgent)
        {
            CHECKFUNC_(_SetUserAgent);
            _SetUserAgent(pszUserAgent);
        }
        BOOL GetFileSizeWithUrl(const wchar_t * lpURL, INT64& iFileSize)
        {
            CHECKFUNC(_GetFileSizeWithUrl, FALSE);
            return _GetFileSizeWithUrl(lpURL, iFileSize);
        }
        BOOL ParseThunderPrivateUrl(const TCHAR *pszThunderUrl, TCHAR *normalUrlBuffer, INT32 bufferLen)
        {
            if (_ParseThunderPrivateUrl == NULL) return FALSE;

            return _ParseThunderPrivateUrl(pszThunderUrl, normalUrlBuffer, bufferLen);
        }
        LONG SetAdditionInfo(HANDLE task_id, WSAPROTOCOL_INFOW *sock_info, CHAR *http_resp_buf, LONG buf_len)
        {
            CHECKFUNC(_SetAdditionInfo, FALSE);
            return _SetAdditionInfo(task_id, sock_info, http_resp_buf, buf_len);
        }
        LONG SetFileIdAndSize(HANDLE hTask, char szFileId[40], unsigned __int64 nFileSize)
        {
            CHECKFUNC(_SetFileIdAndSize, FALSE);
            return _SetFileIdAndSize(hTask, szFileId, nFileSize);
        }

    private:
        HMODULE                     m_hModule;

        fn_Init                     _Init;
        fn_UnInit                   _UnInit;
        fn_TaskCreate               _TaskCreate;
        fn_TaskDelete               _TaskDelete;
        fn_TaskStart                _TaskStart;
        fn_TaskPause                _TaskPause;
        fn_TaskQuery                _TaskQuery;
        fn_TaskQueryEx              _TaskQueryEx;
        fn_LimitSpeed               _LimitSpeed;
        fn_LimitUploadSpeed         _LimitUploadSpeed;
        fn_DelTempFile              _DelTempFile;
        fn_SetProxy                 _SetProxy;
        fn_SetUserAgent             _SetUserAgent;
        fn_GetFileSizeWithUrl       _GetFileSizeWithUrl;
        fn_ParseThunderPrivateUrl   _ParseThunderPrivateUrl;
        fn_SetAdditionInfo          _SetAdditionInfo;
        fn_SetFileIdAndSize         _SetFileIdAndSize;
        fn_TaskForceStop            _TaskForceStop;
    };
}


namespace xlDownloaderError
{
    const DWORD xlDownErr_Init_Failed = 0x90000001;
    const DWORD xlDownErr_Invalid_DownParam = 0x90000002;
    const DWORD xlDownErr_Task_Not_Exist = 0x90000003;
}

namespace cactus
{


class KFirewallRuleRegister
{
    enum Action
    {
        Act_Null = 0,
        Act_EnableNewCommunication,
    };

public:
    KFirewallRuleRegister(void);
    ~KFirewallRuleRegister(void);

    DWORD EnableNetWorkCommunication(
        __in LPCTSTR lpctModuleFullPath
        );

    void _ParseCmd(
        __in LPCTSTR lpctModuleFullPath,
        __in Action enAction,
        __out CString& strCmd,
        __out CString& strParam);

};


enum DTManager_Stat
{
	TASK_DONE = 0,
	TASK_DOWNING,
	TASK_PAUSED,
	TASK_CONECTING,
	TASK_ERROR,
	TASK_ERROR_MD5,
	TASK_CHANGE_DOWNLOAD,
	TASK_NEW,
	TASK_STOP,
	TASK_RESUME,
	TASK_ERROR_MD5_2,
	TASK_DONE_2,
	TASK_TERMINATED,
	TASK_XL_TIMEOUT,
	TASK_XL_INITFAILED,
};

enum DTManagerSrc
{
	TASK_SRC_UNDEFINE = 0,
	TASK_SRC_XL,
	TASK_SRC_HTTP,
	TASK_SRC_P2P,
	
};

typedef void (*GetTaskCallback)(void* tk,void* para);

//获取任务信息的回调
typedef void (*InfoCallBack)(__int64 cur,__int64 all,__int64 speed,DWORD time,__int64 accspeed,DTManagerSrc src, void* para);

typedef void (*DtStateChangeNotifyCallBack)(DTManager_Stat st,void* tk,void* para);


class IDTManagerCallBack
{
public:

	/**
	* p2p下载情况回调通知DTManger
	* @return   HRESULT		S_OK：表示已经更换下载方式(采用http)
	*						S_FALSE 表示未更换下载方式
	*/						 
	virtual STDMETHODIMP NotifyP2pCallBack( unsigned int uTaskId,                   ///< 任务ID
											void *pContext,                         ///< 任务Context 
											int nErrorCode							///<BtCompleteCode结束码
											) = 0; 
};




class DownWrapper;


class CXunleiDownloader
{
public:

    class CallBack
    {
    public:
        virtual DWORD OnDownloadProgress(
            DWORD dwTaskHandle,
            __int64 nTotalSize,
            __int64 nTotalDownload,
            int nTotalSpeed,
            int nAccSpeed,
            float fPercent) = 0;
        virtual DWORD OnDownloadComplete(DWORD dwTaskHandle, DTManager_Stat nState) = 0;
    };

    enum EnumAction {
        Act_Null = 0,
        Act_Create,
        Act_Pause,
        Act_Resume,
        Act_Delete,
    };

    struct TaskItem {
    public:
        HANDLE      _hTask;
        CallBack*   _pCallBackObj;
        CString     _strUrl;
        CString     _strSaveFolder;
        CString     _strFileName;
        CString     _strMd5;
        __time64_t   _startTime;
        __time64_t   _endTime;

        TaskItem()
            : _hTask(NULL)
            , _pCallBackObj(NULL)
            , _startTime(0)
            , _endTime(0)           
        {}
    };

public:
    static CXunleiDownloader& GetInstance();
    virtual ~CXunleiDownloader();
public:
    BOOL StartInitailize();    

    DWORD CreateTask(
        LPCWSTR lpUrl,
        LPCWSTR lpMd5,
        LPCWSTR lpFilePath,
        __in CallBack* pCallBackObj,
        __out DWORD* pdwTaskHandle);

    DWORD PauseTask(
        DWORD dwTaskHandle
        );

    DWORD ResumeTask(
        DWORD dwTaskHandle
        );

    DWORD DeleteTask(
        DWORD dwTaskHandle
        );

    BOOL TaskQueryEx(HANDLE hTask, DownTaskInfo & stTaskInfo);

    void WaitDone();

    void DelAllTasks();

    /* must call "Clear()" after use KxlDownloader! */
    void Clear();

protected:
    CXunleiDownloader();
    static DWORD  WINAPI _ThreadProc(LPVOID lpParam);
    static DWORD  WINAPI _InitXlEngineThread(LPVOID lpParam);
    DWORD _DownloadFunctionImpl();
    VOID _NotifyJob();
    VOID _NotifyDownloadThreadPause();
    VOID _NotifyDownloadThreadStop();
    VOID _StartReadyTasks();
    VOID _WaitDownloadThreadStop();
    VOID _PauseDownloadingTasks();
    VOID _ReportTasksStatus();
    VOID _DeleteOriginFileIfExists(const std::wstring& strFolder, const std::wstring& strFileName);
    BOOL _DeleteTaskAndTmpFile(const TaskItem& task);
    VOID _CancelUnfinishedTasks();
    VOID _WaitAllTasksCanceled(DWORD dwWaitMs = INFINITE);
    //BOOL _CheckMd5(const TaskItem& taskItem_refer);
    DWORD _PushTaskToList(const TaskItem& downloadItem);
    const TaskItem* _GetTaskItem(HANDLE TaskHandle);
    BOOL _CheckInit();
    DownWrapper* _LoadxlEngine(LPCWSTR lpcwDllpath);
    BOOL _HasReport_Register_FireWallRule();
    BOOL _PreparexlModules(const std::wstring& strxlModulePath);
private:
    DWORD _ProcessTask(DWORD dwTaskHandle, EnumAction enAction);
    void _TerminateXLProcess();
    void _CheckXLProcessExist(PHANDLE pProc, CString& XLPath);
private:
    HANDLE m_hInitEngineThread;
    ATL::CComAutoCriticalSection m_csInit;
    int m_TimeOutCount;
    BOOL m_bCanInit;
    BOOL m_bInitEngine;

    static CXunleiDownloader m_Instance;
    std::map<HANDLE, TaskItem> m_mapTasks;


    ATL::CComAutoCriticalSection m_csTaskJobs;
    HANDLE m_hDownloadThread;
    HANDLE m_hPauseEvent;
    HANDLE m_hJobEvent;
    HANDLE m_hStopEvent;
    DownWrapper* m_pxlEngineWrapper;

    ATL::CComAutoCriticalSection m_csTaskPause;
    std::queue<HANDLE> m_TasksforPause;
    ATL::CComAutoCriticalSection m_csTaskStart;
    std::queue<HANDLE> m_TasksforStart;



    KFirewallRuleRegister m_filewallRuleRegister;
};

} // namespace cactus

namespace cactus
{
    class CCCXunleiDownloader
    {
    public:
        enum EnumAction {
            Act_Null = 0,
            Act_Create,
            Act_Pause,
            Act_Resume,
            Act_Delete,
        };

        CCCXunleiDownloader();
        ~CCCXunleiDownloader();
    public:
        BOOL Init();
        BOOL UnInit();
        HANDLE CreateTask(LPCWSTR url, LPCWSTR filename);
        BOOL TaskQueryEx(HANDLE hTask, DownTaskInfo& stTaskInfo);
        BOOL DeleteTask(HANDLE hTask, CString saveFolder);
    protected:
        BOOL _DeleteTaskAndTmpFile(HANDLE hTask, CString saveFolder);
    private:
        BOOL _HasReport_Register_FireWallRule();        
    private:
        DownWrapper* wrapper_;
    };
}


#endif // __CACTUS_XUNLEI_H__
