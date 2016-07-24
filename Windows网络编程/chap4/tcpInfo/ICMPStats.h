#if !defined(AFX_ICMPSTATS_H__0EC9C10F_9B55_4978_B052_745122AB7C3F__INCLUDED_)
#define AFX_ICMPSTATS_H__0EC9C10F_9B55_4978_B052_745122AB7C3F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ICMPStats.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CICMPStats dialog

class CICMPStats : public CDialog
{
// Construction
public:
	CICMPStats(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CICMPStats)
	enum { IDD = IDD_DIALOG4 };
	CString	m_strMsgsI;
	CString	m_strErrorsI;
	CString	m_strDstUnrI;
	CString	m_TTLExcI;
	CString	m_strParamProbsI;
	CString	m_strSrcQI;
	CString	m_strRedirI;
	CString	m_strEchoI;
	CString	m_strEchoReplI;
	CString	m_strTimeI;
	CString	m_strTimeReplI;
	CString	m_strAddrI;
	CString	m_strAddrReplI;
	CString	m_strMsgsO;
	CString	m_strErrorsO;
	CString	m_strDstUnrO;
	CString	m_strTTLExcO;
	CString	m_strParamProbsO;
	CString	m_strSrcQO;
	CString	m_strRedirO;
	CString	m_strEchoO;
	CString	m_strEchoReplO;
	CString	m_strTimeO;
	CString	m_strTimeReplO;
	CString	m_strAddrO;
	CString	m_strAddrReplO;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CICMPStats)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CICMPStats)
	virtual BOOL OnInitDialog();
	afx_msg void OnRefresh();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ICMPSTATS_H__0EC9C10F_9B55_4978_B052_745122AB7C3F__INCLUDED_)
