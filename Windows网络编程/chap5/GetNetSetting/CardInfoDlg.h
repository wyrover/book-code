#if !defined(AFX_CARDINFODLG_H__050FAAFF_2EAB_492B_A64E_7CBA2C04A89D__INCLUDED_)
#define AFX_CARDINFODLG_H__050FAAFF_2EAB_492B_A64E_7CBA2C04A89D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CardInfoDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CCardInfoDlg dialog

class CCardInfoDlg : public CDialog
{
// Construction
public:
	CCardInfoDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CCardInfoDlg)
	enum { IDD = IDD_CARDINFO };
	CStatic	m_staticIPAddress;
	CEdit	m_addrMAC;
	CListBox	m_listNetGate;
	CStatic	m_staticIsRoute;
	CStatic	m_staticIsDns;
	CListCtrl	m_listctrlIP;
	CStatic	m_staticDomain;
	CListBox	m_listDNS;
	CStatic	m_staticHostName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCardInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CCardInfoDlg)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CARDINFODLG_H__050FAAFF_2EAB_492B_A64E_7CBA2C04A89D__INCLUDED_)
