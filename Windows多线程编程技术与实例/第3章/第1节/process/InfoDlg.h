#if !defined(AFX_INFODLG_H__DCEFCEEC_BB17_45D2_ABB1_EEFA7805B27C__INCLUDED_)
#define AFX_INFODLG_H__DCEFCEEC_BB17_45D2_ABB1_EEFA7805B27C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInfoDlg dialog

class CInfoDlg : public CDialog
{

// Construction
public:
	CInfoDlg(CWnd* pParent = NULL);   // standard constructor
	
	DWORD dwSize,cntUsage,processID,defaultHeapID,moduleID,cntThreads,parentProcessID,dwFlags;
	long classBase;
// Dialog Data
	//{{AFX_DATA(CInfoDlg)
	enum { IDD = IDD_INFORMATION };
	CString	m_strExeFile;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CInfoDlg)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_INFODLG_H__DCEFCEEC_BB17_45D2_ABB1_EEFA7805B27C__INCLUDED_)
