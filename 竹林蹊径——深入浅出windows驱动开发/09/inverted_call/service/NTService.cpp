// NTService.cpp
//
// Implementation of CNTService

#include "stdafx.h"
#include "NTService.h"

// static variables
CNTService* CNTService::m_pThis = NULL;

CNTService::CNTService(const WCHAR* szServiceName,const WCHAR* szServiceDescription) : m_bDebugging(FALSE)
{
    // copy the address of the current object so we can access it from
    // the static member callback functions. 
    // WARNING: This limits the application to only one CNTService object. 
    m_pThis = this;
    
    // Set the default service name and version
    memset(m_szServiceName,0,sizeof(m_szServiceName));
    wcsncpy(m_szServiceName, szServiceName, wcslen(szServiceName));
    memset(m_szServiceDescription,0,sizeof(m_szServiceDescription));
    wcsncpy(m_szServiceDescription, szServiceDescription, wcslen(szServiceDescription));
    m_iMajorVersion = 1;
    m_iMinorVersion = 0;
    m_hEventSource = NULL;

    // set up the initial service status 
    m_hServiceStatus = NULL;
    m_Status.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
    m_Status.dwCurrentState = SERVICE_STOPPED;
    m_Status.dwControlsAccepted = SERVICE_ACCEPT_STOP;
    m_Status.dwWin32ExitCode = 0;
    m_Status.dwServiceSpecificExitCode = 0;
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;
    m_bIsRunning = FALSE;
}

CNTService::~CNTService()
{
    DebugMsg(L"CNTService::~CNTService()");
    if (m_hEventSource) {
        ::DeregisterEventSource(m_hEventSource);
    }
}

////////////////////////////////////////////////////////////////////////////////////////
// Default command line argument parsing

// Returns TRUE if it found an arg it recognised, FALSE if not
// Note: processing some arguments causes output to stdout to be generated.
BOOL CNTService::ParseStandardArgs(int argc, WCHAR* argv[])
{
    // See if we have any command line args we recognise
    if (argc <= 1) return FALSE;

    if (_wcsicmp(argv[1], L"-v") == 0) {

        // Spit out version info
        wprintf(L"%ls Version %d.%d\n",
               &m_szServiceName[0], m_iMajorVersion, m_iMinorVersion);
        wprintf(L"The service is %s installed\n",
               IsInstalled() ? L"currently" : L"not");
        return TRUE; // say we processed the argument

    } else if (_wcsicmp(argv[1], L"-i") == 0) {

        // Request to install.
        if (IsInstalled()) {
            wprintf(L"%ls is already installed\n", &m_szServiceName[0]);
        } else {
            // Try and install the copy that's running
            if (Install()) {
                wprintf(L"%ls installed\n", &m_szServiceName[0]);
            } else {
                wprintf(L"l%s failed to install. Error %d\n", &m_szServiceName[0], GetLastError());
            }
        }
        return TRUE; // say we processed the argument

    } else if (_wcsicmp(argv[1], L"-u") == 0) {

        // Request to uninstall.
        if (!IsInstalled()) {
            wprintf(L"%ls is not installed\n", &m_szServiceName[0]);
        } else {
            // Try and remove the copy that's installed
            if (Uninstall()) {
                // Get the executable file path
                WCHAR szFilePath[_MAX_PATH];
                ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));
                wprintf(L"%ls removed. (You must delete the file (%s) yourself.)\n",
                       &m_szServiceName[0], szFilePath);
            } else {
                wprintf(L"Could not remove %ls. Error %d\n", &m_szServiceName[0], GetLastError());
            }
        }
        return TRUE; // say we processed the argument
    
    } 
    
    // Don't recognise the args
    return FALSE;
}


////////////////////////////////////////////////////////////////////////////////////////
// Install/uninstall routines

// Test if the service is currently installed
BOOL CNTService::IsInstalled()
{
    BOOL bResult = FALSE;

    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (hSCM) {

        // Try to open the service
        SC_HANDLE hService = ::OpenService(hSCM,
                                           m_szServiceName,
                                           SERVICE_QUERY_CONFIG);
        if (hService) {
            bResult = TRUE;
            ::CloseServiceHandle(hService);
        }

        ::CloseServiceHandle(hSCM);
    }
    
    return bResult;
}

