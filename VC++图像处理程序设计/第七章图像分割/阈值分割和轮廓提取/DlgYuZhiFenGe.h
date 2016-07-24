#if !defined(AFX_DLGYUZHIFENGE_H__BA9C396D_BCD0_4A71_A146_F7C10076E017__INCLUDED_)
#define AFX_DLGYUZHIFENGE_H__BA9C396D_BCD0_4A71_A146_F7C10076E017__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgYuZhiFenGe.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgYuZhiFenGe dialog

class CDlgYuZhiFenGe : public CDialog
{
// Construction
public:
	CDlgYuZhiFenGe(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgYuZhiFenGe)
	enum { IDD = IDD_DIALOG_YuZhiFenGe };
	int		m_Yuzhi;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgYuZhiFenGe)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgYuZhiFenGe)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGYUZHIFENGE_H__BA9C396D_BCD0_4A71_A146_F7C10076E017__INCLUDED_)
