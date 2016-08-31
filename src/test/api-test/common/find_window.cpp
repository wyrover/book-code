#include "find_window.h"

using namespace cactus;

// Enum data
struct EnumData {
    DWORD dwProcessId;
    HWND hWnd;
};

// Callback for EnumWindows
BOOL CALLBACK EnumProc(HWND hWnd, LPARAM lParam)
{
    // Retrieve storage location for communication data
    EnumData& ed = *(EnumData*)lParam;
    DWORD dwProcessId = 0x0;
    // Query process ID for hWnd
    GetWindowThreadProcessId(hWnd, &dwProcessId);

    // Apply filter - if you want to implement additional restrictions,
    // this is the place to do so.
    if (ed.dwProcessId == dwProcessId) {
        // Found a window matching the process ID
        ed.hWnd = hWnd;
        // Report success
        SetLastError(ERROR_SUCCESS);
        // Stop enumeration
        return FALSE;
    }

    // Continue enumeration
    return TRUE;
}

// Find window from process id
HWND FindWindow::FindWindowFromProcessId(DWORD dwProcessId)
{
    EnumData ed = { dwProcessId };

    if (!EnumWindows(EnumProc, (LPARAM)&ed) &&
        (GetLastError() == ERROR_SUCCESS)) {
        return ed.hWnd;
    }

    return NULL;
}

// Find window from process handle
HWND FindWindow::FindWindowFromProcess(HANDLE hProcess)
{
    return FindWindowFromProcessId(GetProcessId(hProcess));
}