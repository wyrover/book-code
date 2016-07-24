// DownloadDlg.h : header file
//

#if !defined(AFX_DOWNLOADDLG_H__5C90CE2A_529B_11D5_B587_00E04C39B036__INCLUDED_)
#define AFX_DOWNLOADDLG_H__5C90CE2A_529B_11D5_B587_00E04C39B036__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "GetList.h"
#define IDC_GET_LIST	4321		// define any constant. ;-)
#include "httpdownload.h"

/////////////////////////////////////////////////////////////////////////////
// CDownloadDlg dialog

class CDownloadDlg : public CDialog
{
// Construction
public:
	DWORD Finish();
	CDownloadDlg(CWnd* pParent = NULL);	// standard constructor
	CHTTPDownload http;
	CString m_localfile;

// Dialog Data
	//{{AFX_DATA(CDownloadDlg)
	enum { IDD = IDD_DOWNLOAD_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownloadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CGetList m_ctrGetList;
	BOOL m_bFinished;
	HANDLE m_hThread[4];
	HANDLE m_hNotify;

	// Generated message map functions
	//{{AFX_MSG(CDownloadDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnStart();
	afx_msg void OnDestroy();
	afx_msg void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	void CreateThread();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOADDLG_H__5C90CE2A_529B_11D5_B587_00E04C39B036__INCLUDED_)
