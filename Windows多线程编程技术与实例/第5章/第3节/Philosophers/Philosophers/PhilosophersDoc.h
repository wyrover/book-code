// PhilosophersDoc.h : interface of the CPhilosophersDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_PHILOSOPHERSDOC_H__748DB278_3EC1_4606_A320_BF33DD1A5354__INCLUDED_)
#define AFX_PHILOSOPHERSDOC_H__748DB278_3EC1_4606_A320_BF33DD1A5354__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CPhilosophersDoc : public CDocument
{
protected: // create from serialization only
	CPhilosophersDoc();
	DECLARE_DYNCREATE(CPhilosophersDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPhilosophersDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CPhilosophersDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CPhilosophersDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PHILOSOPHERSDOC_H__748DB278_3EC1_4606_A320_BF33DD1A5354__INCLUDED_)
