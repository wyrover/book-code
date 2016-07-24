// A_setDialog.cpp : implementation file
//

#include "stdafx.h"
#include "test_audio_loop.h"
#include "A_setDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// A_setDialog dialog


A_setDialog::A_setDialog(CWnd* pParent /*=NULL*/)
	: CDialog(A_setDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(A_setDialog)
	//}}AFX_DATA_INIT
}


void A_setDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(A_setDialog)
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(A_setDialog, CDialog)
	//{{AFX_MSG_MAP(A_setDialog)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// A_setDialog message handlers
