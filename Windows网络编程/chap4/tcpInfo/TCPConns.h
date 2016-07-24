#if !defined(AFX_TCPCONNS_H__6F480C58_0008_4273_B83E_592D6C73286A__INCLUDED_)
#define AFX_TCPCONNS_H__6F480C58_0008_4273_B83E_592D6C73286A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TCPConns.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTCPConns dialog

class CTCPConns : public CDialog
{
// Construction
public:
	CTCPConns(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTCPConns)
	enum { IDD = IDD_DIALOG5 };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCPConns)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTCPConns)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPCONNS_H__6F480C58_0008_4273_B83E_592D6C73286A__INCLUDED_)
