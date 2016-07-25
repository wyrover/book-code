// OpenglExaView.h : interface of the COpenglExaView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLEXAVIEW_H__74F26409_F447_4153_AF70_5A883B8B0781__INCLUDED_)
#define AFX_OPENGLEXAVIEW_H__74F26409_F447_4153_AF70_5A883B8B0781__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COpenglExaView : public CView
{
protected: // create from serialization only
	COpenglExaView();
	DECLARE_DYNCREATE(COpenglExaView)

// Attributes
public:
	COpenglExaDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenglExaView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_GLPixelIndex;
	virtual ~COpenglExaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COpenglExaView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HGLRC m_hGLContext;
	BOOL CreateViewGLContext(HDC hDC);
	BOOL SetWindowPixelFormat(HDC hDC);
};

#ifndef _DEBUG  // debug version in OpenglExaView.cpp
inline COpenglExaDoc* COpenglExaView::GetDocument()
   { return (COpenglExaDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLEXAVIEW_H__74F26409_F447_4153_AF70_5A883B8B0781__INCLUDED_)
