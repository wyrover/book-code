// NamedPipeSrvDoc.h : interface of the CNamedPipeSrvDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEDPIPESRVDOC_H__C220E2DE_51ED_4ECD_BDA6_9E565E74A68B__INCLUDED_)
#define AFX_NAMEDPIPESRVDOC_H__C220E2DE_51ED_4ECD_BDA6_9E565E74A68B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNamedPipeSrvDoc : public CDocument
{
protected: // create from serialization only
	CNamedPipeSrvDoc();
	DECLARE_DYNCREATE(CNamedPipeSrvDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamedPipeSrvDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNamedPipeSrvDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNamedPipeSrvDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDPIPESRVDOC_H__C220E2DE_51ED_4ECD_BDA6_9E565E74A68B__INCLUDED_)