BOOL CNTService::Install()
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    // Get the executable file path
    WCHAR szFilePath[_MAX_PATH];
    ::GetModuleFileName(NULL, szFilePath, sizeof(szFilePath));

    // Create the service
    SC_HANDLE hService = ::CreateService(hSCM,
                                         m_szServiceName,
                                         m_szServiceName,
                                         SERVICE_ALL_ACCESS,
                                         SERVICE_WIN32_OWN_PROCESS,
                                         SERVICE_DEMAND_START,        // start condition
                                         SERVICE_ERROR_NORMAL,
                                         szFilePath,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL,
                                         NULL);
    if (!hService) {
        ::CloseServiceHandle(hSCM);
        return FALSE;
    }

    // make registry entries to support logging messages
    // Add the source name as a subkey under the Application
    // key in the EventLog service portion of the registry.
    WCHAR szKey[256];
    HKEY hKey = NULL;
    wcscpy(szKey, L"SYSTEM\\CurrentControlSet\\Services\\EventLog\\Application\\");
    wcscat(szKey, m_szServiceName);
    if (::RegCreateKey(HKEY_LOCAL_MACHINE, szKey, &hKey) != ERROR_SUCCESS) {
        ::CloseServiceHandle(hService);
        ::CloseServiceHandle(hSCM);
        return FALSE;
    }

    // Add the Event ID message-file name to the 'EventMessageFile' subkey.
    ::RegSetValueEx(hKey,
                    L"EventMessageFile",
                    0,
                    REG_EXPAND_SZ, 
                    (CONST BYTE*)szFilePath,
                    wcslen(szFilePath) + sizeof(WCHAR));     

    // Set the supported types flags.
    DWORD dwData = EVENTLOG_ERROR_TYPE | EVENTLOG_WARNING_TYPE | EVENTLOG_INFORMATION_TYPE;
    ::RegSetValueEx(hKey,
                    L"TypesSupported",
                    0,
                    REG_DWORD,
                    (CONST BYTE*)&dwData,
                     sizeof(DWORD));
    ::RegCloseKey(hKey);

    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_INSTALLED, m_szServiceName);

    // Add our description
    wcscpy(szKey,L"SYSTEM\\CurrentControlSet\\Services\\");
    wcscat(szKey,m_szServiceName);

    if(::RegOpenKey(HKEY_LOCAL_MACHINE, szKey, &hKey) == ERROR_SUCCESS) {

        ::RegSetValueEx(hKey,
                        L"Description",
                        0,
                        REG_SZ, 
                        (CONST BYTE*) m_szServiceDescription,
                        (wcslen(m_szServiceDescription) * sizeof(WCHAR)) + sizeof(WCHAR));     

        ::RegCloseKey(hKey);
    }

    // tidy up
    ::CloseServiceHandle(hService);
    ::CloseServiceHandle(hSCM);
    return TRUE;
}

BOOL CNTService::Uninstall()
{
    // Open the Service Control Manager
    SC_HANDLE hSCM = ::OpenSCManager(NULL, // local machine
                                     NULL, // ServicesActive database
                                     SC_MANAGER_ALL_ACCESS); // full access
    if (!hSCM) return FALSE;

    BOOL bResult = FALSE;
    SC_HANDLE hService = ::OpenService(hSCM,
                                       m_szServiceName,
                                       DELETE);
    if (hService) {
        if (::DeleteService(hService)) {
            LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_REMOVED, m_szServiceName);
            bResult = TRUE;
        } else {
            LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_NOTREMOVED, m_szServiceName);
        }
        ::CloseServiceHandle(hService);
    }
    
    ::CloseServiceHandle(hSCM);
    return bResult;
}

///////////////////////////////////////////////////////////////////////////////////////
// Logging functions

