#include "StdAfx.h"
#include "MainWindow.h"
#include "DartBoard.h"

CMainWindow::CMainWindow(void)
{
}

CMainWindow::~CMainWindow(void)
{
}

///////////////////////////////////////////////////////////
//
// Window message handlers
//

LRESULT CMainWindow::OnCreate(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &bHandled)
{
    CRect rect;
    GetClientRect(&rect);
    CString controlName = L"AtlInternals2e.BullsEye";
    m_bullseyeWin.Create(m_hWnd, rect, controlName, WS_CHILD | WS_VISIBLE, 0, ID_BULLSEYE);
    CComPtr< IAxWinAmbientDispatch > ambient;
    m_bullseyeWin.QueryHost(&ambient);
    ambient->put_ForeColor(RGB(255, 255, 0));
    m_bullseyeControl.CoCreateInstance(__uuidof(BullsEye));
    m_bullseyeWin.AttachControl(m_bullseyeControl, 0);
    AtlAdviseSinkMap(this, true);
    m_bullseyeControl->put_RingCount(9);
    m_bullseyeControl->put_BackStyle(0);
    m_bullseyeControl->put_CenterColor(RGB(0, 0, 0));
    m_bullseyeControl->put_BackColor(RGB(0, 0, 255));
    //m_bullseyeControl->put_ForeColor( RGB( 0, 255, 0 ) );
    m_bullseyeControl->put_AlternateColor(RGB(255, 0, 0));
    return 0;
}

LRESULT CMainWindow::OnDestroy(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &bHandled)
{
    AtlAdviseSinkMap(this, false);
    return 0;
}

LRESULT CMainWindow::OnSize(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL &bHandled)
{
    CRect rect;
    GetClientRect(&rect);
    m_bullseyeWin.MoveWindow(rect);
    return 0;
}

LRESULT CMainWindow::OnFileExit(WORD wNotifyCode, WORD wID, HWND hwndCtl, BOOL &bHandled)
{
    SendMessage(WM_CLOSE);
    return 0;
}

LRESULT CMainWindow::OnWindowsDartboard(WORD wNotifyCode, WORD wID, HWND hwndCtl, BOOL &bHandled)
{
    CDartboard dlg;
    dlg.DoModal(m_hWnd);
    return 0;
}
// Window is closing, clean up
LRESULT CMainWindow::OnClose(UINT nMsg, WPARAM wparam, LPARAM lparam, BOOL & bHandled)
{
    DefWindowProc();
    PostQuitMessage(0);
    return 0;
}

///////////////////////////////////////////////////////////
//
// Event handlers for events from Bullseye control
//

void CMainWindow::OnRingHit(short ringNum)
{
    COLORREF altColor;
    m_bullseyeControl->get_AlternateColor(&altColor);

    switch (altColor) {
    case RGB(255, 0, 0):
        m_bullseyeControl->put_AlternateColor(RGB(0, 255, 0));
        break;

    case RGB(0, 255, 0):
        m_bullseyeControl->put_AlternateColor(RGB(0, 0, 255));
        break;

    case RGB(0, 0, 255):
        m_bullseyeControl->put_AlternateColor(RGB(255, 0, 0));
        break;
    }
}

void CMainWindow::OnScoreChanged(long ringValue)
{
}