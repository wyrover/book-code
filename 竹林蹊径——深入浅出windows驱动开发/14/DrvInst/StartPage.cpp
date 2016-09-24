// DrvStartPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrvInst.h"
#include "StartPage.h"
#include "setup.h"
#include "DrvInstSheetEx.h"
// DrvStartPage dialog

IMPLEMENT_DYNAMIC(DrvStartPage, CPropertyPage)

DrvStartPage::DrvStartPage()
	: CPropertyPage(DrvStartPage::IDD, 0)
	, m_nFunction(0)
	, m_nDrvRadio(0)
{
	//m_psp.dwFlags |= PSP_HIDEHEADER;
}

DrvStartPage::~DrvStartPage()
{
}

void DrvStartPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	DDX_Radio(pDX, IDRD_INSTALL, m_nDrvRadio);
	DDX_Control(pDX, IDSTA_MAIL, m_staLink_Mail);
	DDX_Control(pDX, IDSTA_CY001, m_ctl_sta_cy001);
	DDX_Control(pDX, IDSTA_TAOBAO, m_ctl_sta_taobao);
}


BEGIN_MESSAGE_MAP(DrvStartPage, CPropertyPage)
END_MESSAGE_MAP()


BOOL DrvStartPage::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_NEXT);
	return CPropertyPageEx::OnSetActive();
}


// DrvStartPage message handlers

BOOL DrvStartPage::OnInitDialog()
{
	CPropertyPage::OnInitDialog();

	//m_staLink_Mail.SetWindowText("");
	m_staLink_Mail.SetURL("mailto:ChinaHearing@gmail.comm");
	m_ctl_sta_cy001.SetURL("http://bbs.driverdevelop.com/read.php?fid=22&tid=119314&toread=1");
	m_ctl_sta_taobao.SetURL("http://item.taobao.com/auction/item_detail-0db1-a0c291b295a21e59db601e65166b98e1.jhtml");
	return TRUE;  
}

LRESULT DrvStartPage::OnWizardNext()
{
	UpdateData();

	CDrvInstSheet* pSheet = (CDrvInstSheet*)GetParent();
	ASSERT_KINDOF(CDrvInstSheet, pSheet);
	pSheet->setDrvMode(m_nDrvRadio);

	return CPropertyPage::OnWizardNext();
}