// This function makes an entry into the application event log
void CNTService::LogEvent(WORD wType, DWORD dwID,
                          const WCHAR* pszS1,
                          const WCHAR* pszS2,
                          const WCHAR* pszS3)
{
    const WCHAR* ps[3];
    ps[0] = pszS1;
    ps[1] = pszS2;
    ps[2] = pszS3;

    int iStr = 0;
    for (int i = 0; i < 3; i++) {
        if (ps[i] != NULL) iStr++;
    }
        
    // Check the event source has been registered and if
    // not then register it now
    if (!m_hEventSource) {
        m_hEventSource = ::RegisterEventSource(NULL,  // local machine
                                               m_szServiceName); // source name
    }

    if (m_hEventSource) {
        ::ReportEvent(m_hEventSource,
                      wType,
                      0,
                      dwID,
                      NULL, // sid
                      (WORD) iStr,
                      0,
                      ps,
                      NULL);
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////
// Service startup and registration

BOOL CNTService::StartService()
{
    SERVICE_TABLE_ENTRY st[] = {
        {m_szServiceName, ServiceMain},
        {NULL, NULL}
    };

    DebugMsg(L"Calling StartServiceCtrlDispatcher()");
    BOOL b = ::StartServiceCtrlDispatcher(st);
    if(!b) {
        DebugMsg(L"Error starting %x.",GetLastError());
    }
    DebugMsg(L"Returned from StartServiceCtrlDispatcher()");
    return b;
}

// static member function (callback)
void CNTService::ServiceMain(DWORD dwArgc, LPTSTR* lpszArgv)
{
    // Get a pointer to the C++ object
    CNTService* pService = m_pThis;
    
    pService->DebugMsg(L"Entering CNTService::ServiceMain()");
    // Register the control request handler
    pService->m_Status.dwCurrentState = SERVICE_START_PENDING;

#ifndef HANDLEREX
    pService->m_hServiceStatus = RegisterServiceCtrlHandler(pService->m_szServiceName,
                                                           Handler);
#else   HANDLEREX
    pService->m_hServiceStatus = RegisterServiceCtrlHandlerEx(pService->m_szServiceName,HandlerEx,NULL);
#endif  HANDLEREX

    if (pService->m_hServiceStatus == NULL) {
		pService->DebugMsg(L"CNTService::ServiceMain() Service Not Registered");
        pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_CTRLHANDLERNOTINSTALLED);
        return;
    }

	  pService->DebugMsg(L"CNTService::ServiceMain() Service Registered");

    // Start the initialisation
    if (pService->Initialize()) {

        // Do the real work. 
        // When the Run function returns, the service has stopped.
        pService->m_bIsRunning = TRUE;
        pService->m_Status.dwWin32ExitCode = 0;
        pService->m_Status.dwCheckPoint = 0;
        pService->m_Status.dwWaitHint = 0;
        pService->Run();
    }

    // Tell the service manager we are stopped
    pService->SetStatus(SERVICE_STOPPED);

    pService->DebugMsg(L"Leaving CNTService::ServiceMain()");
}

///////////////////////////////////////////////////////////////////////////////////////////
// status functions

void CNTService::SetStatus(DWORD dwState)
{
    DebugMsg(L"CNTService::SetStatus(%lu, %lu)", m_hServiceStatus, dwState);
    m_Status.dwCurrentState = dwState;
    ::SetServiceStatus(m_hServiceStatus, &m_Status);
}

///////////////////////////////////////////////////////////////////////////////////////////
// Service initialization

BOOL CNTService::Initialize()
{
    DebugMsg(L"Entering CNTService::Initialize()");

    // Start the initialization
    SetStatus(SERVICE_START_PENDING);
    
    // Perform the actual initialization
    BOOL bResult = OnInit(); 
    
    // Set final state
    m_Status.dwWin32ExitCode = GetLastError();
    m_Status.dwCheckPoint = 0;
    m_Status.dwWaitHint = 0;
    if (!bResult) {
        LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_FAILEDINIT);
        SetStatus(SERVICE_STOPPED);
        return FALSE;    
    }
    
    LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STARTED);
    SetStatus(SERVICE_RUNNING);

    DebugMsg(L"Leaving CNTService::Initialize()");
    return TRUE;
}

///////////////////////////////////////////////////////////////////////////////////////////////
// main function to do the real work of the service

