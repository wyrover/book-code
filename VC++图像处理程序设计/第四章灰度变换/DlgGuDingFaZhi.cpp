// DlgGuDingFaZhi.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DlgGuDingFaZhi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGuDingFaZhi dialog


CDlgGuDingFaZhi::CDlgGuDingFaZhi(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGuDingFaZhi::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGuDingFaZhi)
	m_Yuzhi = 100;
	//}}AFX_DATA_INIT
}


void CDlgGuDingFaZhi::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGuDingFaZhi)
	DDX_Text(pDX, IDC_EDIT1, m_Yuzhi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGuDingFaZhi, CDialog)
	//{{AFX_MSG_MAP(CDlgGuDingFaZhi)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGuDingFaZhi message handlers
