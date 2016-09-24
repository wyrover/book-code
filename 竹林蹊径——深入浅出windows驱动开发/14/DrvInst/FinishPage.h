#pragma once


// FinishPage dialog

class FinishPage : public CPropertyPage
{
	DECLARE_DYNAMIC(FinishPage)

public:
	FinishPage();
	virtual ~FinishPage();

// Dialog Data
	enum { IDD = IDD_FINISHPAGE };

protected:
	virtual BOOL OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
};
