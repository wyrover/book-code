// ViewAdapterInfoDlg.h : header file
//

#if !defined(AFX_VIEWADAPTERINFODLG_H__C91BFC24_E1E1_4516_8319_B285ECDB7971__INCLUDED_)
#define AFX_VIEWADAPTERINFODLG_H__C91BFC24_E1E1_4516_8319_B285ECDB7971__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CViewAdapterInfoDlg dialog

class CViewAdapterInfoDlg : public CDialog
{
public:
	void GetInfo();
	void ParseData();
// Construction
public:
	CViewAdapterInfoDlg(CWnd* pParent = NULL);	// standard constructor
	~CViewAdapterInfoDlg();

// Dialog Data
	//{{AFX_DATA(CViewAdapterInfoDlg)
	enum { IDD = IDD_GETMACADDRESS_DIALOG };
	CButton	m_buttonnext;
	CString	m_macaddress;
	CString	m_description;
	CString	m_type;
	CString	m_subnet;
	CString	m_IpAddress;
	CString	m_gateway;
	CString	m_PrimaryWinsServer;
	CString	m_dhcp;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewAdapterInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CViewAdapterInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnRefresh();
	afx_msg void OnBUTTONNext();	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWADAPTERINFODLG_H__C91BFC24_E1E1_4516_8319_B285ECDB7971__INCLUDED_)
