// StyleDoc.h : interface of the CStyleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_STYLEDOC_H__EBE32AA3_7028_4DF5_9F19_556AB84EAB68__INCLUDED_)
#define AFX_STYLEDOC_H__EBE32AA3_7028_4DF5_9F19_556AB84EAB68__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CStyleDoc : public CDocument
{
protected: // create from serialization only
	CStyleDoc();
	DECLARE_DYNCREATE(CStyleDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStyleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStyleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CStyleDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STYLEDOC_H__EBE32AA3_7028_4DF5_9F19_556AB84EAB68__INCLUDED_)
