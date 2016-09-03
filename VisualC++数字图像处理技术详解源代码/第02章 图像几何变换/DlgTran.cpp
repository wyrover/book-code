// DlgTran.cpp : implementation file
//

#include "stdafx.h"
#include "dimageprocess.h"
#include "DlgTran.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTran dialog


CDlgTran::CDlgTran(CWnd* pParent /*=NULL*/)
: CDialog(CDlgTran::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTran)
	m_horOff = 50;
	m_verOff = 50;
	//}}AFX_DATA_INIT
}


void CDlgTran::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTran)
	DDX_Text(pDX, IDC_horOffSet, m_horOff);
	DDX_Text(pDX, IDC_verOffSet, m_verOff);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTran, CDialog)
	//{{AFX_MSG_MAP(CDlgTran)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTran message handlers

BOOL CDlgTran::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	CSpinButtonCtrl* pSpinHor=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_hor);
	//	pSpinHor->SetBuddy((CEdit*)GetDlgItem(IDC_horOffSet));
	pSpinHor->SetRange(-100,100);
	CSpinButtonCtrl* pSpinVer=(CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_ver);
	//	pSpinVer->SetBuddy((CEdit*)GetDlgItem(IDC_verOffSet));
	pSpinVer->SetRange(-100,100);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
