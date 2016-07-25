// GraphicDoc.h : interface of the CGraphicDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_GRAPHICDOC_H__E9414E31_AC6E_49DC_93FD_52375BE6EFD7__INCLUDED_)
#define AFX_GRAPHICDOC_H__E9414E31_AC6E_49DC_93FD_52375BE6EFD7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CGraphicDoc : public CDocument
{
protected: // create from serialization only
	CGraphicDoc();
	DECLARE_DYNCREATE(CGraphicDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGraphicDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CGraphicDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CGraphicDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GRAPHICDOC_H__E9414E31_AC6E_49DC_93FD_52375BE6EFD7__INCLUDED_)
