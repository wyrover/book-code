// FileDoc.h : interface of the CFileDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FILEDOC_H__057EBF92_758D_43B2_B392_2B756354D79C__INCLUDED_)
#define AFX_FILEDOC_H__057EBF92_758D_43B2_B392_2B756354D79C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CFileDoc : public CDocument
{
protected: // create from serialization only
	CFileDoc();
	DECLARE_DYNCREATE(CFileDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFileDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CFileDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEDOC_H__057EBF92_758D_43B2_B392_2B756354D79C__INCLUDED_)
