#pragma once
#include "afxcmn.h"
#include "afxwin.h"


// CHIDPage dialog

class CHIDPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CHIDPage)

public:
	CHIDPage();
	virtual ~CHIDPage();

// Dialog Data
	enum { IDD = IDD_HIDPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_lstctl_drv_status;
	CListCtrl m_lstctl_HID;
	CButton m_btn_update;
	afx_msg void OnBnClickedUpdate();
};
