// YuZhi.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "YuZhi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CYuZhi dialog


CYuZhi::CYuZhi(CWnd* pParent /*=NULL*/)
	: CDialog(CYuZhi::IDD, pParent)
{
	//{{AFX_DATA_INIT(CYuZhi)
	m_gray = 0;
	//}}AFX_DATA_INIT
}


void CYuZhi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CYuZhi)
	DDX_Text(pDX, IDC_EDITGray, m_gray);
	DDV_MinMaxInt(pDX, m_gray, 0, 255);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CYuZhi, CDialog)
	//{{AFX_MSG_MAP(CYuZhi)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CYuZhi message handlers
