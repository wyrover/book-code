// CTelnetDoc.h : interface of the CTelnetDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CTELNETDOC_H__FE95BC8D_9860_11D3_8CD5_00C0F0405B24__INCLUDED_)
#define AFX_CTELNETDOC_H__FE95BC8D_9860_11D3_8CD5_00C0F0405B24__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CTelnetDoc : public CDocument
{
protected: // create from serialization only
	CTelnetDoc();
	DECLARE_DYNCREATE(CTelnetDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTelnetDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CTelnetDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CTelnetDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CTELNETDOC_H__FE95BC8D_9860_11D3_8CD5_00C0F0405B24__INCLUDED_)
