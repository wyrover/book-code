// viewNICDlg.h : header file
//

#if !defined(AFX_VIEWNICDLG_H__0E7173FB_AA31_4D42_8902_E12A205AE5CD__INCLUDED_)
#define AFX_VIEWNICDLG_H__0E7173FB_AA31_4D42_8902_E12A205AE5CD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CViewNICDlg dialog
#include "MibAccess.h"

class CViewNICDlg : public CDialog
{
// Construction
public:
	CViewNICDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CViewNICDlg)
	enum { IDD = IDD_VIEWNIC_DIALOG };
	CString	m_NICInfoDescription;
	CString	m_NICInfoIP;
	CString	m_NICInfoMAC;
	UINT	m_NICInfoNum;
	CString	m_NICInfoSubnetMask;
	UINT	m_NICInfoType;
	BOOL	m_bDialup;
	BOOL	m_bLoopback;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewNICDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON				m_hIcon;
	UINT				m_NICIndex;
	UINT				m_NICCount;
	tSTRUCTNICINFO		*m_pNICInfo;

	int					GetNICInfo();
	void				DisplayNICInfo(UINT NICIndex, tSTRUCTNICINFO *pNICInfo);

	// Generated message map functions
	//{{AFX_MSG(CViewNICDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnNicNext();
	afx_msg void OnNicPrevious();
	afx_msg void OnNicRefresh();
	afx_msg void OnWindowPosChanged(WINDOWPOS FAR* lpwndpos);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWNICDLG_H__0E7173FB_AA31_4D42_8902_E12A205AE5CD__INCLUDED_)
