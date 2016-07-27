// CDCStep3Doc.h : interface of the CCDCStep3Doc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CDCSTEP3DOC_H__B1D5178E_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_)
#define AFX_CDCSTEP3DOC_H__B1D5178E_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000


class CCDCStep3Doc : public CDocument
{
protected: // create from serialization only
	CCDCStep3Doc();
	DECLARE_DYNCREATE(CCDCStep3Doc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCDCStep3Doc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCDCStep3Doc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CCDCStep3Doc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CDCSTEP3DOC_H__B1D5178E_AAE9_11D2_BC14_02608C6CA09B__INCLUDED_)
