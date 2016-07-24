// askconnect.cpp : implementation file
//

#include "stdafx.h"
#include "wuziqi.h"
#include "askconnect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// askconnect dialog


askconnect::askconnect(CWnd* pParent /*=NULL*/)
	: CDialog(askconnect::IDD, pParent)
{
	//{{AFX_DATA_INIT(askconnect)
	m_ip = _T("");
	m_pro = _T("");
	//}}AFX_DATA_INIT
}


void askconnect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(askconnect)
	DDX_Text(pDX, IDC_IP, m_ip);
	DDX_Text(pDX, IDC_PRO, m_pro);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(askconnect, CDialog)
	//{{AFX_MSG_MAP(askconnect)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// askconnect message handlers
