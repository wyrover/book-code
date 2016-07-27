//-----------------------------------------------------------
// (1) GetDC() 이용 : API함수 
//-----------------------------------------------------------

#include <windows.h>
#include <math.h>
#include "stdafx.h"
#include "CView.h"

CView app;

//{{AFX_MESSAGE
BEGIN_MESSAGE_MAP(CView)
    {WM_CREATE,CView::OnCreate},
    {WM_PAINT,CView::OnDraw},
    {WM_DESTROY,CView::OnDestroy},
    //{{seojt
    {WM_SIZE,CView::OnSize},
    {WM_LBUTTONDOWN,CView::OnLButtonDown},
    //}}seojt
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


//{{seojt
LRESULT CView::OnLButtonDown(WPARAM wParam,LPARAM lParam) {
    int i;

    hdc = GetDC(hwnd);

    MoveToEx (hdc, 0,        cyClient / 2, NULL) ;
    LineTo   (hdc, cxClient, cyClient / 2) ;

    for (i = 0 ; i < NUM ; i++) {
        pt[i].x = i * cxClient / NUM ;
        pt[i].y = (int) (cyClient / 2 *
            (1 - sin (TWOPI * i / NUM))) ;
    }//for
    Polyline (hdc, pt, NUM) ;

    ReleaseDC(hwnd,hdc);
    return 0L;
}
//}}seojt


//{{seojt
LRESULT CView::OnSize(WPARAM wParam,LPARAM lParam) {
    //#define LOWORD(a) ( (a) & 0x0000ffff )
    //#define HIWORD(a) ( ((a) & 0xffff0000) >> 16)
    cxClient = LOWORD(lParam);
    cyClient = HIWORD(lParam);
    return 0L;
}
//}}seojt