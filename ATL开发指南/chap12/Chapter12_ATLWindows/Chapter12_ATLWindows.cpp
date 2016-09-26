//
// Chapter12_ATLWindows.cpp
//

#include "stdafx.h"
#include "resource.h"

#include <atlbase.h>
CComModule _Module;
#include <atlwin.h>

class CModelessDialog : public CDialogImpl< CModelessDialog >
{
public:
    enum { IDD = IDD_MODELESS_DIALOG };

    BEGIN_MSG_MAP(CModelessDialog)
    MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
    MESSAGE_HANDLER(WM_CLOSE, OnClose)
    END_MSG_MAP()

    LRESULT OnInitDialog(UINT, WPARAM, LPARAM, BOOL&)
    {
        return 0;
    }

    LRESULT OnClose(UINT, WPARAM, LPARAM, BOOL&)
    {
        DestroyWindow();
        return 0;
    }
};

class CMainWindow : public CWindowImpl< CMainWindow >
{
    BEGIN_MSG_MAP(CMainWindow)
    MESSAGE_HANDLER(WM_CREATE, OnCreate)
    MESSAGE_HANDLER(WM_PAINT, OnPaint)
    MESSAGE_HANDLER(WM_RBUTTONDOWN, OnRButtonDown)
    MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
    MESSAGE_HANDLER(WM_DESTROY, OnDestroy)
    END_MSG_MAP()

    LRESULT OnLButtonDown(UINT, WPARAM, LPARAM, BOOL&)
    {
        CSimpleDialog<IDD_SIMPLEDIALOG> simpleDialog;
        simpleDialog.DoModal();
        return 0;
    }

    LRESULT OnRButtonDown(UINT, WPARAM, LPARAM, BOOL&)
    {
        CModelessDialog* pDlg = new CModelessDialog;
        pDlg->Create(0);
        return 0;
    }

    LRESULT OnCreate(UINT, WPARAM, LPARAM, BOOL&)
    {
        return 0;
    }

    LRESULT OnPaint(UINT, WPARAM, LPARAM, BOOL&)
    {
        PAINTSTRUCT ps;
        HDC hdc = GetDC();
        BeginPaint(&ps);
        RECT rc;
        GetClientRect(&rc);
        HBRUSH hbr = CreateSolidBrush(RGB(0x00, 0xFF, 0xFF));
        HBRUSH hOldBr = (HBRUSH) SelectObject(hdc, hbr);
        FillRect(hdc, &rc, hbr);
        SelectObject(hdc, hOldBr);
        DeleteObject(hbr);
        EndPaint(&ps);
        return 0;
    }

    LRESULT OnDestroy(UINT, WPARAM, LPARAM, BOOL&)
    {
        PostQuitMessage(0);
        return 0;
    }
};

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    _Module.Init(0, hInstance);
    CMainWindow window;
    RECT rectPos = { 0, 0, 300, 300 };
    window.Create(NULL,
                  rectPos,
                  "Main Window",
                  WS_OVERLAPPEDWINDOW | WS_VISIBLE);
    MSG msg;

    while (GetMessage(&msg, 0, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    _Module.Term();
    return 0;
}



