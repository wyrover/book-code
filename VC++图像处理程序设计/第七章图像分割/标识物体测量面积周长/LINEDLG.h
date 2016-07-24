#if !defined(AFX_LINEDLG_H__76283426_4E5F_431D_A904_FD237B4D3324__INCLUDED_)
#define AFX_LINEDLG_H__76283426_4E5F_431D_A904_FD237B4D3324__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LINEDLG.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// LINEDLG dialog

class LINEDLG : public CDialog
{
// Construction
public:
	LINEDLG(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(LINEDLG)
	enum { IDD = IDD_LINE_DLG };
	int		m_shumu;
	int		m_zongshu;
	CString	m_line;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(LINEDLG)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(LINEDLG)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LINEDLG_H__76283426_4E5F_431D_A904_FD237B4D3324__INCLUDED_)
