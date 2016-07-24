// DlgSuoFang.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DlgSuoFang.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSuoFang dialog


CDlgSuoFang::CDlgSuoFang(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSuoFang::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSuoFang)
	m_XZoom = 0.0f;
	m_YZoom = 0.0f;
	//}}AFX_DATA_INIT
}


void CDlgSuoFang::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSuoFang)
	DDX_Text(pDX, IDC_EDIT_XZoom, m_XZoom);
	DDX_Text(pDX, IDC_EDIT_YZoom, m_YZoom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSuoFang, CDialog)
	//{{AFX_MSG_MAP(CDlgSuoFang)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSuoFang message handlers
