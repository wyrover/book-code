// OpenglExaDoc.h : interface of the COpenglExaDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_OPENGLEXADOC_H__99795462_2A53_4F2C_BF36_D0FE29D1A30E__INCLUDED_)
#define AFX_OPENGLEXADOC_H__99795462_2A53_4F2C_BF36_D0FE29D1A30E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class COpenglExaDoc : public CDocument
{
protected: // create from serialization only
	COpenglExaDoc();
	DECLARE_DYNCREATE(COpenglExaDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenglExaDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~COpenglExaDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(COpenglExaDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENGLEXADOC_H__99795462_2A53_4F2C_BF36_D0FE29D1A30E__INCLUDED_)
