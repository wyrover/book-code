// NNJUNZHI.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "NNJUNZHI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// NNJUNZHI dialog


NNJUNZHI::NNJUNZHI(CWnd* pParent /*=NULL*/)
	: CDialog(NNJUNZHI::IDD, pParent)
{
	//{{AFX_DATA_INIT(NNJUNZHI)
	m_n = 3;
	//}}AFX_DATA_INIT
}


void NNJUNZHI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(NNJUNZHI)
	DDX_Text(pDX, IDC_EDIT1, m_n);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(NNJUNZHI, CDialog)
	//{{AFX_MSG_MAP(NNJUNZHI)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// NNJUNZHI message handlers
