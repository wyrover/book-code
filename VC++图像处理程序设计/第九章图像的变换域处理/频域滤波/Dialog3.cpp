// Dialog3.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "Dialog3.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// Dialog3 dialog


Dialog3::Dialog3(CWnd* pParent /*=NULL*/)
	: CDialog(Dialog3::IDD, pParent)
{
	//{{AFX_DATA_INIT(Dialog3)
	m_x = 50;
	m_y = 50;
	m_x1 = 100;
	m_y1 = 100;
	//}}AFX_DATA_INIT
}


void Dialog3::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(Dialog3)
	DDX_Text(pDX, IDC_EDIT1, m_x);
	DDX_Text(pDX, IDC_EDIT2, m_y);
	DDX_Text(pDX, IDC_EDIT3, m_x1);
	DDX_Text(pDX, IDC_EDIT4, m_y1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(Dialog3, CDialog)
	//{{AFX_MSG_MAP(Dialog3)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// Dialog3 message handlers
