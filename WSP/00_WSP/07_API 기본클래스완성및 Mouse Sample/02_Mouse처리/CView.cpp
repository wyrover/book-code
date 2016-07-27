
#include <windows.h>
#include "resource.h"
#include "stdafx.h"
#include "CView.h"
#include "CDocument.h"


CView app; // one global object


//{{AFX_MESSAGE
BEGIN_MESSAGE_MAP(CView)
	{WM_SETFOCUS, CView::OnSetFocus},
    {WM_CREATE,CView::OnCreate},
	{WM_SIZE, CView::OnSize},
	{WM_COMMAND, CView::OnCommand},
	{WM_KEYDOWN, CView::OnKeyDown},
    {WM_DESTROY,CView::OnDestroy},
	{WM_LBUTTONDOWN, CView::OnLButtonDown},
	{WM_PAINT, CView::OnPaint},
END_MESSAGE_MAP()
//}}AFX_MESSAGE

CView::CView()
{
	 bFull = FALSE;	// 전체 보기인가??
}


LRESULT CView::OnSetFocus(HWND hwnd,WPARAM wParam,LPARAM lParam) 
{
    return 0L;
}


LRESULT CView::OnCreate(HWND hwnd,WPARAM wParam,LPARAM lParam) 
{

    return 0L;
}

LRESULT CView::OnSize(HWND hwnd,WPARAM wParam,LPARAM lParam) 
{
    return 0L;
}

LRESULT CView::OnCommand(HWND hwnd,WPARAM wParam,LPARAM lParam) 
{
	switch( LOWORD(wParam))
	{
	case IDM_VIEW_FULL:
		{
			static HMENU	hMenu;
			bFull = !bFull;
			if( bFull )
			{
				ModifyStyle(hwnd, WS_CAPTION, 0);
				hMenu =  GetMenu(hwnd);
				SetMenu(hwnd, 0);
				ShowWindow(hwnd, SW_MAXIMIZE);
			}
			else
			{
				ModifyStyle(hwnd, 0, WS_CAPTION);
				SetMenu(hwnd, hMenu);
				ShowWindow(hwnd, SW_SHOWNORMAL);
			}
		}
	}
	return 0L;
}


LRESULT CView::OnKeyDown(HWND hwnd,WPARAM wParam,LPARAM lParam) 
{
	if( wParam == VK_ESCAPE)
	{
		SendMessage(hwnd, WM_COMMAND, IDM_VIEW_FULL, 0);
	}
	return 0L;
}


BOOL WINAPI CView::PreTranslateMessage(HWND hwnd, MSG *pMsg)
{
	if( pMsg->message	== WM_KEYDOWN &&
				pMsg->wParam == VK_ESCAPE && bFull)
	{
		// 다시 작게 한다.
		SendMessage(hwnd, WM_COMMAND, IDM_VIEW_FULL, 0);

		// 더 이상 메시지가 분배되지 않도록
		return TRUE;
	}
	return FALSE;
}


LRESULT CView::OnDestroy(HWND hwnd,WPARAM wParam,LPARAM lParam) 
{
    PostQuitMessage(0);
    return 0L;
}

void CView::ModifyStyle( HWND hwnd, UINT Remove, UINT Add, BOOL bRedraw)
{
	UINT style = GetWindowLong(hwnd, GWL_STYLE);
	style = style | Add;
	style = style & ~Remove;
	SetWindowLong(hwnd, GWL_STYLE, style);

	if( bRedraw )
		SetWindowPos( hwnd, 0, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER);
}

LRESULT CView::OnLButtonDown(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	HDC hdc		= GetDC(hwnd);
	POINTS pt	= MAKEPOINTS(lParam);

	Rectangle(hdc, pt.x, pt.y, pt.x+75, pt.y+75);

	ReleaseDC(hwnd, hdc);
	/*
	POINTS pt = MAKEPOINTS(lParam);
	
	HDC hdc = GetDC(hwnd);
	Rectangle(hdc, pt.x, pt.y, pt.x + 50, pt.y+50);

	ReleaseDC(hwnd,hdc);

	//---------- 데이터 저장 ------------------
	GetDocument()->data.push_back(pt);*/
	
	return 0;
}

CDocument * CView::GetDocument()
{
	return pDoc;
}

LRESULT CView::OnPaint(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hwnd, &ps); 

	for( int i=0; i< GetDocument()->data.size(); i++)
	{
		POINTS pt = GetDocument()->data[i];
		Rectangle(hdc, pt.x, pt.y, pt.x + 50, pt.y+50);
	}
	EndPaint(hwnd, &ps);
	return 0L;
}
