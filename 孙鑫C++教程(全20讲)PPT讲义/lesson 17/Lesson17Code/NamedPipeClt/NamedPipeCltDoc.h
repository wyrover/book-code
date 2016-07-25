// NamedPipeCltDoc.h : interface of the CNamedPipeCltDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_NAMEDPIPECLTDOC_H__057A924E_E1B5_4043_812E_9DCDEBA792DB__INCLUDED_)
#define AFX_NAMEDPIPECLTDOC_H__057A924E_E1B5_4043_812E_9DCDEBA792DB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CNamedPipeCltDoc : public CDocument
{
protected: // create from serialization only
	CNamedPipeCltDoc();
	DECLARE_DYNCREATE(CNamedPipeCltDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CNamedPipeCltDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CNamedPipeCltDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CNamedPipeCltDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_NAMEDPIPECLTDOC_H__057A924E_E1B5_4043_812E_9DCDEBA792DB__INCLUDED_)
