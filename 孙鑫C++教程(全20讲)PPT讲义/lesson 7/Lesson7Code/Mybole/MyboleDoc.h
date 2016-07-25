// MyboleDoc.h : interface of the CMyboleDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBOLEDOC_H__E4D34DFA_2BEE_4CE3_865E_EEBD5B0989AB__INCLUDED_)
#define AFX_MYBOLEDOC_H__E4D34DFA_2BEE_4CE3_865E_EEBD5B0989AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyboleDoc : public CDocument
{
protected: // create from serialization only
	CMyboleDoc();
	DECLARE_DYNCREATE(CMyboleDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyboleDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyboleDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyboleDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBOLEDOC_H__E4D34DFA_2BEE_4CE3_865E_EEBD5B0989AB__INCLUDED_)
