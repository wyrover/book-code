// DlgPingYi.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DlgPingYi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPingYi dialog


CDlgPingYi::CDlgPingYi(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPingYi::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPingYi)
	m_Xmove = 0;
	m_Ymove = 0;
	//}}AFX_DATA_INIT
}


void CDlgPingYi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPingYi)
	DDX_Text(pDX, IDC_EDIT_XMOVE, m_Xmove);
	DDX_Text(pDX, IDC_EDIT_YMOVE, m_Ymove);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPingYi, CDialog)
	//{{AFX_MSG_MAP(CDlgPingYi)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPingYi message handlers
