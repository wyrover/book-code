#include <cactus/cactus_service.h>
#include "cactus/cactus_exception.h"
#include "cactus/cactus_process.h"
#include "cactus/cactus_registry.h"
#include <atlstr.h>
#include <ShellAPI.h>

namespace cactus
{

namespace service
{
#define _SERVICE_QUERY_SLEEP_TIME   (500)

#define _MAX_SERVICE_QUERY_TIMES    (10)

#define _SERVICE_GET_LOCK_SLEEP_TIME (500)

#define _MAX_SERVICE_TRY_GET_LOCK_TIMES (10)

//
// MessageId: E_KXEBASE_START_SERVICE_STOPPED
//
// MessageText:
//
//  Service auto stop when start service.
//
#define E_KXEBASE_START_SERVICE_STOPPED  ((DWORD)0xE0000018L)

static BOOL s_bIsReboot = FALSE;

enum enumKSERVICE_FIX_ACTION {
    enumSERVICE_FIX_NONE        = 0,
    enumSERVICE_FIX_START       = 1,
    enumSERVICE_FIX_INSTALL_AND_START   = 2
};

void RunasAdmin()
{
    TCHAR szPath[MAX_PATH] = {0};
    ::GetModuleFileName(NULL, szPath, MAX_PATH);
    SHELLEXECUTEINFO sei = {0};
    sei.cbSize = sizeof(SHELLEXECUTEINFOW);
    sei.fMask = 0;
    sei.nShow = SW_SHOWNORMAL;
    sei.lpFile = szPath;
    sei.lpVerb = TEXT("runas");
    sei.lpDirectory = NULL;
    KillProcess(NULL, TEXT("kwstray"));
    ShellExecuteEx(&sei);
    s_bIsReboot = TRUE;
}

BOOL IsNeededReboot(void)
{
    return s_bIsReboot;
}


BOOL IsServiceExist(const string16& service_name)
{
    BOOL bReturn = FALSE;
    HKEY hKey = NULL;
    LONG lRetCode = FALSE;
    CStringW strRegPath;
    strRegPath.Format(L"SYSTEM\\CurrentControlSet\\services\\%s", service_name.c_str());
    lRetCode = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE, strRegPath, 0, KEY_READ, &hKey);

    if (lRetCode != ERROR_SUCCESS)
        goto Exit0;

    bReturn = TRUE;
Exit0:

    if (hKey) RegCloseKey(hKey);

    return bReturn;
}

BOOL IsInstalled(const string16& service_name)
{
    BOOL bRet;
    SC_HANDLE schService;
    SC_HANDLE schSCManager;

    do {
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService) {
            CloseServiceHandle(schSCManager);
            break;
        }

        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
        bRet = TRUE;
    } while (0);

    return bRet;
}

BOOL IsStarted(const string16& service_name)
{
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;
    SERVICE_STATUS ssStatus;
    BOOL bRet = FALSE;

    do {
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService)
            break;

        if (QueryServiceStatus(schService, &ssStatus)) {
            if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
                bRet = TRUE;
            } else if (ssStatus.dwCurrentState == SERVICE_START_PENDING) {
                bRet = TRUE;
            }
        }
    } while (0);

    if (schService)
        CloseServiceHandle(schService);

    if (schSCManager)
        CloseServiceHandle(schSCManager);

    return bRet;
}

BOOL Start(const string16& service_name)
{
    BOOL bRet;
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;
    SERVICE_STATUS ssStatus;

    do {
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService)
            break;

        // try to stop the service
        if (StartService(schService, 0, NULL)) {
            Sleep(1000);

            while (QueryServiceStatus(schService, &ssStatus)) {
                if (ssStatus.dwCurrentState == SERVICE_START_PENDING) {
                    Sleep(1000);
                } else
                    break;
            }

            if (ssStatus.dwCurrentState != SERVICE_RUNNING) {
                break;
            }

            bRet = TRUE;
        }
    } while (0);

    if (schService)
        CloseServiceHandle(schService);

    if (schSCManager)
        CloseServiceHandle(schSCManager);

    return bRet;
}

void Stop(const string16& service_name)
{
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;
    SERVICE_STATUS ssStatus;

    do {
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService)
            break;

        // try to stop the service
        if (ControlService(schService, SERVICE_CONTROL_STOP, &ssStatus)) {
            std::cout << "Stopping service.";
            Sleep(1000);

            while (QueryServiceStatus(schService, &ssStatus)) {
                if (ssStatus.dwCurrentState == SERVICE_STOP_PENDING) {
                    std::cout << ".";
                    Sleep(1000);
                } else
                    break;
            }

            std::cout << std::endl;

            if (ssStatus.dwCurrentState != SERVICE_STOPPED)
                break;
        }
    } while (0);

    if (schService)
        CloseServiceHandle(schService);

    if (schSCManager)
        CloseServiceHandle(schSCManager);
}

