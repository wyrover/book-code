#pragma once

#include "StartPage.h"
#include "DrvInstPage.h"
#include "HIDPage.h"
#include "DrvDeletePage.h"
#include "FinishPage.h"

// CDrvInstSheet
class CDrvInstSheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CDrvInstSheet)

public:
	CDrvInstSheet(UINT nIDCaption, 
		CWnd* pParentWnd = NULL, 
		UINT iSelectPage = 0);

	CDrvInstSheet(LPCTSTR pszCaption, 
		CWnd* pParentWnd = NULL, 
		UINT iSelectPage = 0);

	CDrvInstSheet(UINT nIDCaption, CWnd* pParentWnd, UINT iSelectPage, 
		HBITMAP hbmWatermark, 
		HPALETTE hpalWatermark = NULL,
		HBITMAP hbmHeader = NULL);

	CDrvInstSheet(LPCTSTR pszCaption, CWnd* pParentWnd , UINT iSelectPage, 
		HBITMAP hbmWatermark,
		HPALETTE hpalWatermark = NULL, 
		HBITMAP hbmHeader = NULL);

	virtual ~CDrvInstSheet();

	void setDrvMode(bool bDelete)
	{
		if(bDelete)
			SetActivePage(&m_FinishPage1);
	}
protected:

	DrvStartPage m_StartPage;
	DrvInstPage m_DrvInstPage;
	CHIDPage	m_HIDPage;
	DrvDeletePage m_DrvDelPage;
	FinishPage  m_FinishPage1;
	FinishPage  m_FinishPage2;

	void InitSheet();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnDestroy();
};


