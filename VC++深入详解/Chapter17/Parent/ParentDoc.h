// ParentDoc.h : interface of the CParentDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PARENTDOC_H__9E3D66D7_BB93_4F85_96A6_F7AAAB269470__INCLUDED_)
#define AFX_PARENTDOC_H__9E3D66D7_BB93_4F85_96A6_F7AAAB269470__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CParentDoc : public CDocument
{
protected: // create from serialization only
	CParentDoc();
	DECLARE_DYNCREATE(CParentDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CParentDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CParentDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CParentDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PARENTDOC_H__9E3D66D7_BB93_4F85_96A6_F7AAAB269470__INCLUDED_)
