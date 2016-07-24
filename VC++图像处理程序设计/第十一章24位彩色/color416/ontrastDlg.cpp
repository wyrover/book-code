// ontrastDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "ontrastDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// ContrastDlg dialog


ContrastDlg::ContrastDlg(CWnd* pParent /*=NULL*/)
	: CDialog(ContrastDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(ContrastDlg)
	m_Increment = 0;
	//}}AFX_DATA_INIT
}


void ContrastDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(ContrastDlg)
	DDX_Text(pDX, IDC_EDITContrast, m_Increment);
	DDV_MinMaxInt(pDX, m_Increment, -127, 128);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(ContrastDlg, CDialog)
	//{{AFX_MSG_MAP(ContrastDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// ContrastDlg message handlers
