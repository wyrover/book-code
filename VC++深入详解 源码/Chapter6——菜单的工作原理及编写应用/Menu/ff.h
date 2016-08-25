// MenuView.cpp : implementation of the CMenuView class
//

#include "stdafx.h"
#include "Menu.h"

#include "MenuDoc.h"
#include "MenuView.h"
#include "resource.h"

......

/////////////////////////////////////////////////////////////////////////////
// CMenuView

IMPLEMENT_DYNCREATE(CMenuView, CView)

BEGIN_MESSAGE_MAP(CMenuView, CView)
	ON_WM_CONTEXTMENU()
	//{{AFX_MSG_MAP(CMenuView)
	ON_COMMAND(IDM_TEST, OnTest)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

......

void CMenuView::OnTest() 
{
	// TODO: Add your command handler code here
	
}