BOOL Install(const string16& service_name, const string16& display_name, const string16& dependencies, DWORD dwServiceType /*= SERVICE_WIN32_OWN_PROCESS*/, string16& args /*= string16()*/, string16& exe /*= string16()*/)
{
    BOOL bRet;
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;

    do {
        if (exe.empty()) {
            char16 szPath[512];

            if (GetModuleFileName(NULL, szPath, 512) == 0)
                break;

            exe = szPath;
        }

        string16 bin = _T("\"") + exe + _T("\"");

        if (!args.empty())
            bin += _T(" ") + args;

        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = CreateService(
                         schSCManager,               // SCManager database
                         service_name.c_str(),             // name of service
                         display_name.c_str(),      // name to display
                         SERVICE_ALL_ACCESS,         // desired access
                         dwServiceType,              // service type
                         SERVICE_AUTO_START,         // start type
                         SERVICE_ERROR_NORMAL,       // error control type
                         bin.c_str(),                // service's binary
                         NULL,                       // no load ordering group
                         NULL,                       // no tag identifier
                         dependencies.c_str(),     // dependencies
                         NULL,                       // LocalSystem account
                         NULL);                      // no password

        if (!schService)
            break;

        bRet = TRUE;
    } while (0);

    if (schService)
        CloseServiceHandle(schService);

    if (schSCManager)
        CloseServiceHandle(schSCManager);

    return bRet;
}

BOOL Uninstall(const string16& service_name)
{
    BOOL bRet;
    SC_HANDLE   schService        ;
    SC_HANDLE   schSCManager;
    Stop(service_name);

    do {
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService)
            break;

        if (!DeleteService(schService))
            break;

        bRet = TRUE;
    } while (0);

    if (schService)
        CloseServiceHandle(schService);

    if (schSCManager)
        CloseServiceHandle(schSCManager);

    return bRet;
}

typedef BOOL (WINAPI*PFChangeServiceConfig2)(SC_HANDLE hService, DWORD dwInfoLevel, LPVOID lpInfo);

void SetDescription(const string16& service_name, const string16& desc)
{
    PFChangeServiceConfig2 FChangeServiceConfig2;
    HMODULE ADVAPI;
    SC_HANDLE schSCManager;
    SC_HANDLE schService;

    do {
        ADVAPI = ::LoadLibrary(_T("Advapi32"));

        if (!ADVAPI)
            break;

#ifdef UNICODE
        FChangeServiceConfig2 = (PFChangeServiceConfig2)::GetProcAddress(ADVAPI, "ChangeServiceConfig2W");
#else
        FChangeServiceConfig2 = (PFChangeServiceConfig2)::GetProcAddress(ADVAPI, _TEXT("ChangeServiceConfig2A"));
#endif

        if (!FChangeServiceConfig2)
            break;

        SERVICE_DESCRIPTION descr;
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService)
            break;

        TCHAR* d = new TCHAR[desc.length() + 2];
        wcsncpy(d, desc.c_str(), desc.length() + 1);
        descr.lpDescription = d;
        BOOL bResult = FChangeServiceConfig2(schService, SERVICE_CONFIG_DESCRIPTION, &descr);
        delete [] d;
    } while (0);

    if (schService)
        CloseServiceHandle(schService);

    if (schSCManager)
        CloseServiceHandle(schSCManager);

    if (ADVAPI)
        FreeLibrary(ADVAPI);
}

BOOL GetServiceType(const string16& service_name, DWORD& service_type)
{
    BOOL bRet;
    SC_HANDLE schService;
    SC_HANDLE schSCManager;
    LPQUERY_SERVICE_CONFIG lpqscBuf;

    do {
        lpqscBuf = (LPQUERY_SERVICE_CONFIG) LocalAlloc(LPTR, 4096);

        if (lpqscBuf == NULL)
            break;

        TCHAR szPath[512];

        if (GetModuleFileName(NULL, szPath, 512) == 0)
            break;

        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService)
            break;

        DWORD dwBytesNeeded = 0;
        BOOL success = QueryServiceConfig(schService, lpqscBuf,  4096, &dwBytesNeeded);

        if (success != TRUE)
            break;

        service_type = lpqscBuf->dwServiceType;
        bRet = TRUE;
    } while (0);

    if (schService)
        CloseServiceHandle(schService);

    if (schSCManager)
        CloseServiceHandle(schSCManager);

    if (lpqscBuf)
        LocalFree(lpqscBuf);

    return bRet;
}

BOOL CheckServiceStatus(const wchar_t* pszServiceName,
                        BOOL* pbServiceExist,
                        DWORD* pdwStatus,
                        DWORD* pdwError)
{
    if (NULL == pszServiceName || NULL == pbServiceExist || NULL == pdwStatus)
        return FALSE;

    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    DWORD       dwErr       = 0;
    SC_HANDLE   hScm        = NULL;
    SC_HANDLE   hService    = NULL;
    SERVICE_STATUS ssStatus = { 0 };
    hScm = ::OpenSCManagerW(NULL, NULL, SERVICE_QUERY_STATUS);

    if (NULL == hScm) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_QUERY_STATUS);

    if (NULL == hService) {
        dwErr = ::GetLastError();

        if (ERROR_SERVICE_DOES_NOT_EXIST == dwErr) {
            bRetCode = TRUE;
        }

        goto Exit0;
    } else {
        bRetCode = TRUE;
        *pbServiceExist = TRUE;
    }

    bResult = ::QueryServiceStatus(hService, &ssStatus);

    if (bResult) {
        *pdwStatus = ssStatus.dwCurrentState;
    } else {
        dwErr = ::GetLastError();
    }

Exit0:

    if (pdwError)
        *pdwError = dwErr;

    if (hService)
        ::CloseServiceHandle(hService);

    if (hScm)
        ::CloseServiceHandle(hScm);

    return bRetCode;
}


