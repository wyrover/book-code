// SquareDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "SquareDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// SquareDlg dialog


SquareDlg::SquareDlg(CWnd* pParent /*=NULL*/)
	: CDialog(SquareDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(SquareDlg)
	m_number = 0;
	m_squareALL = 0;
	m_ShuChu = _T("");
	//}}AFX_DATA_INIT
}


void SquareDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(SquareDlg)
	DDX_Text(pDX, IDC_EDIT1, m_number);
	DDX_Text(pDX, IDC_SQUARE_ALL, m_squareALL);
	DDX_Text(pDX, IDC_SQUARE_EDIT2, m_ShuChu);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(SquareDlg, CDialog)
	//{{AFX_MSG_MAP(SquareDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// SquareDlg message handlers
