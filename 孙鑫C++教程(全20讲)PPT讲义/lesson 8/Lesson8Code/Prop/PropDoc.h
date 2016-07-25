// PropDoc.h : interface of the CPropDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PROPDOC_H__22531F7B_70D4_4678_AF21_1DDC7A666D01__INCLUDED_)
#define AFX_PROPDOC_H__22531F7B_70D4_4678_AF21_1DDC7A666D01__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPropDoc : public CDocument
{
protected: // create from serialization only
	CPropDoc();
	DECLARE_DYNCREATE(CPropDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPropDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPropDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPropDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPDOC_H__22531F7B_70D4_4678_AF21_1DDC7A666D01__INCLUDED_)
