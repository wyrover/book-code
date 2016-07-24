// resultwin.cpp : implementation file
//

#include "stdafx.h"
#include "wuziqi.h"
#include "resultwin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// resultwin dialog


resultwin::resultwin(CWnd* pParent /*=NULL*/)
	: CDialog(resultwin::IDD, pParent)
{
	//{{AFX_DATA_INIT(resultwin)
	m_Who = _T("");
	//}}AFX_DATA_INIT
}


void resultwin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(resultwin)
	DDX_Text(pDX, IDC_WHO_WIN, m_Who);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(resultwin, CDialog)
	//{{AFX_MSG_MAP(resultwin)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// resultwin message handlers
