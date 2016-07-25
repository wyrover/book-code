// ASCDoc.h : interface of the CASCDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_ASCDOC_H__356EC9A7_9AF6_4BBA_B841_B6FD3112B4D0__INCLUDED_)
#define AFX_ASCDOC_H__356EC9A7_9AF6_4BBA_B841_B6FD3112B4D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CASCDoc : public CDocument
{
protected: // create from serialization only
	CASCDoc();
	DECLARE_DYNCREATE(CASCDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CASCDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CASCDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CASCDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASCDOC_H__356EC9A7_9AF6_4BBA_B841_B6FD3112B4D0__INCLUDED_)
