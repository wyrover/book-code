#if !defined(AFX_SEARCHLOGDLG_H__58BBB79B_2011_4BDF_9524_514D66EA0250__INCLUDED_)
#define AFX_SEARCHLOGDLG_H__58BBB79B_2011_4BDF_9524_514D66EA0250__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SearchLogDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSearchLogDlg dialog

struct searchLogData{ 
    COleDateTime TimeStamp;
    UINT Size;
    CString Mail_File;
    CString Sender;
    CString Subject;
    CString Receiver;  
};


class CSearchLogDlg : public CDialog
{
private:
	void TimeToStr(CTime& time1,CTime& time2,CString& strTime);
	BOOL GetChunk(FieldPtr pField, LPVOID lpData);
	searchLogData m_searchLogData;

// Construction
public:
	CSearchLogDlg(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSearchLogDlg)
	enum { IDD = IDD_SEARCHLOGDIALOG };
	CDateTimeCtrl	m_toTime2;
	CDateTimeCtrl	m_toTime1;
	CDateTimeCtrl	m_fromTime2;
	CDateTimeCtrl	m_fromTime1;
	CListCtrl	m_logList;
	CTime	m_fromTimeValue1;
	CTime	m_fromTimeValue2;
	CTime	m_toTimeValue1;
	CTime	m_toTimeValue2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSearchLogDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSearchLogDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSearch();
	afx_msg void OnSeemail();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEARCHLOGDLG_H__58BBB79B_2011_4BDF_9524_514D66EA0250__INCLUDED_)
