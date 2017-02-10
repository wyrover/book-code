#include "stdafx.h"

#define SERVICE_NAME L"ServiceWithCrash"

class CSystemService
{
public:
    static
    VOID
    WINAPI
    ServiceMain(
        __in DWORD dwArgc,
        __in_ecount(dwArgc) LPTSTR* lpszArgv
        )
    {
        UNREFERENCED_PARAMETER(dwArgc);
        UNREFERENCED_PARAMETER(lpszArgv);

        ChkProlog();

        g_ServiceStatus.dwServiceType = SERVICE_WIN32_OWN_PROCESS;
        g_hServiceStatusHandle = RegisterServiceCtrlHandlerEx(
            SERVICE_NAME,
            ServiceHandlerEx,
            NULL
            );

        //
        // Report start pending...
        //
        g_ServiceStatus.dwCurrentState = SERVICE_START_PENDING;
        ChkWin32(SetServiceStatus(
            g_hServiceStatusHandle,
            &g_ServiceStatus
            ));

        //
        // Do some initialization work before reporting "running"
        //
        ChkHr(Initialize());

        //
        // Report running
        //
        g_ServiceStatus.dwCurrentState = SERVICE_RUNNING;
        g_ServiceStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
        ChkWin32(SetServiceStatus(
            g_hServiceStatusHandle,
            &g_ServiceStatus
            ));

        ChkCleanup();

        if (FAILED(ChkGetHr()))
        {
            g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
            g_ServiceStatus.dwWin32ExitCode = HRESULT_CODE(ChkGetHr());
            (VOID)SetServiceStatus(
                g_hServiceStatusHandle,
                &g_ServiceStatus
                );
        }

        ChkEpilogVoid();
    }

private:
    static
    DWORD
    WINAPI
    ServiceHandlerEx(
        __in DWORD dwControl,
        __in DWORD dwEventType,
        __in LPVOID lpEventData,
        __in LPVOID lpContext
        )
    {
        UNREFERENCED_PARAMETER(dwEventType);
        UNREFERENCED_PARAMETER(lpEventData);
        UNREFERENCED_PARAMETER(lpContext);

        ChkProlog();

        switch (dwControl)
        {
            case SERVICE_CONTROL_SHUTDOWN:
            case SERVICE_CONTROL_STOP:
                //
                // Stop the service
                //
                g_ServiceStatus.dwCurrentState = SERVICE_STOPPED;
                g_ServiceStatus.dwControlsAccepted = 0;
                ChkWin32(SetServiceStatus(
                    g_hServiceStatusHandle,
                    &g_ServiceStatus
                    ));
        }

        ChkNoCleanupRet(0);
    }

    static
    HRESULT
    Initialize(
        VOID
        )
    {
        BYTE* pMem = NULL;

        ChkProlog();

        pMem[0] = 5;
        wprintf(L"Value at memory address is 0x%x\n", pMem[0]);

        ChkNoCleanup();
    }

    static SERVICE_STATUS_HANDLE g_hServiceStatusHandle;
    static SERVICE_STATUS g_ServiceStatus;
};

SERVICE_STATUS_HANDLE CSystemService::g_hServiceStatusHandle = NULL;
SERVICE_STATUS CSystemService::g_ServiceStatus = {0};

class CMainApp
{
public:
    HRESULT
    MainHR(
        VOID
        )
    {
        const SERVICE_TABLE_ENTRY pServiceTable[] =
        {
            {SERVICE_NAME, CSystemService::ServiceMain},
            {NULL, NULL}, 
        };

        ChkProlog();

        ChkWin32(StartServiceCtrlDispatcher(
            pServiceTable
            ));

        ChkNoCleanup();
    }
};

int
__cdecl
wmain(
    VOID
    )
{
    HRESULT hr;
    CMainApp app;

    hr = app.MainHR();

    if (FAILED(hr))
    {
        wprintf(L"HRESULT: 0x%08X\n", hr);
    }
    else
    {
        wprintf(L"Success.\n");
    }

    return 0;
}
