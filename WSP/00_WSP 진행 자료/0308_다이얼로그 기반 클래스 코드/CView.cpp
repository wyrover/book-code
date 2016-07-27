
#include <windows.h>
#include "resource.h"
#include "stdafx.h"
#include "CView.h"
#include "cdocument.h"


CView app; // one global object


//{{AFX_MESSAGE
BEGIN_MESSAGE_MAP(CView)
    {WM_INITDIALOG,CView::OnInitDialog},
	{WM_COMMAND, CView::OnCommand},
END_MESSAGE_MAP()
//}}AFX_MESSAGE

CView::CView()
{

}

LRESULT CView::OnInitDialog(HWND hwnd,WPARAM wParam,LPARAM lParam) 
{
	//=========================================
	// 반드시 먼저 처리
	if(CObject::OnInitDialog(hwnd, wParam, lParam)==-1)
		return -1L;
	//==========================================

	//TODO:

    return 0L;
}


LRESULT CView::OnCommand(HWND hDlg,WPARAM wParam,LPARAM lParam) 
{
	switch( LOWORD(wParam))
	{
	case IDCANCEL: EndDialog(hDlg, IDCANCEL);	return 0;
	}
	return 0L;
}


CDocument* CView::GetDocument()
{
	return pDoc;
}

