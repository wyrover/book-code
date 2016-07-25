// receiveProcessDlg.h : header file
//

#if !defined(AFX_RECEIVEPROCESSDLG_H__10909753_5D5C_406B_A7C6_10C8F6665480__INCLUDED_)
#define AFX_RECEIVEPROCESSDLG_H__10909753_5D5C_406B_A7C6_10C8F6665480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CReceiveProcessDlg dialog

class CReceiveProcessDlg : public CDialog
{
// Construction
public:
	CReceiveProcessDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CReceiveProcessDlg)
	enum { IDD = IDD_RECEIVEPROCESS_DIALOG };
	CString	m_strText2;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CReceiveProcessDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL pasteText(COleDataObject* pDataObject);
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CReceiveProcessDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBRecieve();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RECEIVEPROCESSDLG_H__10909753_5D5C_406B_A7C6_10C8F6665480__INCLUDED_)
