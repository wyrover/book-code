// ImageProcessingDoc.h : interface of the CImageProcessingDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_IMAGEPROCESSINGDOC_H__613B030D_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
#define AFX_IMAGEPROCESSINGDOC_H__613B030D_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CDib.h"

class CImageProcessingDoc : public CDocument
{
protected: // create from serialization only
	CImageProcessingDoc();
	DECLARE_DYNCREATE(CImageProcessingDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CImageProcessingDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	int m_nColorIndex;		// Î±²ÊÉ«±àÂë±àºÅ
	CDib* m_pDibTest;
	int InitDocVariable();
	int FreeDocVariable();
	CDib* m_pDibInit;
	virtual ~CImageProcessingDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CImageProcessingDoc)
	afx_msg void OnFileSaveAs();
	afx_msg void OnFileReload();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_IMAGEPROCESSINGDOC_H__613B030D_9C37_11D6_A6C4_00D0094CCE8A__INCLUDED_)
