// wuziqiDoc.h : interface of the CWuziqiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WUZIQIDOC_H__C166A278_11DA_4B35_9AA4_C1E873971D6D__INCLUDED_)
#define AFX_WUZIQIDOC_H__C166A278_11DA_4B35_9AA4_C1E873971D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWuziqiDoc : public CDocument
{
protected: // create from serialization only
	CWuziqiDoc();
	DECLARE_DYNCREATE(CWuziqiDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWuziqiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWuziqiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWuziqiDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WUZIQIDOC_H__C166A278_11DA_4B35_9AA4_C1E873971D6D__INCLUDED_)
