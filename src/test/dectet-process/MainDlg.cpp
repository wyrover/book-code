// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"

#include "MainDlg.h"
#include "CProcessDetect.h"

using namespace pnpsub;

VOID WINAPI Notify(const char* process_name)
{
    TRACE_SEND_A(process_name);
}

int GetDisplayMonitorNumber()
{
    DISPLAY_DEVICE DisplayDevice;
    DEVMODE        dmDevMode;
    CString        strLog;
    DWORD i = 0;
    int   nCount = 0;
    ZeroMemory(&DisplayDevice, sizeof(DisplayDevice));
    ZeroMemory(&dmDevMode, sizeof(DEVMODE));
    DisplayDevice.cb = sizeof(DisplayDevice);
    dmDevMode.dmSize = sizeof(dmDevMode);
    HWINSTA hWinStaUser = ::OpenWindowStation(_T("WinSta0"), FALSE, MAXIMUM_ALLOWED);

    if (hWinStaUser != NULL) {
        if (::SetProcessWindowStation(hWinStaUser)) {
            HDESK hDesk = ::OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);

            if (hDesk != NULL) {
               // if (::SetThreadDesktop(hDesk)) {
                    for (i = 0; EnumDisplayDevices(NULL, i, &DisplayDevice, 0); i++) {
                        if (EnumDisplaySettings(DisplayDevice.DeviceName, ENUM_CURRENT_SETTINGS, &dmDevMode)) {
                            nCount++;
                        }
                    }
              //  }

                CloseDesktop(hDesk);
            }
        }

        ::CloseWindowStation(hWinStaUser);
    }

	TRACE_SEND_FORMAT("%d", nCount);
   
    return nCount;
}


LRESULT CMainDlg::OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
    // center the dialog on the screen
    CenterWindow();
    // set icons
    HICON hIcon = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
                                     IMAGE_ICON, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON), LR_DEFAULTCOLOR);
    SetIcon(hIcon, TRUE);
    HICON hIconSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDR_MAINFRAME),
                                          IMAGE_ICON, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON), LR_DEFAULTCOLOR);
    SetIcon(hIconSmall, FALSE);
    CProcessDetect::Instance().Init(Notify);
    return TRUE;
}

LRESULT CMainDlg::OnAppAbout(WORD /*wNotifyCode*/, WORD /*wID*/, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    CSimpleDialog<IDD_ABOUTBOX, FALSE> dlg;
    dlg.DoModal();
    return 0;
}

LRESULT CMainDlg::OnOK(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    // TODO: Add validation code
    EndDialog(wID);
    return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
    EndDialog(wID);
    return 0;
}
LRESULT CMainDlg::OnBnClickedButton1(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
{
    TRACE_MSGBOX_A("hello world!");
	GetDisplayMonitorNumber();
    return 0;
}

