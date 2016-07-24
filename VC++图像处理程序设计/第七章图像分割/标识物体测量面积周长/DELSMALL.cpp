// DELSMALL.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DELSMALL.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// DELSMALL dialog


DELSMALL::DELSMALL(CWnd* pParent /*=NULL*/)
	: CDialog(DELSMALL::IDD, pParent)
{
	//{{AFX_DATA_INIT(DELSMALL)
	m_delsmall = 0;
	//}}AFX_DATA_INIT
}


void DELSMALL::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(DELSMALL)
	DDX_Text(pDX, IDC_EDITDEL, m_delsmall);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(DELSMALL, CDialog)
	//{{AFX_MSG_MAP(DELSMALL)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// DELSMALL message handlers