// This function performs the main work of the service. 
// When this function returns the service has stopped.
void CNTService::Run()
{
    DebugMsg(L"Entering CNTService::Run()");

    while (m_bIsRunning) {
        DebugMsg(L"Sleeping...");
        Sleep(5000);
    }

    // nothing more to do
    DebugMsg(L"Leaving CNTService::Run()");
}

//////////////////////////////////////////////////////////////////////////////////////
// Control request handlers

// static member function (callback) to handle commands from the
// service control manager
#ifndef HANDLEREX
void CNTService::Handler(DWORD dwOpcode)
#else   HANDLEREX
DWORD CNTService::HandlerEx(DWORD dwOpcode,DWORD dwEventType,LPVOID lpEventData,LPVOID lpContext)
#endif  HANDLEREX
{
    // Get a pointer to the object
    CNTService* pService = m_pThis;
    
    pService->DebugMsg(L"CNTService::Handler(%lu)", dwOpcode);
    switch (dwOpcode) {
    case SERVICE_CONTROL_STOP: // 1
        pService->SetStatus(SERVICE_STOP_PENDING);
        pService->OnStop();
        pService->m_bIsRunning = FALSE;
        pService->LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_STOPPED);
        break;

    case SERVICE_CONTROL_PAUSE: // 2
        pService->OnPause();
        break;

    case SERVICE_CONTROL_CONTINUE: // 3
        pService->OnContinue();
        break;

    case SERVICE_CONTROL_INTERROGATE: // 4
        pService->OnInterrogate();
        break;

    case SERVICE_CONTROL_SHUTDOWN: // 5
        pService->OnShutdown();
        break;

    case SERVICE_CONTROL_DEVICEEVENT:
        pService->OnDeviceEvent(dwEventType,lpEventData);
        break;

    default:
        if (dwOpcode >= SERVICE_CONTROL_USER) {
            if (!pService->OnUserControl(dwOpcode)) {
                pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
            }
        } else {
            pService->LogEvent(EVENTLOG_ERROR_TYPE, EVMSG_BADREQUEST);
        }
        break;
    }

    // Report current status
    pService->DebugMsg(L"Updating status (%lu, %lu)",
                       pService->m_hServiceStatus,
                       pService->m_Status.dwCurrentState);
    ::SetServiceStatus(pService->m_hServiceStatus, &pService->m_Status);

#ifdef HANDLEREX
    return NO_ERROR;
#endif HANDLEREX
}
        
// Called when the service is first initialized
BOOL CNTService::OnInit()
{
    DebugMsg(L"CNTService::OnInit()");
	  return TRUE;
}

// Called when the service control manager wants to stop the service
void CNTService::OnStop()
{
    DebugMsg(L"CNTService::OnStop()");
}

// called when the service is interrogated
void CNTService::OnInterrogate()
{
    DebugMsg(L"CNTService::OnInterrogate()");
}

// called when the service is paused
void CNTService::OnPause()
{
    DebugMsg(L"CNTService::OnPause()");
}

// called when the service is continued
void CNTService::OnContinue()
{
    DebugMsg(L"CNTService::OnContinue()");
}

// called when the service is shut down
void CNTService::OnShutdown()
{
    DebugMsg(L"CNTService::OnShutdown()");
}

void CNTService::OnDeviceEvent(DWORD dwEventType,LPVOID lpEventData)
{
    DebugMsg(L"CNTService::OnDeviceEvent(DWORD dwEventType,LPVOID lpEventData)");
}

// called when the service gets a user control message
BOOL CNTService::OnUserControl(DWORD dwOpcode)
{
    DebugMsg(L"CNTService::OnUserControl(%8.8lXH)", dwOpcode);
    return FALSE; // say not handled
}


////////////////////////////////////////////////////////////////////////////////////////////
// Debugging support

void CNTService::DebugMsg(const WCHAR* pszFormat, ...)
{
    WCHAR buf[1024];
    swprintf(buf, L"[%ls](%lu): ", m_szServiceName, GetCurrentThreadId());
	  va_list arglist;
	  va_start(arglist, pszFormat);
    vswprintf(&buf[wcslen(buf)], pszFormat, arglist);
	  va_end(arglist);
    wcscat(buf, L"\n");
    OutputDebugString(buf);
}
