// connectto.cpp : implementation file
//

#include "stdafx.h"
#include "wuziqi.h"
#include "connectto.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// connectto dialog


connectto::connectto(CWnd* pParent /*=NULL*/)
	: CDialog(connectto::IDD, pParent)
{
	//{{AFX_DATA_INIT(connectto)
	m_where = _T("");
	//}}AFX_DATA_INIT
}


void connectto::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(connectto)
	DDX_Text(pDX, IDC_WHERE, m_where);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(connectto, CDialog)
	//{{AFX_MSG_MAP(connectto)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// connectto message handlers
