// FinishPage.cpp : implementation file
//

#include "stdafx.h"
#include "DrvInst.h"
#include "FinishPage.h"


// FinishPage dialog

IMPLEMENT_DYNAMIC(FinishPage, CPropertyPage)

FinishPage::FinishPage()
	: CPropertyPage(FinishPage::IDD)
{
	m_psp.dwFlags |= PSP_HIDEHEADER;
}

FinishPage::~FinishPage()
{
}

void FinishPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(FinishPage, CPropertyPage)
END_MESSAGE_MAP()


BOOL FinishPage::OnSetActive()
{
	CPropertySheet* pSheet = (CPropertySheet*)GetParent();
	ASSERT_KINDOF(CPropertySheet, pSheet);
	pSheet->SetWizardButtons(PSWIZB_BACK|PSWIZB_FINISH);

	return CPropertyPageEx::OnSetActive();
}