BOOL StartService(const wchar_t* pszServiceName, DWORD* pdwError)
{
    if (NULL == pszServiceName)
        return FALSE;

    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    DWORD       dwErr       = 0;
    SC_HANDLE   hScm        = NULL;
    SC_HANDLE   hService    = NULL;
    int         nTimes      = 0;
    SC_HANDLE   schSCManager    = NULL;
    SC_HANDLE   schService      = NULL;
    SERVICE_STATUS ssStatus     = {0};
    hScm = ::OpenSCManager(NULL, NULL, SERVICE_START | SERVICE_QUERY_STATUS);

    if (NULL == hScm) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_START | SERVICE_QUERY_STATUS);

    if (NULL == hService) {
        dwErr = ::GetLastError();

        if (dwErr == ERROR_ACCESS_DENIED) {
            RunasAdmin();
        }

        goto Exit0;
    }

    // 查询服务当前的状态
    bResult = ::QueryServiceStatus(hService, &ssStatus);

    if (!bResult) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    // 如果正在运行就退出，不做什么
    if (ssStatus.dwCurrentState == SERVICE_RUNNING) {
        bRetCode = TRUE;
        goto Exit0;
    }

    // 开始服务
    bResult = ::StartService(hService, 0L, NULL);

    if (!bResult) {
        dwErr = ::GetLastError();

        if (ERROR_SERVICE_ALREADY_RUNNING == dwErr) {
            bRetCode = TRUE;
        }

        goto Exit0;
    }

    // 不停的查询服务状态,直到超过规定的次数
    do {
        bResult = ::QueryServiceStatus(hService, &ssStatus);

        if (ssStatus.dwCurrentState != SERVICE_START_PENDING)
            break;

        nTimes++;
        Sleep(_SERVICE_QUERY_SLEEP_TIME);
    } while (nTimes < _MAX_SERVICE_QUERY_TIMES);

    if (ssStatus.dwCurrentState == SERVICE_RUNNING)
        bRetCode = TRUE;

Exit0:

    if (pdwError)
        *pdwError = dwErr;

    if (hService)
        ::CloseServiceHandle(hService);

    if (hScm)
        ::CloseServiceHandle(hScm);

    return bRetCode;
}

BOOL ChangeServiceConfig(const wchar_t* pszServiceName, const wchar_t* pszImagePath, const wchar_t* pszSvcOrderGroup, const wchar_t* pszDisplayName, const wchar_t* pszDescription, DWORD dwStartType, DWORD* pdwError)
{
    if (NULL == pszServiceName)
        return FALSE;

    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    DWORD       dwErr       = 0;
    SC_HANDLE   hScm        = NULL;
    SC_HANDLE   hService    = NULL;
    SC_LOCK     sclLock     = NULL;
    int         nTimes      = 0;
    SERVICE_STATUS ssStatus = { 0 };
    SERVICE_DESCRIPTION sd = {0};
    hScm = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_LOCK | SC_MANAGER_CREATE_SERVICE);

    if (NULL == hScm) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    do {
        sclLock = LockServiceDatabase(hScm);

        if (sclLock)
            break;

        ++nTimes;
        Sleep(_SERVICE_GET_LOCK_SLEEP_TIME);
    } while (nTimes < _MAX_SERVICE_TRY_GET_LOCK_TIMES);

    hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_CHANGE_CONFIG | SERVICE_STOP | SERVICE_USER_DEFINED_CONTROL);

    if (NULL == hService) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    // 如果打开了, 则修改为我们想要的服务
    bResult = ::ChangeServiceConfig(
                  hService,                 // handle of service
                  SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS, // change service type
                  dwStartType,         // change service start type
                  SERVICE_ERROR_NORMAL,//SERVICE_NO_CHANGE       // change error control
                  pszImagePath,               // change binary path
                  pszSvcOrderGroup,           // change load order group
                  NULL,                       // tag ID: no change
                  NULL,                       // dependencies: no change
                  NULL,                       // service start account: no change
                  NULL,                       // password: no change
                  pszDisplayName);            // change display name

    if (pszDescription) {
        SERVICE_DESCRIPTION sd = {0};
        sd.lpDescription = (LPWSTR)pszDescription;
        ::ChangeServiceConfig2(
            hService,
            SERVICE_CONFIG_DESCRIPTION,
            &sd
        );
    }

    if (bResult)
        bRetCode = TRUE;
    else
        dwErr = ::GetLastError();

Exit0:

    if (pdwError)
        *pdwError = dwErr;

    if (sclLock)
        ::UnlockServiceDatabase(sclLock);

    if (hService)
        ::CloseServiceHandle(hService);

    if (hScm)
        ::CloseServiceHandle(hScm);

    return bRetCode;
}

