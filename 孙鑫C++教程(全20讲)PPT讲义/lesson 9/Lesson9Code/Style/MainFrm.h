// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__119CAE05_AEB3_4522_9052_51E710656808__INCLUDED_)
#define AFX_MAINFRM_H__119CAE05_AEB3_4522_9052_51E710656808__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define UM_PROGRESS		WM_USER+1

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	CStatusBar  m_wndStatusBar;
protected:  // control bar embedded members
	
	CToolBar    m_wndToolBar;
	CToolBar	m_newToolBar;
	CProgressCtrl m_progress;

// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnTest();
	afx_msg void OnViewNewtool();
	afx_msg void OnUpdateViewNewtool(CCmdUI* pCmdUI);
	afx_msg void OnPaint();
	//}}AFX_MSG
	afx_msg void OnProgress();
	DECLARE_MESSAGE_MAP()
private:
	HICON m_hIcons[3];
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__119CAE05_AEB3_4522_9052_51E710656808__INCLUDED_)
