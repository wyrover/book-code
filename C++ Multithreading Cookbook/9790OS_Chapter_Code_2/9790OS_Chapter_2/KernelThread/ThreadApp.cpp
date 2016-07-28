#include "ThreadApp.h"

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCommandLine, int iCmdShow)
{
    StartDriver(DRIVER_NAME);
    ShellAbout(0, DRIVER_SERVICE_NAME, TEXT(""), NULL);
    StopDriver();
    return 0;
}
BOOL StartDriver(LPTSTR szCurrentDriver)
{
    HANDLE          hFile = 0;
    DWORD           dwReturn = 0;
    SC_HANDLE       hSCManager = { 0 };
    SC_HANDLE       hService = { 0 };
    SERVICE_STATUS  ServiceStatus = { 0 };
    TCHAR           szDriverPath[MAX_PATH] = { 0 };
    GetSystemDirectory(szDriverPath, MAX_PATH);
    TCHAR szDriver[MAX_PATH + 1];
#ifdef _UNICODE
    wsprintf(szDriver, L"\\drivers\\%ws", DRIVER_NAME);
#else
    sprintf(szDriver, "\\drivers\\%s", DRIVER_NAME);
#endif
    _tcscat_s(szDriverPath, (_tcslen(szDriver) + 1) * sizeof(TCHAR), szDriver);
    BOOL bSuccess = CopyFile(szCurrentDriver, szDriverPath, FALSE);

    if (bSuccess == FALSE) {
        Message("copy driver failed");
        return bSuccess;
    }

    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    if (hSCManager == 0) {
        Message("open sc manager failed!");
        return FALSE;
    }

    hService = CreateService(hSCManager, DRIVER_SERVICE_NAME, DRIVER_SERVICE_NAME, SERVICE_START | DELETE | SERVICE_STOP, SERVICE_KERNEL_DRIVER, SERVICE_DEMAND_START, SERVICE_ERROR_IGNORE, szDriverPath, NULL, NULL, NULL, NULL, NULL);

    if (hService == 0) {
        hService = OpenService(hSCManager, DRIVER_SERVICE_NAME, SERVICE_START | DELETE | SERVICE_STOP);
        Message("create service failed!");
    }

    if (hService == 0) {
        Message("open service failed!");
        return FALSE;
    }

    BOOL startSuccess = StartService(hService, 0, NULL);

    if (startSuccess == FALSE) {
        Message("start service failed!");
        return startSuccess;
    }

    CloseHandle(hFile);
    return TRUE;
}
BOOL StopDriver(void)
{
    SC_HANDLE       hSCManager = { 0 };
    SC_HANDLE       hService = { 0 };
    SERVICE_STATUS  ServiceStatus = { 0 };
    TCHAR           szDriverPath[MAX_PATH] = { 0 };
    GetSystemDirectory(szDriverPath, MAX_PATH);
    TCHAR szDriver[MAX_PATH + 1];
#ifdef _UNICODE
    wsprintf(szDriver, L"\\drivers\\%ws", DRIVER_NAME);
#else
    sprintf(szDriver, "\\drivers\\%s", DRIVER_NAME);
#endif
    _tcscat_s(szDriverPath, (_tcslen(szDriver) + 1) * sizeof(TCHAR), szDriver);
    hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE);

    if (hSCManager == 0) {
        return FALSE;
    }

    hService = OpenService(hSCManager, DRIVER_SERVICE_NAME, SERVICE_START | DELETE | SERVICE_STOP);

    if (hService) {
        ControlService(hService, SERVICE_CONTROL_STOP, &ServiceStatus);
        DeleteService(hService);
        CloseServiceHandle(hService);
        BOOL ifSuccess = DeleteFile(szDriverPath);
        return TRUE;
    }

    return FALSE;
}