BOOL CreateService(const wchar_t* pszServiceName, const wchar_t* pszImagePath, const wchar_t* pszSvcOrderGroup, const wchar_t* pszDisplayName, const wchar_t* pszDescription, DWORD dwStartType, DWORD* pdwError, const wchar_t* pDepend /*= NULL*/, DWORD nServicesType /*= SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS*/)
{
    if (NULL == pszServiceName)
        return FALSE;

    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    DWORD       dwErr       = 0;
    SC_HANDLE   hScm        = NULL;
    SC_HANDLE   hService    = NULL;
    SC_LOCK     sclLock     = NULL;
    int         nTimes      = 0;
    SERVICE_STATUS ssStatus = { 0 };
    hScm = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_CONNECT | SC_MANAGER_LOCK | SC_MANAGER_CREATE_SERVICE);

    if (NULL == hScm) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    do {
        sclLock = LockServiceDatabase(hScm);

        if (sclLock)
            break;

        ++nTimes;
        Sleep(_SERVICE_GET_LOCK_SLEEP_TIME);
    } while (nTimes < _MAX_SERVICE_TRY_GET_LOCK_TIMES);

    hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_CHANGE_CONFIG | SERVICE_STOP | SERVICE_USER_DEFINED_CONTROL);

    if (NULL == hService) {
        hService = ::CreateService(
                       hScm,                   // SCManager database
                       pszServiceName,         // name of service
                       pszDisplayName,         // name to display
                       SERVICE_QUERY_STATUS | SERVICE_CHANGE_CONFIG,// desired access
                       nServicesType,// service type
                       SERVICE_AUTO_START,         // start type
                       SERVICE_ERROR_NORMAL,       // error control type
                       pszImagePath,               // service's binary
                       pszSvcOrderGroup,           // load ordering group
                       NULL,                       // no tag identifier
                       pDepend,                        // dependencies
                       NULL,                       // LocalSystem account
                       NULL);                      // no password

        if (hService)
            bResult = TRUE;
    } else {
        // 如果打开了, 则修改为我们想要的服务
        bResult = ::ChangeServiceConfig(
                      hService,                 // handle of service
                      SERVICE_WIN32_OWN_PROCESS, // change service type
                      dwStartType,         // change service start type
                      SERVICE_ERROR_NORMAL,//SERVICE_NO_CHANGE       // change error control
                      pszImagePath,               // change binary path
                      pszSvcOrderGroup,           // change load order group
                      NULL,                       // tag ID: no change
                      NULL,                       // dependencies: no change
                      NULL,                       // service start account: no change
                      NULL,                       // password: no change
                      pszDisplayName);            // change display name
    }

    if (pszDescription) {
        SERVICE_DESCRIPTION sd = {0};
        sd.lpDescription = (LPWSTR)pszDescription;
        ChangeServiceConfig2(
            hService,
            SERVICE_CONFIG_DESCRIPTION,
            &sd
        );
    }

    if (bResult)
        bRetCode = TRUE;
    else
        dwErr = ::GetLastError();

Exit0:

    if (pdwError)
        *pdwError = dwErr;

    if (sclLock)
        ::UnlockServiceDatabase(sclLock);

    if (hService)
        ::CloseServiceHandle(hService);

    if (hScm)
        ::CloseServiceHandle(hScm);

    return bRetCode;
}

BOOL StopService(const wchar_t* pszServiceName, DWORD* pdwError)
{
    if (NULL == pszServiceName)
        return FALSE;

    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    DWORD       dwErr       = 0;
    SC_HANDLE   hScm        = NULL;
    SC_HANDLE   hService    = NULL;
    SERVICE_STATUS ssStatus = { 0 };
    hScm = ::OpenSCManagerW(NULL, NULL, GENERIC_WRITE);

    if (NULL == hScm) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_STOP | SERVICE_QUERY_STATUS);

    if (NULL == hService) {
        dwErr = ::GetLastError();

        if (ERROR_SERVICE_DOES_NOT_EXIST == dwErr)
            bRetCode = TRUE;

        goto Exit0;
    }

    if (hService) {
        bRetCode = ::ControlService(hService, SERVICE_CONTROL_STOP, &ssStatus);

        if (bRetCode || ERROR_SERVICE_NOT_ACTIVE == ::GetLastError())
            bRetCode = TRUE;
        else
            dwErr = ::GetLastError();
    }

Exit0:

    if (pdwError)
        *pdwError = dwErr;

    if (hService)
        ::CloseServiceHandle(hService);

    if (hScm)
        ::CloseServiceHandle(hScm);

    return bRetCode;
}

