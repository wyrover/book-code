// ChatDlg.h : header file
//

#if !defined(AFX_CHATDLG_H__E4D0CF55_050F_4064_AD2F_3A1FB513BCF1__INCLUDED_)
#define AFX_CHATDLG_H__E4D0CF55_050F_4064_AD2F_3A1FB513BCF1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CChatDlg dialog
#define UM_SOCK		WM_USER+1
class CChatDlg : public CDialog
{
// Construction
public:
	BOOL InitSocket();
	CChatDlg(CWnd* pParent = NULL);	// standard constructor
	~CChatDlg();
// Dialog Data
	//{{AFX_DATA(CChatDlg)
	enum { IDD = IDD_CHAT_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChatDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CChatDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnBtnSend();
	//}}AFX_MSG
	afx_msg void OnSock(WPARAM,LPARAM);
	DECLARE_MESSAGE_MAP()
private:
	SOCKET m_socket;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHATDLG_H__E4D0CF55_050F_4064_AD2F_3A1FB513BCF1__INCLUDED_)
