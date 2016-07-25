// sequenceMultDlg.h : header file
//

#if !defined(AFX_SEQUENCEMULTDLG_H__CDCBB3AA_26C8_4A59_AF62_612C60F68BD0__INCLUDED_)
#define AFX_SEQUENCEMULTDLG_H__CDCBB3AA_26C8_4A59_AF62_612C60F68BD0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include <time.h>
#include <sys/types.h>
#include <sys/timeb.h>
/////////////////////////////////////////////////////////////////////////////
// CSequenceMultDlg dialog

class CSequenceMultDlg : public CDialog
{
private:

	BOOL hasPrintInsert;
	BOOL hasPrintSelect;
	BOOL hasPrintHeap;
	BOOL hasPrintBubble;
	struct _timeb timetr1;
	struct _timeb timetr2;
	long milliseconds;
	long *data1, *data2, *data3, *data4, *data5,*data;
	int iDataLen;
	UINT m_nTimer;
	time_t sec1,sec2;
public:
	void PrintResult(long* Array, int iLength, int seqList);


// Construction
public:
	void getData();
	CSequenceMultDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSequenceMultDlg)
	enum { IDD = IDD_SEQUENCEMULT_DIALOG };
	CListCtrl	m_selectList;
	CListCtrl	m_quickList;
	CListCtrl	m_insertList;
	CListCtrl	m_headList;
	CListCtrl	m_bubbleList;
	CString	m_strData;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSequenceMultDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSequenceMultDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnButtonSort();
	afx_msg void OnButtonClear();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SEQUENCEMULTDLG_H__CDCBB3AA_26C8_4A59_AF62_612C60F68BD0__INCLUDED_)
