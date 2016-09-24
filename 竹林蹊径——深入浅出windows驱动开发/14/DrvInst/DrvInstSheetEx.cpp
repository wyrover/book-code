// DrvInstSheetEx.cpp : implementation file
//

#include "stdafx.h"
#include "DrvInst.h"
#include "DrvInstSheetEx.h"
#include "setup.h"


// CDrvInstSheet

IMPLEMENT_DYNAMIC(CDrvInstSheet, CPropertySheet)

void CDrvInstSheet::InitSheet()
{
	m_StartPage.Construct(DrvStartPage::IDD, 0, IDS_START_T, IDS_START_ST);
	m_DrvInstPage.Construct(DrvInstPage::IDD, 0, IDS_INF_T, IDS_INF_ST);
	m_HIDPage.Construct(CHIDPage::IDD, 0, IDS_HID_T, IDS_HID_ST);
	m_DrvDelPage.Construct(DrvDeletePage::IDD, 0, IDS_DEL_T, IDS_DEL_ST);

	AddPage(&m_StartPage);

	AddPage(&m_DrvInstPage);
	AddPage(&m_HIDPage);
	AddPage(&m_FinishPage1);

	AddPage(&m_DrvDelPage);
	AddPage(&m_FinishPage2);
	
	m_psh.dwFlags |= PSH_WIZARD97;
}

CDrvInstSheet::CDrvInstSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
	InitSheet();
}

CDrvInstSheet::CDrvInstSheet(LPCTSTR pszCaption, CWnd* pParentWnd, UINT iSelectPage)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{

	InitSheet();
}

CDrvInstSheet::CDrvInstSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage, 
	HBITMAP hbmWatermark, 
	HPALETTE hpalWatermark,
	HBITMAP hbmHeader)
	:CPropertySheet(nIDCaption, pParentWnd, iSelectPage, hbmWatermark, hpalWatermark, hbmHeader)
{

	InitSheet();
}

CDrvInstSheet::CDrvInstSheet(LPCTSTR pszCaption, CWnd* pParentWnd , UINT iSelectPage, 
	HBITMAP hbmWatermark,
	HPALETTE hpalWatermark, 
	HBITMAP hbmHeader)
	:CPropertySheet(pszCaption, pParentWnd, iSelectPage, hbmWatermark, hpalWatermark, hbmHeader)
{

	InitSheet();
}

CDrvInstSheet::~CDrvInstSheet()
{
}


BEGIN_MESSAGE_MAP(CDrvInstSheet, CPropertySheet)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDrvInstSheet message handlers

BOOL CDrvInstSheet::OnInitDialog()
{
	BOOL bResult = CPropertySheet::OnInitDialog();

	CWnd *pWnd = GetDlgItem(IDHELP);
	if(pWnd)
	{
		pWnd->ShowWindow(FALSE);   
		pWnd->EnableWindow(FALSE);
	}

	FindAllInfFiles(NULL);
	return bResult;
}

void CDrvInstSheet::OnClose()
{
	CPropertySheet::OnClose();
}

void CDrvInstSheet::OnDestroy()
{
	FreeAllInfFiles();
	FreeDevices();
	CPropertySheet::OnDestroy();
}
