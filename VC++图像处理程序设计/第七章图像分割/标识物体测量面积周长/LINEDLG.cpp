// LINEDLG.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "LINEDLG.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// LINEDLG dialog


LINEDLG::LINEDLG(CWnd* pParent /*=NULL*/)
	: CDialog(LINEDLG::IDD, pParent)
{
	//{{AFX_DATA_INIT(LINEDLG)
	m_shumu = 0;
	m_zongshu = 0;
	m_line = _T("");
	//}}AFX_DATA_INIT
}


void LINEDLG::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(LINEDLG)
	DDX_Text(pDX, IDC_EDIT1, m_shumu);
	DDX_Text(pDX, IDC_LINE_ALL, m_zongshu);
	DDX_Text(pDX, IDC_LINE_EDIT2, m_line);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(LINEDLG, CDialog)
	//{{AFX_MSG_MAP(LINEDLG)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// LINEDLG message handlers