BOOL DeleteService(const wchar_t* pszServiceName, DWORD* pdwError)
{
    if (NULL == pszServiceName)
        return FALSE;

    BOOL        bResult     = FALSE;
    BOOL        bRetCode    = FALSE;
    DWORD       dwErr       = 0;
    SC_HANDLE   hScm        = NULL;
    SC_HANDLE   hService    = NULL;
    SERVICE_STATUS ssStatus = { 0 };
    hScm = ::OpenSCManagerW(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    if (NULL == hScm) {
        dwErr = ::GetLastError();
        goto Exit0;
    }

    hService = ::OpenServiceW(hScm, pszServiceName, DELETE);

    if (NULL == hService) {
        dwErr = ::GetLastError();

        if (ERROR_SERVICE_DOES_NOT_EXIST == dwErr)
            bRetCode = TRUE;

        goto Exit0;
    }

    if (hService) {
        bRetCode = ::DeleteService(hService);

        if (bRetCode)
            bRetCode = TRUE;
        else
            dwErr = ::GetLastError();
    }

Exit0:

    if (pdwError)
        *pdwError = dwErr;

    if (hService)
        ::CloseServiceHandle(hService);

    if (hScm)
        ::CloseServiceHandle(hScm);

    return bRetCode;
}

BOOL CheckAndFixService(const wchar_t* pszServiceName, const wchar_t* pszImagePath, const wchar_t* pszSvcOrderGroup, const wchar_t* pszDisplayName, const wchar_t* pszDescription, int* pnFixAction, DWORD* pdwError)
{
    if (pnFixAction)
        *pnFixAction = enumSERVICE_FIX_NONE;

    if (pdwError)
        *pdwError = 0;

    if (NULL == pszServiceName)
        return FALSE;

    DWORD dwStatus  = 0;
    DWORD dwErr     = 0;
    BOOL bSvcExist  = FALSE;
    BOOL bResult    = FALSE;
    //查询服务状态
    s_bIsReboot = FALSE;
    bResult = CheckServiceStatus(pszServiceName, &bSvcExist, &dwStatus, &dwErr);

    if (bSvcExist && dwStatus == SERVICE_RUNNING)
        return TRUE;

    if (bSvcExist && dwStatus == SERVICE_PAUSED) {
        SC_HANDLE           hScm        = NULL;
        SC_HANDLE           hService    = NULL;
        SERVICE_STATUS      status      = {0};

        do {
            hScm = ::OpenSCManager(NULL, NULL, SERVICE_START | SERVICE_QUERY_STATUS);

            if (NULL == hScm) {
                dwErr = ::GetLastError();

                if (dwErr == ERROR_ACCESS_DENIED) {
                    RunasAdmin();
                }

                break;
            }

            hService = ::OpenServiceW(hScm, pszServiceName, SERVICE_START | SERVICE_PAUSE_CONTINUE);

            if (NULL == hService) {
                dwErr = ::GetLastError();

                if (dwErr == ERROR_ACCESS_DENIED) {
                    RunasAdmin();
                }

                break;
            }

            ControlService(hService, SERVICE_CONTROL_CONTINUE, &status);
        } while (0);

        if (NULL != hService) {
            ::CloseServiceHandle(hService);
            hService = NULL;
        }

        if (NULL != hScm) {
            ::CloseServiceHandle(hScm);
            hScm = NULL;
        }

        if (status.dwCurrentState == SERVICE_RUNNING) {
            return TRUE;
        } else {
            return FALSE;
        }
    }

    if (bSvcExist) {
        //启动服务
        bResult = StartService(pszServiceName, &dwErr);

        if (bResult) {
            if (pnFixAction)
                *pnFixAction = enumSERVICE_FIX_START;

            return TRUE;
        }

        if (IsNeededReboot()) {
            return FALSE;
        }

        if (!bResult && ERROR_SERVICE_DISABLED == dwErr) {
            //如果是由于服务被禁用而无法启动,那么把服务设置为手工启动(因为可能是用户禁用,所有不设置为自动启动)
            bResult = ChangeServiceConfig(pszServiceName, NULL, NULL, NULL, NULL, SERVICE_DEMAND_START, &dwErr);

            //修改成功后再次尝试启动
            if (bResult && StartService(pszServiceName, &dwErr)) {
                if (pnFixAction)
                    *pnFixAction = enumSERVICE_FIX_START;

                return TRUE;
            }
        }

        //可能是注册表损坏导致无法启动或修改,那么先删除服务然后再安装
        if (!bResult && (ERROR_PATH_NOT_FOUND == dwErr || ERROR_FILE_NOT_FOUND == dwErr)) {
            bResult =  DeleteService(pszServiceName, &dwErr);
            bSvcExist = FALSE;
        }
    }

    if (!bSvcExist && pszImagePath) { //安装服务必须要有服务程序路径
        bResult = CreateService(
                      pszServiceName,
                      pszImagePath,
                      pszSvcOrderGroup,
                      pszDisplayName,
                      pszDescription,
                      SERVICE_AUTO_START,
                      &dwErr);

        if (bResult && StartService(pszServiceName, &dwErr)) {
            if (pnFixAction)
                *pnFixAction = enumSERVICE_FIX_INSTALL_AND_START;

            return TRUE;
        }
    }//!bSvcExist

    if (pdwError)
        *pdwError = dwErr;

    return FALSE;
}

int KxEInstallService(TCHAR* pPath,
                      TCHAR* pInternalName,
                      TCHAR* pszServiceName,
                      TCHAR* pDescriptiveName,
                      TCHAR* pSvcGroupOrder)
{
    SC_HANDLE schSCManager = OpenSCManager(
                                 NULL,
                                 NULL,
                                 SC_MANAGER_CREATE_SERVICE
                             );

    if (schSCManager == NULL) {
        return GetLastError();
    }

    SC_HANDLE schService = CreateService(
                               schSCManager,                    /* SCManager database      */
                               pInternalName,                   /* name of service         */
                               pszServiceName,                  /* service name to display */
                               SERVICE_START | SERVICE_STOP | SERVICE_CHANGE_CONFIG,    /* desired access          */
                               SERVICE_WIN32_OWN_PROCESS ,       /* service type            */
                               SERVICE_AUTO_START,              /* start type              */
                               SERVICE_ERROR_NORMAL,            /* error control type      */
                               pPath,                           /* service's binary        */
                               pSvcGroupOrder,                  /* no load ordering group  */
                               NULL,                            /* no tag identifier       */
                               NULL,                            /* no dependencies         */
                               NULL,                            /* LocalSystem account     */
                               NULL
                           );                                  /* no password             */

    if (schService == NULL) {
        if (GetLastError() != ERROR_SERVICE_EXISTS) { // 如果服务存在,还原设置
            int nErr = GetLastError();
            CloseServiceHandle(schSCManager);
            return nErr;
        }

        schService = OpenService(
                         schSCManager,
                         pInternalName,
                         SERVICE_CHANGE_CONFIG
                     );

        if (schService == NULL) {
            int nErr = GetLastError();
            CloseServiceHandle(schSCManager);
            return nErr;
        }

        BOOL bRet = ChangeServiceConfig(
                        schService,
                        SERVICE_WIN32_OWN_PROCESS ,
                        SERVICE_AUTO_START,
                        SERVICE_ERROR_NORMAL,
                        pPath,
                        pSvcGroupOrder,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        pDescriptiveName
                    );

        if (!bRet) { // 成功还原设置
            int nErr = GetLastError();
            CloseServiceHandle(schSCManager);
            return nErr;
        }
    }

    SERVICE_DESCRIPTION sd = {0};
    sd.lpDescription = pDescriptiveName;
    ChangeServiceConfig2(
        schService,
        SERVICE_CONFIG_DESCRIPTION,
        &sd
    );
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 0;
}

int KxEInstallServiceInteractive(TCHAR* pPath,
                                 TCHAR* pInternalName,
                                 TCHAR* pszServiceName,
                                 TCHAR* pDescriptiveName,
                                 TCHAR* pSvcGroupOrder)
{
    SC_HANDLE schSCManager = OpenSCManager(
                                 NULL,
                                 NULL,
                                 SC_MANAGER_CREATE_SERVICE
                             );

    if (schSCManager == NULL) {
        return GetLastError();
    }

    SC_HANDLE schService = CreateService(
                               schSCManager,                    /* SCManager database      */
                               pInternalName,                   /* name of service         */
                               pszServiceName,                  /* service name to display */
                               SERVICE_START | SERVICE_STOP | SERVICE_CHANGE_CONFIG,    /* desired access          */
                               SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,       /* service type            */
                               SERVICE_AUTO_START,              /* start type              */
                               SERVICE_ERROR_NORMAL,            /* error control type      */
                               pPath,                           /* service's binary        */
                               pSvcGroupOrder,                  /* no load ordering group  */
                               NULL,                            /* no tag identifier       */
                               NULL,                            /* no dependencies         */
                               NULL,                            /* LocalSystem account     */
                               NULL
                           );                                  /* no password             */

    if (schService == NULL) {
        if (GetLastError() != ERROR_SERVICE_EXISTS) { // 如果服务存在,还原设置
            int nErr = GetLastError();
            CloseServiceHandle(schSCManager);
            return nErr;
        }

        schService = OpenService(
                         schSCManager,
                         pInternalName,
                         SERVICE_CHANGE_CONFIG
                     );

        if (schService == NULL) {
            int nErr = GetLastError();
            CloseServiceHandle(schSCManager);
            return nErr;
        }

        BOOL bRet = ChangeServiceConfig(
                        schService,
                        SERVICE_WIN32_OWN_PROCESS | SERVICE_INTERACTIVE_PROCESS,
                        SERVICE_AUTO_START,
                        SERVICE_ERROR_NORMAL,
                        pPath,
                        pSvcGroupOrder,
                        NULL,
                        NULL,
                        NULL,
                        NULL,
                        pDescriptiveName
                    );

        if (!bRet) { // 成功还原设置
            int nErr = GetLastError();
            CloseServiceHandle(schSCManager);
            return nErr;
        }
    }

    SERVICE_DESCRIPTION sd = {0};
    sd.lpDescription = pDescriptiveName;
    ChangeServiceConfig2(
        schService,
        SERVICE_CONFIG_DESCRIPTION,
        &sd
    );
    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 0;
}

int KxEUninstallService(TCHAR *pInternalName)
{
    SC_HANDLE schSCManager = OpenSCManager(
                                 NULL,
                                 NULL,
                                 SC_MANAGER_CREATE_SERVICE
                             );

    if (schSCManager == NULL) {
        return GetLastError();
    }

    SC_HANDLE schService = OpenService(
                               schSCManager,
                               pInternalName,
                               DELETE
                           );

    if (schService == NULL) {
        int nErr = GetLastError();
        CloseServiceHandle(schSCManager);
        return nErr;
    }

    if (!DeleteService(schService)) {
        int nErr = GetLastError();
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
        return nErr;
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return 0;
}


int KxEStartService(const TCHAR *pInternalName,
                    bool bWaitRunning)
{
    SC_HANDLE schSCManager = OpenSCManager(
                                 NULL,
                                 NULL,
                                 GENERIC_WRITE
                             );

    if (schSCManager == NULL) {
        return GetLastError();
    }

    SC_HANDLE schService = OpenService(
                               schSCManager,
                               pInternalName,
                               SERVICE_START | SERVICE_QUERY_STATUS
                           );

    if (schService == NULL) {
        int nErr = GetLastError();
        CloseServiceHandle(schSCManager);
        return nErr;
    }

    SERVICE_STATUS status = {0};

    if (QueryServiceStatus(schService, &status) &&
        status.dwCurrentState == SERVICE_RUNNING) {
        return 0;
    }

    if (!StartService(schService, 0, NULL)) {
        int nErr = GetLastError();
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
        return nErr;
    }

    int nRet = 0;

    if (bWaitRunning) {
        while (1) {
            BOOL bRet = QueryServiceStatus(schService, &status);

            if (!bRet) {
                nRet = GetLastError();
                break;
            } else {
                if (status.dwCurrentState == SERVICE_STOPPED) {
                    nRet = E_KXEBASE_START_SERVICE_STOPPED;
                    break;
                } else if (status.dwCurrentState == SERVICE_RUNNING) {
                    break;
                } else {
                    Sleep(100);
                }
            }
        }
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return nRet;
}

int KxEStopService(const TCHAR *pInternalName,
                   bool bWaitUntilStopped)
{
    SC_HANDLE schSCManager = OpenSCManager(
                                 NULL,
                                 NULL,
                                 GENERIC_WRITE
                             );

    if (schSCManager == NULL) {
        return GetLastError();
    }

    SC_HANDLE schService = OpenService(
                               schSCManager,
                               pInternalName,
                               SERVICE_STOP | SERVICE_QUERY_STATUS
                           );

    if (schService == NULL) {
        int nErr = GetLastError();
        CloseServiceHandle(schSCManager);
        return nErr;
    }

    SERVICE_STATUS status = {0};

    if (!ControlService(schService, SERVICE_CONTROL_STOP, &status)) {
        int nErr = GetLastError();
        CloseServiceHandle(schSCManager);
        CloseServiceHandle(schService);
        return nErr;
    }

    int nRet = 0;

    if (bWaitUntilStopped) {
        SERVICE_STATUS status = {0};

        while (1) {
            BOOL bRet = QueryServiceStatus(schService, &status);

            if (!bRet) {
                nRet = GetLastError();
                break;
            } else {
                if (status.dwCurrentState == SERVICE_STOPPED) {
                    break;
                } else {
                    Sleep(100);
                }
            }
        }
    }

    CloseServiceHandle(schService);
    CloseServiceHandle(schSCManager);
    return nRet;
}

BOOL ModifyServiceType(const string16& service_name, DWORD service_type)
{
    BOOL bRet;
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;
    TCHAR szPath[512];

    do {
        if (GetModuleFileName(NULL, szPath, 512) == 0)
            break;

        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(), SERVICE_ALL_ACCESS);

        if (!schService) {
            DWORD err = GetLastError();
            CloseServiceHandle(schSCManager);
            break;
        }

        BOOL result = ChangeServiceConfig(schService, service_type, SERVICE_NO_CHANGE, SERVICE_NO_CHANGE , NULL, NULL, NULL,
                                          NULL, NULL, NULL, NULL);
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);

        if (result != TRUE)
            break;

        bRet = TRUE;
    } while (0);

    return bRet;
}

string16 get_service_exe_path(const string16& service_name)
{
    string16 ret = StringUtil::sWEmpty;
    SC_HANDLE   schService;
    SC_HANDLE   schSCManager;

    do {
        schSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

        if (!schSCManager)
            break;

        schService = OpenService(schSCManager, service_name.c_str(),  SERVICE_QUERY_CONFIG);

        if (!schService) {
            CloseServiceHandle(schSCManager);
            break;
        }

        DWORD dwBytesNeeded = 0;
        DWORD lErr;

        if (QueryServiceConfig(schService, NULL, 0, &dwBytesNeeded) || ((lErr = GetLastError()) != ERROR_INSUFFICIENT_BUFFER)) {
            CloseServiceHandle(schService);
            CloseServiceHandle(schSCManager);
            break;
        }

        LPQUERY_SERVICE_CONFIG lpqscBuf = (LPQUERY_SERVICE_CONFIG)LocalAlloc(LPTR, dwBytesNeeded + 10);
        BOOL bRet = (lpqscBuf != NULL) && (QueryServiceConfig(schService, lpqscBuf, dwBytesNeeded, &dwBytesNeeded) == TRUE);

        if (!bRet)
            lErr = GetLastError();
        else {
            ret = lpqscBuf->lpBinaryPathName;
        }

        LocalFree(lpqscBuf);
        CloseServiceHandle(schService);
        CloseServiceHandle(schSCManager);
    } while (0);

    return ret;
}

} // namespace service
} // namespace cactus

