#if !defined(AFX_IPSTATS_H__D7B19C28_5E11_4E63_A8B7_9BCEC3C033FB__INCLUDED_)
#define AFX_IPSTATS_H__D7B19C28_5E11_4E63_A8B7_9BCEC3C033FB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// IPStats.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CIPStats dialog

class CIPStats : public CDialog
{
// Construction
public:
	CIPStats(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CIPStats)
	enum { IDD = IDD_DIALOG3 };
	CString	m_strForward;
	CString	m_strDefaultTTL;
	CString	m_strDataRec;
	CString	m_strDataWithHdrErr;
	CString	m_strDataWithAddrErr;
	CString	m_strDataForw;
	CString	m_strDataWithProtErr;
	CString	m_strDataInDisc;
	CString	m_strDataDeliv;
	CString	m_strDataSnd;
	CString	m_strDataRoutDisc;
	CString	m_strDataNoRoutes;
	CString	m_strReassmTimeout;
	CString	m_strDataReqReassm;
	CString	m_strDataReassOK;
	CString	m_strDataReassmNoOK;
	CString	m_strDataFragOK;
	CString	m_strDataFragNOOK;
	CString	m_strFragsCrt;
	CString	m_strNoInterfaces;
	CString	m_strLocalIPAddrs;
	CString	m_strNoRoutes;
	CString	m_strTotalDisc;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CIPStats)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CIPStats)
	afx_msg void OnRefresh();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IPSTATS_H__D7B19C28_5E11_4E63_A8B7_9BCEC3C033FB__INCLUDED_)
