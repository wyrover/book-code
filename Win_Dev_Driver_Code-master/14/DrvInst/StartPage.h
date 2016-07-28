#pragma once
#include "afxwin.h"
#include "XHyperLink.h"


// DrvStartPage dialog

class DrvStartPage : public CPropertyPage
{
	DECLARE_DYNAMIC(DrvStartPage)

public:
	DrvStartPage();
	virtual ~DrvStartPage();


// Dialog Data
	enum { IDD = IDD_STARTPAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	int m_nFunction;
	virtual BOOL OnSetActive();
	virtual BOOL OnInitDialog();
	virtual LRESULT OnWizardNext();
	int m_nDrvRadio;
	CXHyperLink m_staLink_Mail;
	CXHyperLink m_ctl_sta_cy001;
	CXHyperLink m_ctl_sta_taobao;
};
