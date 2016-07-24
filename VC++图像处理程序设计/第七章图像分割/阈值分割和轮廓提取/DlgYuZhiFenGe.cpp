// DlgYuZhiFenGe.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DlgYuZhiFenGe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgYuZhiFenGe dialog


CDlgYuZhiFenGe::CDlgYuZhiFenGe(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgYuZhiFenGe::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgYuZhiFenGe)
	m_Yuzhi = 0;
	//}}AFX_DATA_INIT
}

void CDlgYuZhiFenGe::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgYuZhiFenGe)
	DDX_Text(pDX, IDC_EDIT_YUZHI, m_Yuzhi);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgYuZhiFenGe, CDialog)
	//{{AFX_MSG_MAP(CDlgYuZhiFenGe)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgYuZhiFenGe message handlers
