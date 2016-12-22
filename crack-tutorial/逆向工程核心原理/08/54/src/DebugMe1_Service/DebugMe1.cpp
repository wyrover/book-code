#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define SVCNAME L"SvcTest"

VOID InstallService(LPCTSTR szSvcName, LPCTSTR szPath);
VOID UninstallService(LPCTSTR szSvcName);
VOID WINAPI SvcMain(DWORD argc, LPCTSTR *argv);
VOID WINAPI SvcCtrlHandler( DWORD dwCtrl );

SERVICE_STATUS_HANDLE g_hServiceStatusHandle = NULL;
SERVICE_STATUS g_ServiceStatus = {SERVICE_WIN32_OWN_PROCESS, 0, 0xFF, 0, 0, 0, 0};

void _tmain(int argc, TCHAR *argv[])
{ 
    TCHAR szPath[MAX_PATH] = {0,};
    SERVICE_TABLE_ENTRY DispatchTable[] = 
    { 
        { SVCNAME, (LPSERVICE_MAIN_FUNCTION)SvcMain }, 
        { NULL, NULL } 
    }; 

    if( argc == 1 )
    {
        if (!StartServiceCtrlDispatcher( DispatchTable )) 
        { 
            _tprintf(L"StartServiceCtrlDispatcher() failed!!! [%d]\n", 
                GetLastError()); 
        }
    }
    else if( argc == 2 )
    {
        if( !GetModuleFileName(NULL, szPath, MAX_PATH) )
        {
            _tprintf(L"GetModuleFileName() failed! [%d]\n", 
                GetLastError());
            return;
        }

        if( _tcsicmp(argv[1], L"install") == 0 )
        {
            InstallService(SVCNAME, szPath);
            return;
        }
        else if( _tcsicmp(argv[1], L"uninstall") == 0 )
        {
            UninstallService(SVCNAME);
            return;
        }
        else
        {
            _tprintf(L"Wrong parameters!!!\n");
        }
    }

    _tprintf(L"\nUSAGE : %s <install | uninstall>\n", argv[0]);
}

VOID InstallService(LPCTSTR szSvcName, LPCTSTR szPath)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;
    DWORD dwError = 0;

    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
    if( NULL == schSCManager ) 
    {
        _tprintf(L"InstallService() : OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    schService = CreateService( 
        schSCManager,              // SCM database 
        szSvcName,                 // name of service 
        szSvcName,                 // service name to display 
        SERVICE_ALL_ACCESS,        // desired access 
        SERVICE_WIN32_OWN_PROCESS, // service type 
        SERVICE_DEMAND_START,      // start type 
        SERVICE_ERROR_NORMAL,      // error control type 
        szPath,                    // path to service's binary 
        NULL,                      // no load ordering group 
        NULL,                      // no tag identifier 
        NULL,                      // no dependencies 
        NULL,                      // LocalSystem account 
        NULL);                     // no password 
    if( NULL == schService ) 
    {
        dwError = GetLastError();
        _tprintf(L"InstallService() : CreateService failed (%d)\n", dwError); 
        if( ERROR_SERVICE_EXISTS == dwError )
            _tprintf(L"  -> The specified service already exists.\n");
        goto _EXIT;
    }

    _tprintf(L"InstallService() : Service installed successfully\n"); 

_EXIT:
    if( schService )    CloseServiceHandle(schService); 
    if( schSCManager)   CloseServiceHandle(schSCManager);
}

VOID UninstallService(LPCTSTR szSvcName)
{
    SC_HANDLE schSCManager = NULL;
    SC_HANDLE schService = NULL;
    SERVICE_STATUS ss = {0,};
    DWORD dwError = 0;

    schSCManager = OpenSCManager( 
        NULL,                    // local computer
        NULL,                    // ServicesActive database 
        SC_MANAGER_ALL_ACCESS);  // full access rights 
    if( NULL == schSCManager ) 
    {
        _tprintf(L"UninstallService() : OpenSCManager failed (%d)\n", GetLastError());
        return;
    }

    schService = OpenService( 
        schSCManager,       // SCM database 
        szSvcName,          // name of service 
        SERVICE_INTERROGATE |
        DELETE);            // need delete access 
    if( NULL == schService )
    { 
        dwError = GetLastError();
        if( dwError != ERROR_SERVICE_DOES_NOT_EXIST )
            _tprintf(L"UninstallService() : OpenSCManager failed (%d)\n", dwError);

        goto _EXIT;
    }
        
    ControlService(schService, SERVICE_CONTROL_INTERROGATE, &ss);
    if( ss.dwCurrentState != SERVICE_STOPPED )
    {
        _tprintf(L"  -> Service is running! Stop the service!!!\n");
        goto _EXIT;
    }

    if( !DeleteService(schService) ) 
        _tprintf(L"UninstallService() : DeleteService failed (%d)\n", GetLastError()); 
    else
        _tprintf(L"Service uninstalled successfully\n"); 

_EXIT:
    if( schService )    CloseServiceHandle(schService); 
    if( schSCManager )  CloseServiceHandle(schSCManager);
}

VOID WINAPI SvcMain(DWORD argc, LPCTSTR *argv)
{
    // Service Control Handler
    g_hServiceStatusHandle = RegisterServiceCtrlHandler( 
        SVCNAME, 
        SvcCtrlHandler);
    if( !g_hServiceStatusHandle )
    { 
        OutputDebugString(L"RegisterServiceCtrlHandler() failed!!!"); 
        return; 
    } 

    // Service Status -> SERVICE_RUNNING
    g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
    SetServiceStatus(g_hServiceStatusHandle, &g_ServiceStatus);

    // Print debug string
    while( TRUE )
    {
        OutputDebugString(L"[SvcTest] service is running...");
        Sleep(3 * 1000);        // 3 sec
    }
}

VOID WINAPI SvcCtrlHandler(DWORD dwCtrl)
{
   switch(dwCtrl) 
   {  
        case SERVICE_CONTROL_STOP:
            g_ServiceStatus.dwCurrentState = SERVICE_STOP_PENDING;
            SetServiceStatus(g_hServiceStatusHandle, &g_ServiceStatus);
            
            g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            SetServiceStatus(g_hServiceStatusHandle, &g_ServiceStatus);

            OutputDebugString(L"[SvcTest] service is stopped...");
        break;
 
      default: 
         break;
   }
}