namespace cactus
{

const int WinService::STARTUP_TIMEOUT = 30000;
const string16 WinService::REGISTRY_KEY(L"SYSTEM\\CurrentControlSet\\Services\\");
const string16 WinService::REGISTRY_DESCRIPTION(L"Description");


WinService::WinService(const string16& name):
    _name(name),
    _svcHandle(0)
{
    _scmHandle = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);

    if (!_scmHandle) throw SystemException("cannot open Service Control Manager");
}


WinService::~WinService()
{
    close();
    CloseServiceHandle(_scmHandle);
}


const string16& WinService::name() const
{
    return _name;
}


string16 WinService::displayName() const
{
    LPQUERY_SERVICE_CONFIGW pSvcConfig = config();
    std::wstring udispName(pSvcConfig->lpDisplayName);
    LocalFree(pSvcConfig);
    return udispName;
}


string16 WinService::path() const
{
    LPQUERY_SERVICE_CONFIGW pSvcConfig = config();
    std::wstring path(pSvcConfig->lpBinaryPathName);
    LocalFree(pSvcConfig);
    return path;
}


void WinService::registerService(const string16& path, const string16& displayName)
{
    close();
    _svcHandle = CreateServiceW(
                     _scmHandle,
                     _name.c_str(),
                     displayName.c_str(),
                     SERVICE_ALL_ACCESS,
                     SERVICE_WIN32_OWN_PROCESS,
                     SERVICE_DEMAND_START,
                     SERVICE_ERROR_NORMAL,
                     path.c_str(),
                     NULL, NULL, NULL, NULL, NULL);

    if (!_svcHandle)
        throw SystemException("cannot register service", convert_to<string8>(_name));
}


