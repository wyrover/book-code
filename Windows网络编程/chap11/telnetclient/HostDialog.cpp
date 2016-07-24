// HostDialog.cpp : implementation file
//

#include "stdafx.h"
#include "CTelnet.h"
#include "HostDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHostDialog dialog


CHostDialog::CHostDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CHostDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHostDialog)
	m_HostName = _T("10.12.13.66");
	//}}AFX_DATA_INIT
}


void CHostDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CHostDialog)
	DDX_Text(pDX, IDC_EDIT1, m_HostName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CHostDialog, CDialog)
	//{{AFX_MSG_MAP(CHostDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHostDialog message handlers

void CHostDialog::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}
