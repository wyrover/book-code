#if !defined(AFX_DLGZOOM_H__491A6F55_F95C_470D_9AEE_2E4ABDBD9C2C__INCLUDED_)
#define AFX_DLGZOOM_H__491A6F55_F95C_470D_9AEE_2E4ABDBD9C2C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgZoom.h : header file
//
/////////////////////////////////////////////////////////////////////////////
// CDlgZoom dialog

class CDlgZoom : public CDialog
{
	// Construction
public:
	CDlgZoom(CWnd* pParent = NULL);   // standard constructor

	// Dialog Data
	//{{AFX_DATA(CDlgZoom)
	enum { IDD = IDD_dlg_zoom };
	float	m_horZoom;
	float	m_verZoom;
	//}}AFX_DATA


	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgZoom)
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgZoom)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGZOOM_H__491A6F55_F95C_470D_9AEE_2E4ABDBD9C2C__INCLUDED_)
