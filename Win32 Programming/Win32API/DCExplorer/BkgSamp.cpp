// BkgSamp.cpp : implementation file
//

#include "stdafx.h"
#include "DCExplorer.h"
#include "BkgSamp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBkgSample dialog


CBkgSample::CBkgSample(CWnd* pParent /*=NULL*/)
	: CDialog(CBkgSample::IDD, pParent)
{
	//{{AFX_DATA_INIT(CBkgSample)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CBkgSample::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBkgSample)
	DDX_Control(pDX, IDC_RECT, c_Sample);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBkgSample, CDialog)
	//{{AFX_MSG_MAP(CBkgSample)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBkgSample message handlers

BOOL CBkgSample::OnInitDialog() 
{
	CDialog::OnInitDialog();
				 c_Sample.ShowWindow(SW_HIDE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HBRUSH CBkgSample::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if(pWnd->m_hWnd == this->m_hWnd)
	   { /* sample */
	    pDC->SetBrushOrg(org.x, org.y);
	    pDC->SetBkMode(bkmode);
	    pDC->SetBkColor(bkcolor);

	
	    return (HBRUSH)brush.m_hObject;
	   } /* sample */

	return hbr;
}

void CBkgSample::OnDestroy() 
{
	CDialog::OnDestroy();
	
}
