// DSplitDoc.h : interface of the CDSplitDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_DSPLITDOC_H__76E949CD_9CAD_11D1_907F_00A024782894__INCLUDED_)
#define AFX_DSPLITDOC_H__76E949CD_9CAD_11D1_907F_00A024782894__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "CDIB.h"
#include "TuXiangHeChengDib.h"

class CDSplitDoc : public CDocument
{
protected: // create from serialization only
	CDSplitDoc();
	DECLARE_DYNCREATE(CDSplitDoc)

// Attributes
public:

// Operations
public:
   CDib CDib,CDibOld;
   TuXiangHeChengDib CDibNew;   
   CString filename;
   int statedoc;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDSplitDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CDSplitDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CDSplitDoc)
	afx_msg void OnFileopen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DSPLITDOC_H__76E949CD_9CAD_11D1_907F_00A024782894__INCLUDED_)
