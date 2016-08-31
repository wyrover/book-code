#include "reboot.h"
#include <tchar.h>
#include <powrprof.h>

bool shutDown(int action)
{
    // Prepare for shutdown
    // [-] IRainman old code:
    //UINT iForceIfHung = 0;
    //OSVERSIONINFO osvi = {0};
    //osvi.dwOSVersionInfoSize = sizeof(osvi);
    //if (GetVersionEx(&osvi) != 0 && osvi.dwPlatformId == VER_PLATFORM_WIN32_NT)
    //{
    UINT iForceIfHung = 0x00000010;
    HANDLE hToken;
    OpenProcessToken(GetCurrentProcess(), (TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY), &hToken);
    LUID luid;
    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &luid);
    TOKEN_PRIVILEGES tp;
    tp.PrivilegeCount = 1;
    tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    tp.Privileges[0].Luid = luid;
    AdjustTokenPrivileges(hToken, FALSE, &tp, 0, (PTOKEN_PRIVILEGES)NULL, (PDWORD)NULL);
    CloseHandle(hToken);
    //}

    // Shutdown
    switch (action) {
    case 0: {
        action = EWX_POWEROFF;
        break;
    }

    case 1: {
        action = EWX_LOGOFF;
        break;
    }

    case 2: {
        action = EWX_REBOOT;
        break;
    }

    case 3: {
        SetSuspendState(false, false, false);
        return true;
    }

    case 4: {
        SetSuspendState(true, false, false);
        return true;
    }

    case 5: {
#ifdef FLYLINKDC_SUPPORT_WIN_2000

        if (LOBYTE(LOWORD(GetVersion())) >= 5)
#endif
        {
            typedef bool (CALLBACK * LPLockWorkStation)(void);
            LPLockWorkStation _d_LockWorkStation = (LPLockWorkStation)GetProcAddress(LoadLibrary(_T("user32")), "LockWorkStation");

            if (_d_LockWorkStation) {
                _d_LockWorkStation();
            }
        }

        return true;
    }
    }

    if (ExitWindowsEx(action | iForceIfHung, 0) == 0) {
        return false;
    } else {
        return true;
    }
}