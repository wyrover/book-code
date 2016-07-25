#include <windows.h>
#include <stdio.h>
#include <tchar.h>
#include "srvctmpl.h"

// Borland C++ v4.52 does not support VC-specific
// Unicode functions _tprintf, etc.
#ifdef __BORLANDC__
// therefore, don't compile this with Unicode enabled!
#   define  _tprintf    printf
#   define  _stprintf   sprintf
#endif

VOID main(VOID)
{
    SC_HANDLE  hSCManager = NULL, hService = NULL;
    TCHAR      szWinDir[MAX_PATH], szImagePath[MAX_PATH];

    if((hSCManager = OpenSCManager(NULL, NULL, 
        SC_MANAGER_CREATE_SERVICE)) == NULL) {
        _tprintf(TEXT("OpenSCManager Failed\n"));
	    return;
    }

    GetWindowsDirectory(szWinDir, MAX_PATH);
    _stprintf(szImagePath, TEXT("%s\\system32\\%s.exe"),
        szWinDir, GENSRV_ServiceName);

    if ((hService = CreateService(hSCManager,
        GENSRV_ServiceName,
        GENSRV_DisplayName, SERVICE_ALL_ACCESS, 
        SERVICE_WIN32_OWN_PROCESS, SERVICE_DEMAND_START,
        SERVICE_ERROR_IGNORE, szImagePath, NULL, NULL, 
        NULL, NULL, NULL)) == NULL) {
            _tprintf(TEXT("CreateService Failed, %d\n"),
            GetLastError());
            return;
    }

    CloseServiceHandle(hService);
    CloseServiceHandle(hSCManager);
    _tprintf(TEXT("%s installed successfully\n"),
        szImagePath);
}
