// DlgZoom.cpp : implementation file
//

#include "stdafx.h"
#include "dimageprocess.h"
#include "DlgZoom.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgZoom dialog


CDlgZoom::CDlgZoom(CWnd* pParent /*=NULL*/)
: CDialog(CDlgZoom::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgZoom)
	m_horZoom = 0.5f;
	m_verZoom = 0.5f;
	//}}AFX_DATA_INIT
}


void CDlgZoom::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgZoom)
	DDX_Text(pDX, IDC_EDIT_XZoom, m_horZoom);
	DDX_Text(pDX, IDC_EDIT_YZoom, m_verZoom);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgZoom, CDialog)
	//{{AFX_MSG_MAP(CDlgZoom)
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgZoom message handlers
