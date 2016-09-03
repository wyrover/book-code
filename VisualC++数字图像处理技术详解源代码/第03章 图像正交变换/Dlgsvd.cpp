// Dlgsvd.cpp : 实现文件
//

#include "stdafx.h"
#include "DImageProcess.h"
#include "Dlgsvd.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgsvd dialog


CDlgsvd::CDlgsvd(CWnd* pParent /*=NULL*/)
: CDialog(CDlgsvd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgsvd)
	m_svd = 50;
	//}}AFX_DATA_INIT
}


void CDlgsvd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgRot)
	DDX_Text(pDX, IDC_rot_svd, m_svd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgsvd, CDialog)
	//{{AFX_MSG_MAP(CDlgRot)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgRot message handlers

BOOL CDlgsvd::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CSpinButtonCtrl* pSpinsvd=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_svd);
	pSpinsvd->SetRange(1,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
