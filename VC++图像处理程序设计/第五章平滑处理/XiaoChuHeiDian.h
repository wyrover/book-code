#if !defined(AFX_XIAOCHUHEIDIAN_H__20774180_7748_4C4D_A364_B0BF30A8D451__INCLUDED_)
#define AFX_XIAOCHUHEIDIAN_H__20774180_7748_4C4D_A364_B0BF30A8D451__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// XiaoChuHeiDian.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XiaoChuHeiDian dialog

class XiaoChuHeiDian : public CDialog
{
// Construction
public:
	XiaoChuHeiDian(CWnd* pParent = NULL);   // standard constructor
	int n;
// Dialog Data
	//{{AFX_DATA(XiaoChuHeiDian)
	enum { IDD = IDD_DIALOG1 };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XiaoChuHeiDian)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(XiaoChuHeiDian)
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XIAOCHUHEIDIAN_H__20774180_7748_4C4D_A364_B0BF30A8D451__INCLUDED_)
