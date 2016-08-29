// Cur.cpp : implementation file
//

#include "stdafx.h"
#include "CursorIconExplorer.h"
#include "Cur.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCur property page

IMPLEMENT_DYNCREATE(CCur, CPropertyPage)

CCur::CCur() : CPropertyPage(CCur::IDD)
{
	//{{AFX_DATA_INIT(CCur)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
    tcur = NULL;
}

CCur::~CCur()
{
}

void CCur::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCur)
	DDX_Control(pDX, IDC_COPY, c_Copy);
	DDX_Control(pDX, IDC_AUTOCOPY, c_AutoCopy);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCur, CPropertyPage)
	//{{AFX_MSG_MAP(CCur)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_AUTOCOPY, OnAutocopy)
	ON_WM_CTLCOLOR()
	ON_WM_TIMER()
	ON_WM_SETCURSOR()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_ARROW, IDC_UPARROW, CurSelect)
	ON_COMMAND_RANGE(IDC_ICON, IDC_HELP, CurSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCur message handlers


void CCur::OnCopy() 
{
    CClientDC icoDC(&c_Icon);
    CDC memDC;
    memDC.CreateCompatibleDC(&icoDC);

    CBitmap bm;
    CRect r;

    c_Icon.GetClientRect(&r);

    bm.CreateCompatibleBitmap(&icoDC, r.right, r.bottom);

    CBitmap * oldbm = memDC.SelectObject(&bm);

    memDC.BitBlt(0, 0, r.right, r.bottom, &icoDC, 0, 0, SRCCOPY);
 
     OpenClipboard();
     ::EmptyClipboard();
     ::SetClipboardData(CF_BITMAP, bm.m_hObject);
     CloseClipboard();

     memDC.SelectObject(oldbm);
     bm.Detach();  // make sure bitmap not deleted with CBitmap object
	
}

BOOL CCur::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
 	c_Copy.EnableWindow(FALSE);
	c_Icon.SubclassDlgItem(IDC_ICONDISPLAY, this);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCur::OnAutocopy() 
{
 c_Copy.ShowWindow(c_AutoCopy.GetCheck() ? SW_HIDE : SW_SHOW);
}


void CCur::CurSelect(UINT id)
    {
     HCURSOR cur;
     cur = ::LoadCursor(NULL, MAKEINTRESOURCE(id));
     c_Copy.EnableWindow(cur != NULL);
     if(cur != NULL)
        { /* valid cursor */
	 // We need to create a new icon.  After we set it, we have to save
	 // it, deleting any existing icon
	 c_Icon.cur = cur;
	 c_Icon.InvalidateRect(NULL, TRUE);
	 tcur = cur;
	 SetTimer(1, 4000, NULL);
	 ::SetCursor(cur);
	} /* valid cursor */
     if(c_AutoCopy.GetCheck())
        { /* copy it */
	 c_Icon.UpdateWindow();
	 OnCopy();
	} /* copy it */
    }

HBRUSH CCur::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC && pWnd->m_hWnd == c_Icon.m_hWnd)
	    return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	
	return hbr;
}

void CCur::OnTimer(UINT nIDEvent) 
{
 tcur = NULL;
 KillTimer(nIDEvent);
 ::SetCursor(::LoadCursor(NULL,IDC_ARROW));
}

BOOL CCur::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
 if(tcur != NULL && nHitTest == HTCLIENT)
    { /* use cursor */
     ::SetCursor(tcur);
     return TRUE;
    } /* use cursor */
	
	return CPropertyPage::OnSetCursor(pWnd, nHitTest, message);
}

BEGIN_MESSAGE_MAP(CIco, CStatic)
     ON_WM_PAINT()
END_MESSAGE_MAP()

/****************************************************************************
*                                 CIco::CICo
****************************************************************************/

CIco::CIco()
    {
     cur = NULL;
    }


/****************************************************************************
*                                CIco::OnPaint
****************************************************************************/

void CIco::OnPaint()
    {
     CPaintDC dc(this);
     RECT r;
     CBrush br(RGB(255,255,255));
     GetClientRect(&r);
     dc.FillRect(&r, &br);
     if(cur != NULL)
        { /* has cursor */
	 ::DrawIconEx(dc.m_hDC, 0,0, cur, 0, 0, 0, NULL,
     		DI_DEFAULTSIZE | DI_NORMAL);
	} /* has cursor */
    }
