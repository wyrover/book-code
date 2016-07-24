// test_audio_loopView.h : interface of the CTest_audio_loopView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEST_AUDIO_LOOPVIEW_H__7F10950D_FC88_11D1_8334_00104B7B317F__INCLUDED_)
#define AFX_TEST_AUDIO_LOOPVIEW_H__7F10950D_FC88_11D1_8334_00104B7B317F__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <windows.h>
#include <mmsystem.h>
#include "afxtempl.h"
#include <afxcoll.h>

#include "linedata.h"
#include "typedef.h"

class CTest_audio_loopView : public CFormView
{
protected: // create from serialization only
	CTest_audio_loopView();
	DECLARE_DYNCREATE(CTest_audio_loopView)

public:
	//{{AFX_DATA(CTest_audio_loopView)
	enum{ IDD = IDD_TEST_AUDIO_LOOP_FORM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CTest_audio_loopDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTest_audio_loopView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTest_audio_loopView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTest_audio_loopView)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in test_audio_loopView.cpp
inline CTest_audio_loopDoc* CTest_audio_loopView::GetDocument()
   { return (CTest_audio_loopDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TEST_AUDIO_LOOPVIEW_H__7F10950D_FC88_11D1_8334_00104B7B317F__INCLUDED_)
