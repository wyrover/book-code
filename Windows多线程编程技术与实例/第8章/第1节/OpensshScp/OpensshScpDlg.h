// OpensshScpDlg.h : header file
//

#if !defined(AFX_OPENSSHSCPDLG_H__DFEFE032_2AF0_4BD9_9F74_3A804C5FB96A__INCLUDED_)
#define AFX_OPENSSHSCPDLG_H__DFEFE032_2AF0_4BD9_9F74_3A804C5FB96A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// COpensshScpDlg dialog
typedef struct  {
	HWND hWnd;
	CString strPara;
}ThreadPara;

class COpensshScpDlg : public CDialog
{
// Construction
public:
	COpensshScpDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(COpensshScpDlg)
	enum { IDD = IDD_OPENSSHSCP_DIALOG };
	CIPAddressCtrl	m_IPAddrDesCtrl;
	CString	m_strSource;
	CString	m_strUser;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpensshScpDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(COpensshScpDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButScp();
	afx_msg void OnButtonSelfile();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	CString m_strFileName;
	CWinThread* m_pThread;
//	ThreadPara* m_pThreadPara ;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENSSHSCPDLG_H__DFEFE032_2AF0_4BD9_9F74_3A804C5FB96A__INCLUDED_)