void WinService::registerService(const string16& path)
{
    registerService(path, _name);
}


void WinService::unregisterService()
{
    open();

    if (!DeleteService(_svcHandle))
        throw SystemException("cannot unregister service", convert_to<string8>(_name));
}


bool WinService::isRegistered() const
{
    return tryOpen();
}


bool WinService::isRunning() const
{
    open();
    SERVICE_STATUS ss;

    if (!QueryServiceStatus(_svcHandle, &ss))
        throw SystemException("cannot query service status", convert_to<string8>(_name));

    return ss.dwCurrentState == SERVICE_RUNNING;
}


void WinService::start()
{
    open();

    if (!StartService(_svcHandle, 0, NULL))
        throw SystemException("cannot start service", convert_to<string8>(_name));

    SERVICE_STATUS svcStatus;
    long msecs = 0;

    while (msecs < STARTUP_TIMEOUT) {
        if (!QueryServiceStatus(_svcHandle, &svcStatus)) break;

        if (svcStatus.dwCurrentState != SERVICE_START_PENDING) break;

        Sleep(DWORD(250));
        msecs += 250;
    }

    if (!QueryServiceStatus(_svcHandle, &svcStatus))
        throw SystemException("cannot query status of starting service", convert_to<string8>(_name));
    else if (svcStatus.dwCurrentState != SERVICE_RUNNING)
        throw SystemException("service failed to start within a reasonable time", convert_to<string8>(_name));
}


