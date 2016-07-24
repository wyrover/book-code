#if !defined(AFX_TCPSTATS_H__EA49FB45_881F_40C7_9285_139B85ACD671__INCLUDED_)
#define AFX_TCPSTATS_H__EA49FB45_881F_40C7_9285_139B85ACD671__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TCPStats.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTCPStats dialog

class CTCPStats : public CDialog
{
// Construction
public:
	CTCPStats(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CTCPStats)
	enum { IDD = IDD_DIALOG1 };
	CString	m_strAlgo;
	CString	m_strMinTimeout;
	CString	m_strMaxTimeout;
	CString	m_strMaxConn;
	CString	m_strActiveConn;
	CString	m_strPassiveConn;
	CString	m_strFailedAttempts;
	CString	m_strConnReset;
	CString	m_strEsatblishedConn;
	CString	m_strSegmentsRec;
	CString	m_strSegmentSnd;
	CString	m_strSegRetrans;
	CString	m_strErrRec;
	CString	m_strSegWithRST;
	CString	m_strCumConn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTCPStats)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTCPStats)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TCPSTATS_H__EA49FB45_881F_40C7_9285_139B85ACD671__INCLUDED_)
