// AddDialog.cpp : implementation file
//

#include "stdafx.h"
#include "MultiCastChat.h"
#include "AddDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddDialog dialog


CAddDialog::CAddDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CAddDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAddDialog)
	m_strAddr = _T("234.5.5.5");
	m_nPort = 3505;
	//}}AFX_DATA_INIT
}


void CAddDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAddDialog)
	DDX_Text(pDX, IDC_ADDRESS, m_strAddr);
	DDX_Text(pDX, IDC_PORT, m_nPort);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CAddDialog, CDialog)
	//{{AFX_MSG_MAP(CAddDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAddDialog message handlers
