// MyBrowserDoc.h : interface of the CMyBrowserDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYBROWSERDOC_H__9CD5434B_D9A0_4315_9045_B04A7973F1F0__INCLUDED_)
#define AFX_MYBROWSERDOC_H__9CD5434B_D9A0_4315_9045_B04A7973F1F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CMyBrowserDoc : public CDocument
{
protected: // create from serialization only
	CMyBrowserDoc();
	DECLARE_DYNCREATE(CMyBrowserDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBrowserDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBrowserDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMyBrowserDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBROWSERDOC_H__9CD5434B_D9A0_4315_9045_B04A7973F1F0__INCLUDED_)
