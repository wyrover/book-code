// DialogShuangYu.cpp : implementation file
//

#include "stdafx.h"
#include "DSplit.h"
#include "DialogShuangYu.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogShuangYu dialog


CDialogShuangYu::CDialogShuangYu(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogShuangYu::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogShuangYu)
	m_ZhiYu1 = 20;
	m_ZhiYu2 = 80;
	//}}AFX_DATA_INIT
}


void CDialogShuangYu::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogShuangYu)
	DDX_Text(pDX, IDC_EDIT1, m_ZhiYu1);
	DDX_Text(pDX, IDC_EDIT2, m_ZhiYu2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogShuangYu, CDialog)
	//{{AFX_MSG_MAP(CDialogShuangYu)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogShuangYu message handlers

void CDialogShuangYu::OnRadio1() 
{
	// TODO: Add your control notification handler code here
	m_mode=0;
}

void CDialogShuangYu::OnRadio2() 
{
	// TODO: Add your control notification handler code here
	m_mode=1;
}
