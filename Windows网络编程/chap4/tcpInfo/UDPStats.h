#if !defined(AFX_UDPSTATS_H__09165895_6C34_4DE5_90F2_A46972862554__INCLUDED_)
#define AFX_UDPSTATS_H__09165895_6C34_4DE5_90F2_A46972862554__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UDPStats.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CUDPStats dialog

class CUDPStats : public CDialog
{
// Construction
public:
	CUDPStats(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CUDPStats)
	enum { IDD = IDD_DIALOG2 };
	CString	m_strDataRec;
	CString	m_strDataNoPort;
	CString	m_strErrOnRec;
	CString	m_strDataSnd;
	CString	m_strListenConn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPStats)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CUDPStats)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPSTATS_H__09165895_6C34_4DE5_90F2_A46972862554__INCLUDED_)
