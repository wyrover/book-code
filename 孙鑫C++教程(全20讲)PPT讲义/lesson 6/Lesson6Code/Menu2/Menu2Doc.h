// Menu2Doc.h : interface of the CMenu2Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MENU2DOC_H__8D6B3A1C_EE7D_4471_8E50_7AEDB577B46B__INCLUDED_)
#define AFX_MENU2DOC_H__8D6B3A1C_EE7D_4471_8E50_7AEDB577B46B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMenu2Doc : public CDocument
{
protected: // create from serialization only
	CMenu2Doc();
	DECLARE_DYNCREATE(CMenu2Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMenu2Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMenu2Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMenu2Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MENU2DOC_H__8D6B3A1C_EE7D_4471_8E50_7AEDB577B46B__INCLUDED_)
