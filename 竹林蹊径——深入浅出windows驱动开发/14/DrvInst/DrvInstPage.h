#pragma once
#include "afxwin.h"


// DrvInstPage dialog

class DrvInstPage : public CPropertyPage
{
	DECLARE_DYNAMIC(DrvInstPage)

public:
	DrvInstPage();
	virtual ~DrvInstPage();

// Dialog Data
	enum { IDD = IDD_DRVINSTPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CListCtrl m_lstbox_inf;
	CEdit m_edt_curdir;
	char  m_asPath[MAX_PATH];

	void RefreshListCtl();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedSelDir();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT OnWizardNext();
};
