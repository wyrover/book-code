// ImageMatchingDoc.h : interface of the CImageMatchingDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEMATCHINGDOC_H__EAC2B755_9586_448F_9BC7_822017AB39F0__INCLUDED_)
#define AFX_IMAGEMATCHINGDOC_H__EAC2B755_9586_448F_9BC7_822017AB39F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDib.h"

class CImageMatchingDoc : public CDocument
{
protected: // create from serialization only
	CImageMatchingDoc();
	DECLARE_DYNCREATE(CImageMatchingDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageMatchingDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	int InitDocVariable();
	int FreeDocVariable();
	CDib* m_pDibInit;
	CDib* m_pDibTest;
	int m_nColorIndex;
	virtual ~CImageMatchingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageMatchingDoc)
	afx_msg void OnFileSaveAs();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEMATCHINGDOC_H__EAC2B755_9586_448F_9BC7_822017AB39F0__INCLUDED_)
