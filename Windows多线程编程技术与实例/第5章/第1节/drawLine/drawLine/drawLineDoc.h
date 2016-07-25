// drawLineDoc.h : interface of the CDrawLineDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DRAWLINEDOC_H__428FD34B_193C_4511_BF1A_EC844C4725F1__INCLUDED_)
#define AFX_DRAWLINEDOC_H__428FD34B_193C_4511_BF1A_EC844C4725F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CDrawLineDoc : public CDocument
{
protected: // create from serialization only
	CDrawLineDoc();
	DECLARE_DYNCREATE(CDrawLineDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDrawLineDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDrawLineDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDrawLineDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DRAWLINEDOC_H__428FD34B_193C_4511_BF1A_EC844C4725F1__INCLUDED_)
