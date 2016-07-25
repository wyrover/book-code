// Menu2View.cpp : implementation of the CMenu2View class
//

#include "stdafx.h"
#include "Menu2.h"


#include "Menu2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMenu2View

IMPLEMENT_DYNCREATE(CMenu2View, CView)

BEGIN_MESSAGE_MAP(CMenu2View, CView)
	//{{AFX_MSG_MAP(CMenu2View)
	ON_WM_CHAR()
	
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(IDM_PHONE1, OnPhone1)
	ON_COMMAND(IDM_PHONE2, OnPhone2)
	ON_COMMAND(IDM_PHONE3, OnPhone3)
	ON_COMMAND(IDM_PHONE4, OnPhone4)
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMenu2View construction/destruction

CMenu2View::CMenu2View()
{
	// TODO: add construction code here
	m_nIndex=-1;
	m_strLine="";
}

CMenu2View::~CMenu2View()
{
}

BOOL CMenu2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMenu2View drawing

void CMenu2View::OnDraw(CDC* pDC)
{
	CMenu2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CMenu2View printing

BOOL CMenu2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMenu2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMenu2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMenu2View diagnostics

#ifdef _DEBUG
void CMenu2View::AssertValid() const
{
	CView::AssertValid();
}

void CMenu2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMenu2Doc* CMenu2View::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMenu2Doc)));
	return (CMenu2Doc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMenu2View message handlers

void CMenu2View::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	CClientDC dc(this);
	if(0x0d==nChar)
	{
		if(0==++m_nIndex)
		{
			m_menu.CreatePopupMenu();
			GetParent()->GetMenu()->AppendMenu(MF_POPUP,
					(UINT)m_menu.m_hMenu,"PhoneBook");
			GetParent()->DrawMenuBar();
		}
		m_menu.AppendMenu(MF_STRING,IDM_PHONE1+m_nIndex,m_strLine.Left(m_strLine.Find(' ')));
		m_strArray.Add(m_strLine);
		m_strLine.Empty();
		Invalidate();
		
	}
	else
	{
		m_strLine+=nChar;
		dc.TextOut(0,0,m_strLine);
	}
	CView::OnChar(nChar, nRepCnt, nFlags);
}

void CMenu2View::OnPhone1() 
{
	// TODO: Add your command handler code here
	CClientDC dc(this);
	dc.TextOut(0,0,m_strArray.GetAt(0));
}

void CMenu2View::OnPhone2() 
{
	// TODO: Add your command handler code here
	CClientDC dc(this);
	dc.TextOut(0,0,m_strArray.GetAt(1));
}

void CMenu2View::OnPhone3() 
{
	// TODO: Add your command handler code here
	CClientDC dc(this);
	dc.TextOut(0,0,m_strArray.GetAt(2));
}

void CMenu2View::OnPhone4() 
{
	// TODO: Add your command handler code here
	CClientDC dc(this);
	dc.TextOut(0,0,m_strArray.GetAt(3));
}
