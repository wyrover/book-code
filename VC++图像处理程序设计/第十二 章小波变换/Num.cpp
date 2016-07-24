// Num.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "Num.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNum dialog


CNum::CNum(CWnd* pParent /*=NULL*/)
	: CDialog(CNum::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNum)
	m_Num = 0;
	//}}AFX_DATA_INIT
}


void CNum::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNum)
	DDX_Text(pDX, IDC_EDIT1, m_Num);
	DDV_MinMaxInt(pDX, m_Num, 1, 5);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNum, CDialog)
	//{{AFX_MSG_MAP(CNum)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNum message handlers