void WinService::stop()
{
    open();
    SERVICE_STATUS svcStatus;

    if (!ControlService(_svcHandle, SERVICE_CONTROL_STOP, &svcStatus))
        throw SystemException("cannot stop service", convert_to<string8>(_name));
}


void WinService::setStartup(WinService::Startup startup)
{
    open();
    DWORD startType;

    switch (startup) {
    case SVC_AUTO_START:
        startType = SERVICE_AUTO_START;
        break;

    case SVC_MANUAL_START:
        startType = SERVICE_DEMAND_START;
        break;

    case SVC_DISABLED:
        startType = SERVICE_DISABLED;
        break;

    default:
        startType = SERVICE_NO_CHANGE;
    }

    if (!ChangeServiceConfig(_svcHandle, SERVICE_NO_CHANGE, startType, SERVICE_NO_CHANGE, NULL, NULL, NULL, NULL, NULL, NULL, NULL)) {
        throw SystemException("cannot change service startup mode");
    }
}


WinService::Startup WinService::getStartup() const
{
    LPQUERY_SERVICE_CONFIGW pSvcConfig = config();
    Startup result;

    switch (pSvcConfig->dwStartType) {
    case SERVICE_AUTO_START:
    case SERVICE_BOOT_START:
    case SERVICE_SYSTEM_START:
        result = SVC_AUTO_START;
        break;

    case SERVICE_DEMAND_START:
        result = SVC_MANUAL_START;
        break;

    case SERVICE_DISABLED:
        result = SVC_DISABLED;
        break;

    default:
        //poco_debugger();
        result = SVC_MANUAL_START;
    }

    LocalFree(pSvcConfig);
    return result;
}


void WinService::setDescription(const string16& description)
{
    string16 key(REGISTRY_KEY);
    key += _name;
    CRegKey reg;
    reg.Open(HKEY_LOCAL_MACHINE, key.c_str());
    reg.WriteStringValue(REGISTRY_DESCRIPTION, description);
    //WinRegistryKey regKey(HKEY_LOCAL_MACHINE, key);
    //regKey.setString(REGISTRY_DESCRIPTION, description);
}


string16 WinService::getDescription() const
{
    string16 key(REGISTRY_KEY);
    key += _name;
    CRegKey reg;
    reg.Open(HKEY_LOCAL_MACHINE, key.c_str());
    string16 data;
    BOOL lRes =  reg.ReadStringValue(REGISTRY_DESCRIPTION, data);

    if (lRes)
        return data;

    return StringUtil::sWEmpty;
    //WinRegistryKey regKey(HKEY_LOCAL_MACHINE, key, true);
    //return regKey.getString(REGISTRY_DESCRIPTION);
}


void WinService::open() const
{
    if (!tryOpen())
        throw NotFoundException("service does not exist", convert_to<string8>(_name));
}


bool WinService::tryOpen() const
{
    if (!_svcHandle) {
        _svcHandle = OpenServiceW(_scmHandle, _name.c_str(), SERVICE_ALL_ACCESS);
    }

    return _svcHandle != 0;
}


void WinService::close() const
{
    if (_svcHandle) {
        CloseServiceHandle(_svcHandle);
        _svcHandle = 0;
    }
}


LPQUERY_SERVICE_CONFIGW WinService::config() const
{
    open();
    int size = 4096;
    DWORD bytesNeeded;
    LPQUERY_SERVICE_CONFIGW pSvcConfig = (LPQUERY_SERVICE_CONFIGW) LocalAlloc(LPTR, size);

    if (!pSvcConfig) throw OutOfMemoryException("cannot allocate service config buffer");

    try {
        while (!QueryServiceConfigW(_svcHandle, pSvcConfig, size, &bytesNeeded)) {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                LocalFree(pSvcConfig);
                size = bytesNeeded;
                pSvcConfig = (LPQUERY_SERVICE_CONFIGW) LocalAlloc(LPTR, size);
            } else throw SystemException("cannot query service configuration", convert_to<string8>(_name));
        }
    } catch (...) {
        LocalFree(pSvcConfig);
        throw;
    }

    return pSvcConfig;
}

} // namespace cactus