#pragma once
#include "afxcmn.h"


// DrvDeletePage dialog

class DrvDeletePage : public CPropertyPage
{
	DECLARE_DYNAMIC(DrvDeletePage)

public:
	DrvDeletePage();
	virtual ~DrvDeletePage();

// Dialog Data
	enum { IDD = IDD_DRVDELETEPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnSetActive();
	virtual BOOL OnWizardFinish();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedDelete();
	
	CListCtrl m_lstctl_devices;
	CString m_strHID;
	CString m_strHIDPre;
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual LRESULT OnWizardBack();
};
