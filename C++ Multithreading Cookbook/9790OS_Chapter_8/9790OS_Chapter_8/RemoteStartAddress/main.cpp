#include <Windows.h>
#include <tchar.h>
#include "..\common.h"

typedef struct {
    DWORD dwProcessId;
    HWND hWnd;
} WINDOW_INFORMATION, *PWINDOW_INFORMATION;

BOOL IsMainWindow(HWND hWnd)
{
    return GetWindow(hWnd, GW_OWNER) == (HWND)0 && IsWindowVisible(hWnd);
}

BOOL CALLBACK EnumWindowsCallback(HWND hWnd, LPARAM lParam)
{
    PWINDOW_INFORMATION pWindowInformation = (PWINDOW_INFORMATION)lParam;
    DWORD dwProcessId = 0;
    GetWindowThreadProcessId(hWnd, &dwProcessId);

    if (pWindowInformation->dwProcessId == dwProcessId && IsMainWindow(hWnd)) {
        pWindowInformation->hWnd = hWnd;
        return FALSE;
    }

    return TRUE;
}

HWND FindMainWindow()
{
    WINDOW_INFORMATION wndInfo = { GetCurrentProcessId(), 0 };
    EnumWindows(EnumWindowsCallback, (LPARAM)&wndInfo);
    return wndInfo.hWnd;
}

void ChangeWindowTitle()
{
    HWND hWnd = FindMainWindow();

    if (hWnd) {
        SetWindowText(hWnd, _T("Remotely started thread inside calculator!"));
    }
}

BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
    switch (dwReason) {
    case DLL_PROCESS_ATTACH: {
        HANDLE hMapping = OpenFileMapping(FILE_MAP_ALL_ACCESS, FALSE, MAPPING_NAME);

        if (hMapping) {
            PCOMM_OBJECT pCommObject = (PCOMM_OBJECT)MapViewOfFile(hMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);

            if (pCommObject) {
                pCommObject->hLibrary = hInstance;
            }

            CloseHandle(hMapping);
        }

        ChangeWindowTitle();
        HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);

        if (hEvent) {
            SetEvent(hEvent);
            CloseHandle(hEvent);
        }

        break;
    }

    case DLL_PROCESS_DETACH: {
        //
        break;
    }
    }

    return TRUE;
}
