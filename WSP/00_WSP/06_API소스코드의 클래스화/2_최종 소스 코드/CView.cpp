
#include <windows.h>
#include "stdafx.h"
#include "CView.h"


CView app; // one global object


//{{AFX_MESSAGE
BEGIN_MESSAGE_MAP(CView)
    {WM_CREATE,CView::OnCreate},
    {WM_PAINT,CView::OnDraw},
    {WM_DESTROY,CView::OnDestroy},
    {WM_LBUTTONDOWN,CView::OnLButtonDown},
END_MESSAGE_MAP()
//}}AFX_MESSAGE

LRESULT CView::OnCreate(WPARAM wParam,LPARAM lParam) 
{
    return 0L;
}



LRESULT CView::OnDraw(WPARAM wParam,LPARAM lParam) 
{
    HDC         hdc;
    PAINTSTRUCT ps;
    RECT        rect;

    hdc = BeginPaint(hwnd,&ps);

    GetClientRect(hwnd,&rect);
    DrawText(hdc,"Hello, Windows!",-1,&rect,
        DT_SINGLELINE|DT_CENTER|DT_VCENTER);
    
    EndPaint(hwnd,&ps);
    return 0L;
}

LRESULT CView::OnDestroy(WPARAM wParam,LPARAM lParam) {
    PostQuitMessage(0);
    return 0L;
}

LRESULT CView::OnLButtonDown(WPARAM wParam,LPARAM lParam) {
    MessageBox(hwnd, "LButtonDown", "WM_LBUTTONDOWN", MB_OK);
    return 0L;
}
