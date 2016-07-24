#if !defined(AFX_RESULTWIN_H__BBBC6499_DD76_4124_9E48_3E9E9F704ADD__INCLUDED_)
#define AFX_RESULTWIN_H__BBBC6499_DD76_4124_9E48_3E9E9F704ADD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// resultwin.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// resultwin dialog

class resultwin : public CDialog
{
// Construction
public:
	resultwin(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(resultwin)
	enum { IDD = IDD_DIALOG2 };
	CString	m_Who;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(resultwin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(resultwin)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESULTWIN_H__BBBC6499_DD76_4124_9E48_3E9E9F704ADD__INCLUDED_)
