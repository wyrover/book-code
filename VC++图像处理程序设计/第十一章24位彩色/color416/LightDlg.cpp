// LightDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "LightDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LightDlg dialog


LightDlg::LightDlg(CWnd* pParent /*=NULL*/)
	: CDialog(LightDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(LightDlg)
	m_Light = 0;
	//}}AFX_DATA_INIT
}


void LightDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LightDlg)
	DDX_Text(pDX, IDC_EDITLight, m_Light);
	DDV_MinMaxInt(pDX, m_Light, 0, 500);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LightDlg, CDialog)
	//{{AFX_MSG_MAP(LightDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LightDlg message handlers
