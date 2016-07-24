// DlgArea.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DlgArea.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgArea dialog


CDlgArea::CDlgArea(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgArea::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgArea)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDlgArea::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgArea)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgArea, CDialog)
	//{{AFX_MSG_MAP(CDlgArea)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgArea message handlers
