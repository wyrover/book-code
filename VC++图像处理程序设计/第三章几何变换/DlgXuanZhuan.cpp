// DlgXuanZhuan.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DlgXuanZhuan.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgXuanZhuan dialog


CDlgXuanZhuan::CDlgXuanZhuan(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgXuanZhuan::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgXuanZhuan)
	m_iRotateAngle = 0;
	//}}AFX_DATA_INIT
}


void CDlgXuanZhuan::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgXuanZhuan)
	DDX_Text(pDX, IDC_EDIT_Rotate, m_iRotateAngle);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgXuanZhuan, CDialog)
	//{{AFX_MSG_MAP(CDlgXuanZhuan)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgXuanZhuan message handlers
