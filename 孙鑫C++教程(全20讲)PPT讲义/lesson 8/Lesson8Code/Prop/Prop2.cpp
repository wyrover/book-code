// Prop2.cpp : implementation file
//

#include "stdafx.h"
#include "Prop.h"
#include "Prop2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CProp2 property page

IMPLEMENT_DYNCREATE(CProp2, CPropertyPage)

CProp2::CProp2() : CPropertyPage(CProp2::IDD)
{
	//{{AFX_DATA_INIT(CProp2)
	m_football = FALSE;
	m_basketball = FALSE;
	m_volleyball = FALSE;
	m_swim = FALSE;
	//}}AFX_DATA_INIT
}

CProp2::~CProp2()
{
}

void CProp2::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProp2)
	DDX_Check(pDX, IDC_CHECK1, m_football);
	DDX_Check(pDX, IDC_CHECK2, m_basketball);
	DDX_Check(pDX, IDC_CHECK3, m_volleyball);
	DDX_Check(pDX, IDC_CHECK4, m_swim);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProp2, CPropertyPage)
	//{{AFX_MSG_MAP(CProp2)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProp2 message handlers

BOOL CProp2::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	((CPropertySheet*)GetParent())->SetWizardButtons(PSWIZB_BACK | PSWIZB_NEXT);
	return CPropertyPage::OnSetActive();
}

LRESULT CProp2::OnWizardNext() 
{
	// TODO: Add your specialized code here and/or call the base class
	UpdateData();
	if(m_football || m_basketball || m_volleyball || m_swim)
	{
		return CPropertyPage::OnWizardNext();
	}
	else
	{
		MessageBox("请选择你的兴趣爱好！");
		return -1;
	}
}
