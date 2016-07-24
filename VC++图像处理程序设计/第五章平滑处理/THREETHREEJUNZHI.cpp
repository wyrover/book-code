// THREETHREEJUNZHI.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "THREETHREEJUNZHI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// THREETHREEJUNZHI dialog


THREETHREEJUNZHI::THREETHREEJUNZHI(CWnd* pParent /*=NULL*/)
	: CDialog(THREETHREEJUNZHI::IDD, pParent)
{
	//{{AFX_DATA_INIT(THREETHREEJUNZHI)
	m_T = 0;
	//}}AFX_DATA_INIT
}


void THREETHREEJUNZHI::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(THREETHREEJUNZHI)
	DDX_Text(pDX, IDC_EDIT1, m_T);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(THREETHREEJUNZHI, CDialog)
	//{{AFX_MSG_MAP(THREETHREEJUNZHI)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// THREETHREEJUNZHI message handlers
