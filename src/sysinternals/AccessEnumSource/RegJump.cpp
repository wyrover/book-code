#include <windows.h>
#include <tchar.h>
#include <ShellAPI.h>

#include "AccessEnum.h"
#include "Enumeration.h"


/******************************************************************************
*
*   FUNCTION:   RegeditJump
*
*   PURPOSE:    Opens Regedit and navigates the desired key
*
*****************************************************************************/
void RegeditJump(HWND hWnd, const TCHAR * path)
{
    TCHAR       *   ch;
    TCHAR       *   value = NULL;
    TCHAR           RegPath[ MAX_PATH ] = _T("\\");
    const TCHAR *   FullName = NULL;
    // convert to full path
    HKEY hKey = CRegEnumeration::OpenPath(path, &FullName);

    if (hKey)
        RegCloseKey(hKey);

    // get full path name
    TCHAR * ptr = (TCHAR*)_tcschr(path, _T('\\'));

    if (ptr)
        ++ptr;
    else
        ptr = _T("");

    _stprintf(RegPath, _T("\\%s\\%s"), FullName, ptr);
    // Open RegEdit
    HWND regeditMainHwnd = FindWindow(_T("RegEdit_RegEdit"), NULL);

    if (regeditMainHwnd == NULL)  {
        SHELLEXECUTEINFO info;
        memset(&info, 0, sizeof info);
        info.cbSize = sizeof info;
        info.fMask  = SEE_MASK_NOCLOSEPROCESS;
        info.lpVerb = _T("open");
        info.lpFile = _T("regedit.exe");
        info.nShow  = SW_SHOWNORMAL;
        ShellExecuteEx(&info);
        WaitForInputIdle(info.hProcess, INFINITE);
        regeditMainHwnd = FindWindow(_T("RegEdit_RegEdit"), NULL);
    }

    if (regeditMainHwnd == NULL) {
        MessageBox(hWnd, APPNAME _T(" was unable to launch Regedit."), APPNAME, MB_OK | MB_ICONERROR);
        return;
    }

    ShowWindow(regeditMainHwnd, SW_SHOW);
    SetForegroundWindow(regeditMainHwnd);
    // Get treeview
    HWND regeditHwnd = FindWindowEx(regeditMainHwnd, NULL, _T("SysTreeView32"), NULL);
    SetForegroundWindow(regeditHwnd);
    SetFocus(regeditHwnd);
    DWORD   ProcessID = 0;
    GetWindowThreadProcessId(regeditHwnd, &ProcessID);
    HANDLE hProcess = OpenProcess(SYNCHRONIZE, false, ProcessID);

    // Close it up
    for (int pos = 0; pos < 30; ++pos)  {
        SendMessage(regeditHwnd, WM_KEYDOWN, VK_LEFT, 0);
    }

    // wait for slow displays
    WaitForInputIdle(hProcess, INFINITE);

    // Open path
    for (ch = RegPath; *ch; ++ch)  {
        if (*ch == '\\')  {
            UINT vk = VK_RIGHT;
            SendMessage(regeditHwnd, WM_KEYDOWN, vk, 0);
            // wait for slow displays
            WaitForInputIdle(hProcess, INFINITE);
        } else {
            UINT vk = toupper(*ch);
            SendMessage(regeditHwnd, WM_CHAR, vk, 0);
        }
    }

    SetForegroundWindow(regeditMainHwnd);
    SetFocus(regeditMainHwnd);
    CloseHandle(hProcess);
}

