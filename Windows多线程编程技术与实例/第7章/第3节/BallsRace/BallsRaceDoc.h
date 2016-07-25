// BallsRaceDoc.h : interface of the CBallsRaceDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_BALLSRACEDOC_H__F5A68BF0_EF46_4B46_8F1F_C560CFFE465F__INCLUDED_)
#define AFX_BALLSRACEDOC_H__F5A68BF0_EF46_4B46_8F1F_C560CFFE465F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CBallsRaceDoc : public CDocument
{
protected: // create from serialization only
	CBallsRaceDoc();
	DECLARE_DYNCREATE(CBallsRaceDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBallsRaceDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBallsRaceDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CBallsRaceDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BALLSRACEDOC_H__F5A68BF0_EF46_4B46_8F1F_C560CFFE465F__INCLUDED_)
