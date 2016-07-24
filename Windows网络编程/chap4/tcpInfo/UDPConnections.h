#if !defined(AFX_UDPCONNECTIONS_H__C3D0C057_618F_4F35_BA05_9631DE899FE8__INCLUDED_)
#define AFX_UDPCONNECTIONS_H__C3D0C057_618F_4F35_BA05_9631DE899FE8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UDPConnections.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUDPConnections dialog

class CUDPConnections : public CDialog
{
// Construction
public:
	CUDPConnections(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUDPConnections)
	enum { IDD = IDD_DIALOG6 };
	CListCtrl	m_list;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPConnections)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUDPConnections)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPCONNECTIONS_H__C3D0C057_618F_4F35_BA05_9631DE899FE8__INCLUDED_)
