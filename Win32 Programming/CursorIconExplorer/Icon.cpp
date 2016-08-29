// Icon.cpp : implementation file
//

#include "stdafx.h"
#include "CursorIconExplorer.h"
#include "Icon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CIcon dialog

IMPLEMENT_DYNCREATE(CIcon, CPropertyPage)

CIcon::CIcon() : CPropertyPage(CIcon::IDD)
{
	//{{AFX_DATA_INIT(CIcon)
		// NOTE: the ClassWizard will add member initialization here
    	//}}AFX_DATA_INIT
    ico = NULL;
}


void CIcon::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CIcon)
	DDX_Control(pDX, IDC_COPY, c_Copy);
	DDX_Control(pDX, IDC_AUTOCOPY, c_AutoCopy);
	DDX_Control(pDX, IDC_ICONDISPLAY, c_Icon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CIcon, CPropertyPage)
	//{{AFX_MSG_MAP(CIcon)
	ON_BN_CLICKED(IDC_COPY, OnCopy)
	ON_BN_CLICKED(IDC_AUTOCOPY, OnAutocopy)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDI_APPLICATION, IDI_WINLOGO, IconSelect)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CIcon message handlers

void CIcon::OnCopy() 
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

BOOL CIcon::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
         c_Copy.EnableWindow(FALSE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CIcon::OnAutocopy() 
{
 c_Copy.ShowWindow(c_AutoCopy.GetCheck() ? SW_HIDE : SW_SHOW);
	
}

void CIcon::IconSelect(UINT id)
    {
     ico = ::LoadIcon(NULL, MAKEINTRESOURCE(id));
     c_Copy.EnableWindow(ico != NULL);
     if(ico != NULL)
	 c_Icon.SetIcon(ico);
     if(c_AutoCopy.GetCheck())
	 OnCopy();
    }

HBRUSH CIcon::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CPropertyPage::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(nCtlColor == CTLCOLOR_STATIC && pWnd->m_hWnd == c_Icon.m_hWnd)
	    return (HBRUSH)::GetStockObject(WHITE_BRUSH);
	
	return hbr;
}
